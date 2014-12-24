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
#include <sys/time.h>

#define MAX_BOTS 256
#define MAILBOX_CAPACITY 1 << 10
#define POLL_TIMEOUT 10000
#define SLEEP_INTERVAL_US 50000 // tick(us), 50000 = 0.05s = 20Hz

//uint32_t num_bots;
//bot_t *bot_net[MAX_BOTS];

uint32_t num_bots;
bot_t **bot_list;
pthread_t *bot_threads;
pthread_t *receivers;
pthread_t *callbackers;
pthread_t *schedulers;
pthread_t *executors;
pipe_t **pipes;
pipe_t **timer_pipes;

void *receiver(void *index);
void *callbacker(void *index);
void *scheduler(void *index);
void *executor(void *index);
void *bot_thread(void *bot);


/* Subtract the 'struct timeval' values X and Y,
 * storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 * See: http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
 */

int timeval_subtract (struct timeval *result,
                      struct timeval *x,
                      struct timeval *y)
{
    long y_sec = y->tv_sec;
    long y_usec = y->tv_usec;
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y_usec) {
        int nsec = (y_usec - x->tv_usec) / 1000000 + 1;
        y_usec -= 1000000 * nsec;
        y_sec += nsec;
    }
    if (x->tv_usec - y_usec > 1000000) {
        int nsec = (x->tv_usec - y_usec) / 1000000;
        y_usec += 1000000 * nsec;
        y_sec -= nsec;
    }

    /* Compute the time remaining to wait.
     *      tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y_sec;
    result->tv_usec = x->tv_usec - y_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y_sec;
}

void client_run(bot_t **bots, uint32_t num)
{
    // create 1 thread for receiving packets, and 1 for each bot
    uint64_t i;
    num_bots = num;
    bot_list = bots;

    // create all bot pipes
    pipes = calloc(num_bots, sizeof (pipe_t*));
    timer_pipes = calloc(num_bots, sizeof (pipe_t*));
    for (i = 0; i < num; i++) {
        pipes[i] = pipe_new(sizeof(void *), 0);
        timer_pipes[i] = pipe_new(sizeof(void *), 0);
    }

    // create and start listener threads
    receivers = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(receivers + i, NULL, receiver, (void *)i);

    // create and start callback threads
    callbackers = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(callbackers + i, NULL, callbacker, (void *)i);

    // For timers, create and start scheduler threads.
    schedulers = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(schedulers + i, NULL, scheduler, (void *)i);

    // For timers, create and start executor threads.
    executors = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(executors + i, NULL, executor, (void *)i);

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
        pthread_join(schedulers[i], NULL);
        pthread_join(executors[i], NULL);
    }

    for (i = 0; i < num; i++) {
        pipe_free(pipes[i]);
        pipe_free(timer_pipes[i]);
    }

    free(bot_threads);
    free(receivers);
    free(callbackers);
    free(executors);
    free(schedulers);
}

void *bot_thread(void *bot)
{
    ((bot_t *)bot)->_data->bot_main(bot);
    return NULL;
}

void *receiver(void *index)
{
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

void *callbacker(void *index)
{
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

void *scheduler(void *index)
{
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];
    pipe_producer_t *p = pipe_producer_new(timer_pipes[i]);
    struct timespec rqtp = {(SLEEP_INTERVAL_US)/1000000,
               (1000*SLEEP_INTERVAL_US)%1000000000
    };
    struct timeval tp, dt;
    struct timespec dummy;

    while (1) {
        nanosleep(&rqtp, &dummy);
        // Check for all functions that need to be called.
        timed_function *func = *(bot->_data->timers);
        while (func->f &&
               func->interval->tv_sec >= 0 && func->interval->tv_usec > 0) {
            gettimeofday(&tp, NULL);
            timeval_subtract(&dt, &tp, func->last_time_called);
            timeval_subtract(&dt, &dt, func->interval);
            // dt = current_time - last_call_time - interval_time
            if (dt.tv_sec >= 0 && dt.tv_usec > 0) {
                pipe_push(p, &func, 1);
            }
            func = func->next;
        }
    }

    pipe_producer_free(p);

    return NULL;
}

void *executor(void *index)
{
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];

    int bytes_read;
    timed_function *timer;
    pipe_consumer_t *p = pipe_consumer_new(timer_pipes[i]);
    // Execute whatever is in the pipe.
    while((bytes_read = pipe_pop(p, &timer, 1))) {
        if (timer == NULL) continue;
        gettimeofday(timer->last_time_called, NULL);
        if (timer->f) {
            if (!timer->repeat_count) {
                unregister_timer(bot, timer);
            } else {
                timer->f(bot, NULL);
                if (timer->repeat_count > 0)
                    --(timer->repeat_count);
            }
        }
    }

    pipe_consumer_free(p);

    return NULL;
}
