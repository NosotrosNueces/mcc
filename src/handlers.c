#include <stdio.h>
#include <string.h>
#include "api.h"
#include "bot.h"
#include "protocol.h"
#include <pthread.h>

void mcc_login_success_handler(bot_t *bot, char *uuid, char *username)
{
    printf("Logged in: %s\n", username);

    pthread_mutex_lock(&bot->bot_mutex);

    bot->_data->current_state = PLAY;

    pthread_mutex_unlock(&bot->bot_mutex);
}

void mcc_keepalive_handler(bot_t *bot, vint32_t keepalive_id)
{
    send_play_serverbound_keepalive(bot, keepalive_id);
}

void mcc_join_game_handler(bot_t *bot,
                           int32_t entity_id,
                           uint8_t gamemode,
                           int8_t dimension,
                           uint8_t difficulty,
                           uint8_t max_players,
                           char* level_type)
{

    pthread_mutex_lock(&bot->bot_mutex);

    bot->eid = entity_id;
    bot->gamemode = gamemode;
    bot->dimension = dimension;
    bot->difficulty = difficulty;
    bot->max_players = max_players;
    bot->level_type = level_type;

    pthread_mutex_unlock(&bot->bot_mutex);

    // Set client settings.
    send_play_serverbound_client_settings(bot, "en_GB", 1, 0, 1, 0x7f);
    // Set client version (plugin messages currently broken).
    //send_play_serverbound_plugin_message(bot, "MC|Brand",
    //                                     (uint8_t *)"vanilla");
}

void mcc_chat_handler(bot_t *bot, char *json, int8_t position)
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

void mcc_update_health_handler(bot_t *bot,
                               float health,
                               vint32_t food,
                               float saturation)
{
    pthread_mutex_lock(&bot->bot_mutex);

    bot->health = (int)(health);
    bot->food = food;
    bot->saturation = saturation;

    pthread_mutex_unlock(&bot->bot_mutex);
}

void mcc_item_change_handler(bot_t *bot, int8_t slot)
{
    pthread_mutex_lock(&bot->bot_mutex);

    bot->slot = slot;

    pthread_mutex_unlock(&bot->bot_mutex);
    printf("Slot update: %d\n", slot+1);
}

void mcc_autorespawn_handler(bot_t *bot,
                             float health,
                             vint32_t food,
                             float saturation)
{
    if (health <= 0) {
        send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true);
        send_play_serverbound_player_status(bot, 0);
    }
}

void mcc_position_handler(bot_t *bot,
                          double x,
                          double y,
                          double z,
                          float yaw,
                          float pitch,
                          int8_t flags)
{
    pthread_mutex_lock(&bot->bot_mutex);

    bot->x = x;
    bot->y = y;
    bot->z = z;
    bot->yaw = yaw;
    bot->pitch = pitch;
    bot->flags = flags;

    pthread_mutex_unlock(&bot->bot_mutex);

    //printf("current (x, y, z): (%f, %f, %f)\n", bot->x, bot->y, bot->z);
    send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true);
}
