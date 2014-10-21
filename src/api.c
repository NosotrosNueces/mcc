#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "handlers.h"

#define PROTOCOL_VERSION 47

void nsleep(uint64_t ns)
{
    int finished;
    struct timespec interval;
    struct timespec remainder;

    interval.tv_sec = ns / 1000000000L;
    interval.tv_nsec = ns % 1000000000L;

    do {
        finished = nanosleep(&interval, &remainder);
        interval = remainder;
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
