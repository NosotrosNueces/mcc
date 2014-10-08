#include <stdio.h>
#include "bot.h"
#include "protocol.h"

void login_success_handler(bot_t *bot, void *vp) {
    login_clientbound_success_t *p = (login_clientbound_success_t *)vp;
    printf("Logged in: %s\n", p->username);
    bot->_data->current_state = PLAY;

    //static char msg[64];
    //sprintf(msg, "Hi guys, %s here.", p->username);
    //send_play_serverbound_chat(bot, msg);
}

void keepalive_handler(bot_t *bot, void *vp) {
    play_clientbound_keepalive_t *p = (play_clientbound_keepalive_t *)vp;
    send_play_serverbound_keepalive(bot, p->keepalive_id);
}

void join_game_handler(bot_t *bot, void *vp) { 
    play_clientbound_join_game_t *p = (play_clientbound_join_game_t *)vp;

    bot->eid = p->entity_id;
    bot->gamemode = p->gamemode;
    bot->dimension = p->dimension;
    bot->difficulty = p->difficulty;
    bot->max_players = p->max_players;
    bot->level_type = p->level_type;

    // Set client settings.
    send_play_serverbound_client_settings(bot, "en_GB", 1, 0, 1, 0x7f);
    // Set client version (plugin messages currently broken).
    //send_play_serverbound_plugin_message(bot, "MC|Brand",
    //                                     (uint8_t *)"vanilla");
}

void update_health_handler(bot_t *bot, void *vp) {
    play_clientbound_update_health_t *p =
        (play_clientbound_update_health_t *)vp;

    bot->health = (int)(p->health);
    bot->food = p->food;
    bot->saturation = p->saturation;
}

void position_handler(bot_t *bot, void *vp) {
    play_clientbound_position_t *p =
        (play_clientbound_position_t *)vp;

    bot->x = p->x;
    bot->y = p->y;
    bot->z = p->z;
    bot->yaw = p->yaw;
    bot->pitch = p->pitch;
    bot->flags = p->flags;

    //printf("current (x, y, z): (%f, %f, %f)\n", bot->x, bot->y, bot->z);
    send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true); 
}
