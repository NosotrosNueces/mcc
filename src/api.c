#include "bot.h"
#include "protocol.h"
#include "handlers.h"

#define DEFAULT_PROTOCOL_VERSION 47

void register_defaults(bot_t *bot) {
    // Basic event handling that you should always want
    register_event(bot, LOGIN, 0x02, login_success_handler);
    register_event(bot, PLAY, 0x00, keepalive_handler);
    register_event(bot, PLAY, 0x08, position_handler);
}

void login(bot_t *bot, char *server_address, int port) {
    join_server(bot, server_address, port);
    send_handshaking_serverbound_handshake(bot, DEFAULT_PROTOCOL_VERSION,
                                           "", port, 2);
    send_login_serverbound_login(bot, bot->name);
}


