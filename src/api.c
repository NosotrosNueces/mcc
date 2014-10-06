#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "handlers.h"

#define DEFAULT_PROTOCOL_VERSION 47

int msleep(unsigned long ms) {
    struct timespec req = {0};
    time_t sec = (int)(ms/1000);
    ms = ms - (sec*1000);
    req.tv_sec = sec;
    req.tv_nsec = ms*1000000L;
    while(nanosleep(&req, &req)== -1) continue;
    return 1;
}

void register_defaults(bot_t *bot) {
    // Basic event handling that you should always want
    register_event(bot, LOGIN, 0x02, login_success_handler);
    register_event(bot, PLAY, 0x00, keepalive_handler);
    register_event(bot, PLAY, 0x01, join_game_handler);
    register_event(bot, PLAY, 0x08, position_handler);
}

void login(bot_t *bot, char *server_address, int port) {
    join_server(bot, server_address, port);
    send_handshaking_serverbound_handshake(bot, DEFAULT_PROTOCOL_VERSION,
                                           "", port, 2);
    send_login_serverbound_login(bot, bot->name);
}


