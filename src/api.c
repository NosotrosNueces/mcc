#include <string.h>
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

void place_held(bot_t *bot, long x, long y, long z) {
    long long loc = ((x & 0x3FFFFFF) << 38) |
                    ((y-1 & 0xFFF) << 26) |
                    (z & 0x3FFFFFF);
    unsigned char *location = (char *)calloc(8, sizeof(char));
    for(int i = 0; i < 8; i++)
        *(location+i) = (loc >> ((7-i)*8));
    unsigned char buf[20] = {0x13, 0x08,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // location
        0x01, // direction
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // slot data (unused)
        0x00, 0x00, 0x00, // cursor position (unused)
    };
    // 4 = +Y, 1 = -Z, 2 = +Y
    memcpy(buf+2, location, sizeof(uint64_t));
    printf("Sending:\n");
    for (int i = 0; i < 20; i++) {
        printf("%02X ", buf[i]);
        if (i == 1 || i == 9 || i == 10 || i == 16 || i == 19) {
            printf("\n");
        }
    }
    printf("...\n");
    send_raw(bot, (void *)buf, 2+8+10);
}
