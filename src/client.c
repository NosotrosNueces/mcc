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
pthread_t *bot_threads;

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
    struct pollfd *fds = calloc(num_bots, sizeof(struct pollfd));
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
                pthread_kill(bot_threads[i], SIGUSR1);
            }
        }
    }
}

void signal_handler(int signal) {
    bot_t *bot;
    pthread_t me = pthread_self();
    // TODO: optimize this
    for (uint32_t i = 0; i < num_bots; i++) {
        if (bot_threads[i] == me)
            bot = bot_list + i;
    }
    uint32_t pid = receive_packet(bot);
    void* packet = decode_table[bot->current_state][pid]();
    function *func = bot->callbacks[bot->current_state][pid];
    while (func) {
        func->f(packet);
        func = func->next;
    }
}
