#include "bot.h"
#include "client.h"
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>

#define MAX_BOTS 256
#define MAILBOX_CAPACITY 1 << 10
#define POLL_TIMEOUT 10000

//uint32_t num_bots;
//bot_t *bot_net[MAX_BOTS];

bot_t *bot_list;
uint32_t num_bots;
pid_t *bot_threads;

void *mailbox[MAILBOX_CAPACITY];
uint32_t head = 0, tail = 0, len = 0;

void client_run(bot_t *, uint32_t);

void client_run(bot_t *bots, uint32_t num) {
    // create 1 thread for receiving packets, and 1 for each bot
    bot_list = bots;
    num_bots = num;
}

void *receiver(void *ignore) {
    int i;
    int ready;
    pollfd *fds = calloc(num_bots, sizeof(pollfd));
    for (i = 0; i < num_bots; i++) {
        fds[i].fd = bot_list[i].socketfd;
        fds[i].events = POLLIN;
    }
    while (1) {
        //ready = poll(fds, num_bots, -1);
        ready = poll(fds, num_bots, POLL_TIMEOUT);
        for (i = 0; i < num_bots; i++) {
            // read packet
            // send signal to corresponding thread
            if (fds[i].revents & POLLIN ) {
                // socket is ready to be read from
            }
        }
    }
}

void client_run(bot_t *bot, uint32_t num_bots) {
     
}
