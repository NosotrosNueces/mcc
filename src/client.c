#include "bot.h"
#include "client.h"
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include "protocol.h"
#include <errno.h>
#include <stdio.h>

#define MAX_BOTS 256
#define MAILBOX_CAPACITY 1 << 10
#define POLL_TIMEOUT 10000

//uint32_t num_bots;
//bot_t *bot_net[MAX_BOTS];

bot_t *bot_list;
uint32_t num_bots;
pthread_t *bot_threads;
pthread_key_t bot_key;

void *mailbox[MAILBOX_CAPACITY];
uint32_t head = 0, tail = 0, len = 0;

void client_run(bot_t *, uint32_t);

void *receiver(void *ignore);
void *bot_thread(void *bot);

void client_run(bot_t *bots, uint32_t num) {
    // create 1 thread for receiving packets, and 1 for each bot
    int i;
    bot_list = bots;
    num_bots = num;
    pthread_key_create(&bot_key, NULL);
   
    // create & start listener thread
    pthread_t event_listener;
    pthread_create(&event_listener, NULL, receiver, NULL);

    // create all the bot threads
    for(i = 0; i < num; i++) {
        pthread_create(bot_threads + i, NULL, bot_thread, bot_list + i); 
    }
   
    // wait for all threads to finish
    // TODO: support for exit codes
    pthread_join(event_listener, NULL);
    for(i = 0; i < num; i++) {
        pthread_join(bot_threads[i], NULL);
    }
}

void *bot_thread(void *bot) {
    pthread_setspecific(bot_key, bot);
    ((bot_t *)bot)->bot_main(bot);
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
        ready = poll(fds, num_bots, -1);
        if(ready < 0)
            perror(errno);
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
    bot_t *bot = pthread_getspecific(bot_key);
    callback_decode(bot);
}
