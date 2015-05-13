#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "api.h"
#include "handlers.h"
#include "timers.h"

#define STAY 0
#define FOLLOW 1

typedef struct entity {
    vint32_t eid;
} entity_t;

typedef struct bot_globals {
    entity_t *target;
    int state;
} bot_globals_t;

void slave_main(void *vbot)
{
    bot_t *bot = (bot_t *)vbot;
    struct timeval delay = {0, 500000};
    register_timer(bot, delay, -1, timer_echo_pos);
    ((bot_globals_t *)bot->state)->state = FOLLOW;

    msleep(500);
    send_play_serverbound_player_status(bot, 0);
    send_play_serverbound_item_change(bot, 0);

    pause();
}

void slave_command(bot_t *bot, char *command, char *strargs)
{
    if (strcmp(command, "slot") == 0) {
        char **saveptr = calloc(1, sizeof(char *));
        bool valid_input = true;
        char *token = NULL;
        int slot_no;

        valid_input &= next_int_token(&slot_no, strargs, saveptr);

        // Input checking
        if (slot_no < 1 || 9 < slot_no)
            valid_input = false;
        // Actual packet is 0-indexed.
        --slot_no;

        // Ensure there are no trailing tokens.
        token = strtok_r(*saveptr, " ", saveptr);
        if (token)
            valid_input = false;

        free(saveptr);
        if (valid_input) {
            printf("SLOT CHANGE: (%d)\n", slot_no);
            send_play_serverbound_item_change(bot, slot_no);
        } else {
            send_play_serverbound_chat(bot,
                                       "Invalid arguments for SLOT command "
                                       "(1 <= slot <= 9).");
        }
    } else if (strcmp(command, "stay") == 0) {
        pthread_mutex_lock(&bot->bot_mutex);

        ((bot_globals_t *)bot->state)->state = STAY;

        pthread_mutex_unlock(&bot->bot_mutex);

        printf("Follow mode OFF.\n");
        send_play_serverbound_chat(bot, "Stopped following.");
    } else if (strcmp(command, "follow") == 0) {
        pthread_mutex_lock(&bot->bot_mutex);

        ((bot_globals_t *)bot->state)->state = FOLLOW;

        pthread_mutex_unlock(&bot->bot_mutex);

        printf("Follow mode ON.\n");
        send_play_serverbound_chat(bot, "Currently following.");
    } else if (strcmp(command, "reaquire") == 0) {
        bool had_target = false;
        pthread_mutex_lock(&bot->bot_mutex);

        if (((bot_globals_t *)bot->state)->target) {
            had_target = true;
            free(((bot_globals_t *)bot->state)->target);
            ((bot_globals_t *)bot->state)->target = NULL;
        }

        pthread_mutex_unlock(&bot->bot_mutex);

        if (had_target) {
            printf("Waiting for new player to follow.\n");
            send_play_serverbound_chat(bot,
                                       "Waiting for a new target to appear.");
        } else {
            printf("Waiting...\n");
            send_play_serverbound_chat(bot, "Waiting...");
        }
    } else if (strcmp(command, "help") == 0) {
        send_play_serverbound_chat(bot, "\\slot <slot>\n");
        send_play_serverbound_chat(bot, "\\stay\n");
        send_play_serverbound_chat(bot, "\\follow\n");
        send_play_serverbound_chat(bot, "\\reaquire\n");
    } else {
        printf("Command not implemented: %s\n", command);
        send_play_serverbound_chat(bot,
                                   "Command not implemented. Try \\help.");
    }
}

void slave_chat_handler(bot_t *bot, char *json, int8_t position)
{
    char *msg = NULL;
    char *sender = NULL;
    decode_chat_json(json, &msg, &sender);
    // Ensure that we do not echo ourselves,
    // and that we received a chat message (not a server message).
    if (msg && sender && strcmp(sender, bot->name)) {
        // Commands to bots start with a backslash.
        // Only operate on non-empty commands.
        if (msg[0] == '\\' && msg[1]) {
            // Parse space delimited tokens.
            char **saveptr = calloc(1, sizeof(char *));
            char *command = strtok_r(msg+1, " ", saveptr);
            slave_command(bot, command, *saveptr);
            free(saveptr);
        }
    }
    free(sender);
    free(msg);
}

void move(bot_t *bot, float dx, float dy, float dz)
{
    pthread_mutex_lock(&bot->bot_mutex);

    bot->x += dx;
    bot->y += dy;
    bot->z += dz;

    send_play_serverbound_player_move(bot,
                                      dx+bot->x, dy+bot->y, dz+bot->z, true);

    pthread_mutex_unlock(&bot->bot_mutex);
}

void target_teleport_handler(bot_t *bot, vint32_t entity_id,
                             int32_t x,
                             int32_t y,
                             int32_t z,
                             int8_t yaw,
                             int8_t pitch,
                             bool on_ground)
{
    printf("Entity %d teleported: %f %f %f.\n", entity_id, x/32., y/32., z/32.);

    bool will_teleport;
    pthread_mutex_lock(&bot->bot_mutex);

    bot_globals_t *state = (bot_globals_t *)bot->state;
    will_teleport = state->target
                    && state->target->eid == entity_id
                    && state->state == FOLLOW;

    pthread_mutex_unlock(&bot->bot_mutex);

    if (will_teleport) {
        set_pos(bot, x/32., y/32., z/32.);
    }
}

void target_move_handler(bot_t *bot, vint32_t entity_id,
                         int8_t dx,
                         int8_t dy,
                         int8_t dz,
                         bool on_ground)
{
    bool will_move;
    pthread_mutex_lock(&bot->bot_mutex);

    bot_globals_t *state = (bot_globals_t *)bot->state;
    will_move = state->target
                && state->target->eid == entity_id
                && ((bot_globals_t *)bot->state)->state == FOLLOW;

    pthread_mutex_unlock(&bot->bot_mutex);

    if (will_move) {
        move(bot, dx/32., dy/32., dz/32.);
    }
}

void player_watcher(bot_t *bot, vint32_t entity_id,
                    __uint128_t uuid,
                    int32_t x,
                    int32_t y,
                    int32_t z,
                    int8_t yaw,
                    int8_t pitch,
                    int16_t item,
                    metadata_t metadata)
{
    pthread_mutex_lock(&bot->bot_mutex);

    bot_globals_t *state = (bot_globals_t *)bot->state;
    if (!state->target) {
        printf("Target player eid: %d\n", entity_id);

        entity_t *e = calloc(1, sizeof(entity_t));
        e->eid = entity_id;
        ((bot_globals_t *)bot->state)->target = e;
    }

    pthread_mutex_unlock(&bot->bot_mutex);
}

bot_t *init_slave(char *name, char *server_name, int port)
{
    bot_t *bot = init_bot(name, *slave_main);
    bot->state = calloc(1, sizeof(bot_globals_t));

    register_defaults(bot);
    register_play_clientbound_chat(bot, slave_chat_handler);
    register_play_clientbound_update_health(bot, mcc_autorespawn_handler);
    register_play_clientbound_spawn_player(bot, player_watcher);
    register_play_clientbound_entity_move(bot, target_move_handler);
    register_play_clientbound_entity_teleport(bot, target_teleport_handler);

    login(bot, server_name, port);

    return bot;
}
