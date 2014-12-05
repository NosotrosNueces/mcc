#include <stdio.h>
#include "bot.h"
#include "protocol.h"
#include <pthread.h>

void timer_echo_pos(bot_t *bot, void *data)
{
    send_play_serverbound_player_move(bot, bot->x, bot->y, bot->z, true);
}
