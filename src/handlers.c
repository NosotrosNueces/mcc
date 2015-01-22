#include <stdio.h>
#include <string.h>
#include "api.h"
#include "bot.h"
#include "protocol.h"
#include <pthread.h>

void login_success_handler(bot_t *bot, void *vp)
{
    login_clientbound_success_t *p = (login_clientbound_success_t *)vp;
    printf("Logged in: %s\n", p->username);

    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->_data->current_state = PLAY;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);

    //static char msg[64];
    //sprintf(msg, "Hi guys, %s here.", p->username);
    //send_play_serverbound_chat(bot, msg);
}

void keepalive_handler(bot_t *bot, void *vp)
{
    play_clientbound_keepalive_t *p = (play_clientbound_keepalive_t *)vp;
    send_play_serverbound_keepalive(bot, p->keepalive_id);
}

void join_game_handler(bot_t *bot, void *vp)
{
    play_clientbound_join_game_t *p = (play_clientbound_join_game_t *)vp;

    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->eid = p->entity_id;
    bot->gamemode = p->gamemode;
    bot->dimension = p->dimension;
    bot->difficulty = p->difficulty;
    bot->max_players = p->max_players;
    bot->level_type = p->level_type;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);

    // Set client settings.
    send_play_serverbound_client_settings(bot, "en_GB", 1, 0, 1, 0x7f);
    // Set client version (plugin messages currently broken).
    //send_play_serverbound_plugin_message(bot, "MC|Brand",
    //                                     (uint8_t *)"vanilla");
}

void chat_handler(bot_t *bot, void *vp)
{
    play_clientbound_chat_t *p =
        (play_clientbound_chat_t *)vp;
    char *msg = NULL;
    char *sender = NULL;
    decode_chat_json(p->json, &msg, &sender);
    // Ensure that we do not echo ourselves,
    // and that we received a chat message (not a server message).
    if (msg && sender && strcmp(sender, bot->name)) {
        // Commands to bots start with a backslash.
        // Only operate on non-empty commands.
        if (msg[0] == '\\' && msg[1]) {
            // Parse space delimited tokens.
            char **saveptr = calloc(1, sizeof(char *));
            char *command = strtok_r(msg+1, " ", saveptr);
            execute_command(bot, command, *saveptr);
            free(saveptr);
        }
    }
    free(sender);
    free(msg);
}

void update_health_handler(bot_t *bot, void *vp)
{
    play_clientbound_update_health_t *p =
        (play_clientbound_update_health_t *)vp;

    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->health = (int)(p->health);
    bot->food = p->food;
    bot->saturation = p->saturation;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);
}

void respawn_handler(bot_t *bot, void *vp)
{
    play_clientbound_update_health_t *p =
        (play_clientbound_update_health_t *) vp;
    if (p->health <= 0) {
        send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true);
        send_play_serverbound_player_status(bot, 0);
    }
}

void position_handler(bot_t *bot, void *vp)
{
    play_clientbound_position_t *p =
        (play_clientbound_position_t *)vp;

    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->x = p->x;
    bot->y = p->y;
    bot->z = p->z;
    bot->yaw = p->yaw;
    bot->pitch = p->pitch;
    bot->flags = p->flags;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);

    //printf("current (x, y, z): (%f, %f, %f)\n", bot->x, bot->y, bot->z);
    send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true);
}
