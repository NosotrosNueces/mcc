#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "handlers.h"

#define PROTOCOL_VERSION 47

void msleep(uint64_t ms)
{
    int finished;
    struct timespec req;
    struct timespec rem;

    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;

    do {
        finished = nanosleep(&req, &rem);
        req = rem;
    } while (finished == -1);
}

void register_defaults(bot_t *bot)
{
    // Basic event handling that you should always want
    register_event(bot, LOGIN, 0x02, login_success_handler);
    register_event(bot, PLAY, 0x00, keepalive_handler);
    register_event(bot, PLAY, 0x01, join_game_handler);
    register_event(bot, PLAY, 0x06, update_health_handler);
    register_event(bot, PLAY, 0x08, position_handler);
}

void login(bot_t *bot, char *server_address, int port)
{
    join_server(bot, server_address, port);
    send_handshaking_serverbound_handshake(bot, PROTOCOL_VERSION, "", port, 2);
    send_login_serverbound_login(bot, bot->name);
}
