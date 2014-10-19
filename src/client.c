#include "bot.h"
#include "protocol.h"
#include "marshal.h"
#include "client.h"
#include "pipe.h"
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>

#define MAX_BOTS 256
#define MAILBOX_CAPACITY 1 << 10
#define POLL_TIMEOUT 10000

//uint32_t num_bots;
//bot_t *bot_net[MAX_BOTS];

uint32_t num_bots;
bot_t **bot_list;
pthread_t *bot_threads;
pthread_t *receivers;
pthread_t *callbackers;
pipe_t **pipes;

void *receiver(void *index);
void *callbacker(void *index);
void *bot_thread(void *bot);

void client_run(bot_t **bots, uint32_t num) {
    // create 1 thread for receiving packets, and 1 for each bot
    uint64_t i;
    num_bots = num;
    bot_list = bots;

    // create all bot pipes
    pipes = calloc(num_bots, sizeof (pipe_t*));
    for (i = 0; i < num; i++) {
        pipes[i] = pipe_new(sizeof(void *), 0);
    }

    // create and start listener threads
    receivers = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(receivers + i, NULL, receiver, (void *)i);

    // create and start callback threads
    callbackers = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(callbackers + i, NULL, callbacker, (void *)i);

    // create all the bot threads
    bot_threads = calloc(num, sizeof (pthread_t));
    for(i = 0; i < num; i++)
        pthread_create(bot_threads + i, NULL, bot_thread, bot_list[i]);

    // wait for all threads to finish
    // TODO: support for exit codes
    for(i = 0; i < num; i++) {
        pthread_join(bot_threads[i], NULL);
        pthread_join(receivers[i], NULL);
        pthread_join(callbackers[i], NULL);
    }

    for (i = 0; i < num; i++)
        pipe_free(pipes[i]);

    free(bot_threads);
    free(receivers);
    free(callbackers);
}

void *bot_thread(void *bot) {
    ((bot_t *)bot)->_data->bot_main(bot);
    return NULL;
}

void *receiver(void *index) {
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];
    int ready;
    struct pollfd fds;
    fds.fd = bot->_data->socketfd;
    fds.events = POLLIN;

    pipe_producer_t *p = pipe_producer_new(pipes[i]);
    while (1) {
        ready = poll(&fds, 1, -1);
        if(ready < 0)
            perror("event listener");
        // read packet
        // send signal to corresponding thread
        if (fds.revents & POLLIN ) {
            // socket is ready to be read from
            void *data = protocol_decode(bot);
            pipe_push(p, &data, 1);
        }
    }

    pipe_producer_free(p);

    return NULL;
}

void *callbacker(void *index) {
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];

    int bytes_read;
    protocol_dummy_t *data;
    pipe_consumer_t *p = pipe_consumer_new(pipes[i]);
    while((bytes_read = pipe_pop(p, &data, 1))) {
        if (data == NULL) continue;

        int state = bot->_data->current_state;
        int pid = data->packet_id;

        function *func = &(bot->_data->callbacks[state][pid]);
        while(func->next) {
            (func->f)(bot, data);
            func = func->next;
        }
        free_packet(data);
    }

    pipe_consumer_free(p);

    return NULL;
}
