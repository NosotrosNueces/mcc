#include <stdio.h>
#include <string.h>
#include "api.h"
#include "bot.h"
#include "protocol.h"
#include <pthread.h>

void login_success_handler(bot_t *bot, char *uuid, char *username)
{
    printf("Logged in: %s\n", username);

    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->_data->current_state = PLAY;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);

    //static char msg[64];
    //sprintf(msg, "Hi guys, %s here.", p->username);
    //send_play_serverbound_chat(bot, msg);
}

void keepalive_handler(bot_t *bot, vint32_t keepalive_id)
{
    send_play_serverbound_keepalive(bot, keepalive_id);
}

void join_game_handler(bot_t *bot, 
        int32_t entity_id, 
        uint8_t gamemode, 
        int8_t dimension, 
        uint8_t difficulty, 
        uint8_t max_players, 
        char* level_type)
{

    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->eid = entity_id;
    bot->gamemode = gamemode;
    bot->dimension = dimension;
    bot->difficulty = difficulty;
    bot->max_players = max_players;
    bot->level_type = level_type;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);

    // Set client settings.
    send_play_serverbound_client_settings(bot, "en_GB", 1, 0, 1, 0x7f);
    // Set client version (plugin messages currently broken).
    //send_play_serverbound_plugin_message(bot, "MC|Brand",
    //                                     (uint8_t *)"vanilla");
}

void chat_handler(bot_t *bot, char *json, int8_t position)
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
            execute_command(bot, command, *saveptr);
            free(saveptr);
        }
    }
    free(sender);
    free(msg);
}

void update_health_handler(bot_t *bot, 
        float health, 
        vint32_t food, 
        float saturation)
{
    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->health = (int)(health);
    bot->food = food;
    bot->saturation = saturation;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);
}

/* Rename plz */
void respawn_handler(bot_t *bot,
        float health,
        vint32_t food,
        float saturation)
{
    if (health <= 0) {
        send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true);
        send_play_serverbound_player_status(bot, 0);
    }
}

void position_handler(bot_t *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch,
        int8_t flags)
{
    // acquire lock
    pthread_mutex_lock(&bot->bot_mutex);

    bot->x = x;
    bot->y = y;
    bot->z = z;
    bot->yaw = yaw;
    bot->pitch = pitch;
    bot->flags = flags;

    // release lock
    pthread_mutex_unlock(&bot->bot_mutex);

    //printf("current (x, y, z): (%f, %f, %f)\n", bot->x, bot->y, bot->z);
    send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true);
}
