#include "bot.h"
#include "client.h"
#include <pthread.h>

#define MAX_BOTS 256
#define MAILBOX_CAPACITY 1 << 10

//uint32_t num_bots;
//bot_t *bot_net[MAX_BOTS];

bot_t *bot_list;

void *mailbox[MAILBOX_CAPACITY];
uint32_t head = 0, tail = 0, len = 0;

void client_run(bot_t *, uint32_t);

void client_run(bot_t *bots, uint32_t num_bots) {
    // create 1 thread for receiving packets, and 1 for each bot

}

void *receiver(void *param) {

}
