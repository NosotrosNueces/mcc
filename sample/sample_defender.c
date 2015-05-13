#include <unistd.h>
#include <stdio.h>
#include <string.h>
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

typedef struct entity_node {
    vint32_t entity;
    struct entity_node *next;
} entity_node_t;

typedef struct bot_globals {
    entity_node_t targets;

    entity_t *target;
    int state;
} bot_globals_t;

void insert_target(bot_t *bot, vint32_t eid)
{
    pthread_mutex_lock(&bot->bot_mutex);

    entity_node_t *p = &((bot_globals_t *)bot->state)->targets;
    while (p->next) {
        p = p->next;
    }
    p->entity = eid;
    p->next = calloc(1, sizeof(entity_node_t));

    pthread_mutex_unlock(&bot->bot_mutex);
}

void remove_target(bot_t *bot, vint32_t eid)
{
    pthread_mutex_lock(&bot->bot_mutex);

    entity_node_t *p = &((bot_globals_t *)bot->state)->targets;
    while (p->next) {
        if (p->next->entity == eid) {
            p->next = p->next->next;
        }
        p = p->next;
    }

    pthread_mutex_unlock(&bot->bot_mutex);
}

bool exists_target(bot_t *bot, vint32_t eid)
{
    bool ret = false;
    pthread_mutex_lock(&bot->bot_mutex);

    entity_node_t *p = &((bot_globals_t *)bot->state)->targets;
    while (p->next) {
        if (p->entity == eid) {
            ret = true;
            break;
        }
        p = p->next;
    }

    pthread_mutex_unlock(&bot->bot_mutex);
    return ret;
}

void protect(bot_t *bot, vint32_t eid)
{
    //pthread_mutex_lock(&bot->bot_mutex);
    //int8_t old_slot = bot->slot;
    //pthread_mutex_unlock(&bot->bot_mutex);

    send_play_serverbound_player_look(bot, 0, 0, true);
    send_play_serverbound_animation(bot);

    //send_play_serverbound_item_change(bot, 0);
    send_play_serverbound_entity_use(bot, eid, 1, 0, 0, 0);
    //send_play_serverbound_item_change(bot, 1);
}

void entity_handler(bot_t *bot, vint32_t entity_id,
                    uint8_t type,
                    int32_t x,
                    int32_t y,
                    int32_t z,
                    int8_t yaw,
                    int8_t pitch,
                    int8_t head_pitch,
                    int16_t dx,
                    int16_t dy,
                    int16_t dz,
                    metadata_t metadata)
{
    if (type >= 50) insert_target(bot, entity_id);
}

void entity_move_handler(bot_t *bot, vint32_t entity_id,
                         int8_t dx,
                         int8_t dy,
                         int8_t dz,
                         bool on_ground)
{
    if (exists_target(bot, entity_id)) protect(bot, entity_id);
}

void entity_status_handler(bot_t *bot, int32_t entity_id, int8_t status)
{
    if (status == 3) remove_target(bot, entity_id);
}

void where(bot_t *bot)
{
    char buf[128] = {0};
    sprintf(buf, "Current position: %f %f %f.\n", bot->x, bot->y, bot->z);
    send_play_serverbound_chat(bot, buf);
}

void defender_command(bot_t *bot, char *command, char *strargs)
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
            pthread_mutex_lock(&bot->bot_mutex);

            bot->slot = slot_no;
            send_play_serverbound_item_change(bot, slot_no);

            pthread_mutex_unlock(&bot->bot_mutex);
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
    } else if (strcmp(command, "where") == 0) {
        where(bot);
    } else if (strcmp(command, "help") == 0) {
        send_play_serverbound_chat(bot, "\\slot <slot>\n");
        send_play_serverbound_chat(bot, "\\stay\n");
        send_play_serverbound_chat(bot, "\\follow\n");
        send_play_serverbound_chat(bot, "\\reaquire\n");
        send_play_serverbound_chat(bot, "\\where\n");
    } else {
        printf("Command not implemented: %s\n", command);
        send_play_serverbound_chat(bot,
                                   "Command not implemented. Try \\help.");
    }
}

void defender_chat_handler(bot_t *bot, char *json, int8_t position)
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
            defender_command(bot, command, *saveptr);
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
    //printf("Entity %d teleported: %f %f %f.\n", entity_id, x/32., y/32., z/32.);

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

void defender_main(void *vbot)
{
    bot_t *bot = (bot_t *)vbot;
    struct timeval delay = {0, 500000};
    register_timer(bot, delay, -1, timer_echo_pos);
    ((bot_globals_t *)bot->state)->state = STAY;

    msleep(300);
    send_play_serverbound_item_change(bot, 0);
    msleep(5000);
    printf("Current position: %f %f %f.\n", bot->x, bot->y, bot->z);
    while (1) {
        msleep(30*60*1000);
        where(bot);
    }

    pause();
}

bot_t *init_defender(char *name, char *server_name, int port)
{
    bot_t *bot = init_bot(name, *defender_main);
    bot->state = calloc(1, sizeof(bot_globals_t));

    register_defaults(bot);
    register_play_clientbound_chat(bot, defender_chat_handler);
    register_play_clientbound_update_health(bot, mcc_autorespawn_handler);

    register_play_clientbound_spawn_mob(bot, entity_handler);
    register_play_clientbound_entity_move(bot, entity_move_handler);
    register_play_clientbound_entity_status(bot, entity_status_handler);

    register_play_clientbound_spawn_player(bot, player_watcher);
    register_play_clientbound_entity_move(bot, target_move_handler);
    register_play_clientbound_entity_teleport(bot, target_teleport_handler);

    login(bot, server_name, port);

    return bot;
}
