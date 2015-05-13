#include <unistd.h>
#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "api.h"
#include "handlers.h"
#include "timers.h"

typedef struct bot_globals {
    int state;
} bot_globals_t;



void slave_main(void *vbot)
{
    bot_t *bot = (bot_t *)vbot;
    struct timeval delay = {0, 500000};
    register_timer(bot, delay, -1, timer_echo_pos);

    msleep(500);
    send_play_serverbound_player_status(bot, 0);
    send_play_serverbound_item_change(bot, 0);

    pause();
}

bot_t *init_slave(char *name, char *server_name, int port)
{
    bot_t *bot = init_bot(name, *slave_main);
    bot->state = calloc(1, sizeof(bot_globals_t));

    register_defaults(bot);
    register_play_clientbound_update_health(bot, mcc_autorespawn_handler);

    login(bot, server_name, port);

    return bot;
}
