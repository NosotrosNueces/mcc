#include <stdio.h>
#include "bot.h"
#include "protocol.h"

void login_success_handler(bot_t *bot, void *vp) {
    login_clientbound_success_t *p = (login_clientbound_success_t *)vp;
    printf("Logged in: %s\n", p->username);
    bot->current_state = PLAY;
    //static char msg[64];
    //sprintf(msg, "Hi guys, %s here.", p->username);
    //send_play_serverbound_chat(bot, msg);
}

void keepalive_handler(bot_t *bot, void *vp) {
    play_clientbound_keepalive_t *p = (play_clientbound_keepalive_t *)vp;
    send_play_serverbound_keepalive(bot, p->keepalive_id);
}

void position_handler(bot_t *bot, void *vp) {
    play_clientbound_position_t *p =
        (play_clientbound_position_t *)vp;
    //printf("current (x, y, z): (%f, %f, %f)\n", p->x, p->y, p->z);
    send_play_serverbound_player_move(bot, p->x, p->y, p->z, true); 
}
