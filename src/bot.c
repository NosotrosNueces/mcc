#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "bot.h"
#include "protocol.h"
#include "marshal.h"

void free_list(function *);

// Initializes the bot with defaults given a name and a main function.
bot_t *init_bot(char *name, void (*bot_main)(void *))
{
    // Set the bot name
    bot_t *bot = calloc(1, sizeof(bot_t));
    bot->eid = -1;
    size_t len = strlen(name);
    bot->name = calloc(len + 1, sizeof(char));
    strncpy(bot->name, name, len + 1);
    bot->_data = calloc(1, sizeof(bot_internal));
    bot->_data->packet_threshold = DEFAULT_THRESHOLD;
    bot->_data->buf = calloc(1, DEFAULT_THRESHOLD);
    bot->_data->current_state = LOGIN;
    bot->_data->bot_main = bot_main;
    // Initialize the callback data structure
    bot->_data->callbacks = calloc(NUM_STATES, sizeof(function *));
    bot->_data->callbacks[HANDSHAKE] = calloc(HANDSHAKE_PACKETS, sizeof(function));
    bot->_data->callbacks[LOGIN] = calloc(LOGIN_PACKETS, sizeof(function));
    bot->_data->callbacks[PLAY] = calloc(PLAY_PACKETS, sizeof(function));
    bot->_data->timers = calloc(1, sizeof(timed_function *));
    // Dummy tail for the timer list.
    *(bot->_data->timers) = calloc(1, sizeof(timed_function));

    // initialize pthread_mutex
    pthread_mutex_init(&bot->bot_mutex, NULL);
    return bot;
}

void free_bot(bot_t *bot)
{
    free(bot->name);

    free(bot->_data->buf);

    // Free all handshake callback structs.
    for(int i = 0; i < HANDSHAKE_PACKETS; i++)
        free_list(bot->_data->callbacks[HANDSHAKE][i].next);
    for(int i = 0; i < LOGIN_PACKETS; i++)
        free_list(bot->_data->callbacks[LOGIN][i].next);
    for(int i = 0; i < PLAY_PACKETS; i++)
        free_list(bot->_data->callbacks[PLAY][i].next);
    free(bot->_data->callbacks[HANDSHAKE]);
    free(bot->_data->callbacks[LOGIN]);
    free(bot->_data->callbacks[PLAY]);
    free(bot->_data->callbacks);

    // Free all timers in the linked list.
    timed_function *node = *bot->_data->timers;
    timed_function *prev_node = NULL;
    // Use free_list?
    while(node) {
        prev_node = node;
        node = node->next;
        free(prev_node->last_time_called);
        free(prev_node->interval);
        free(prev_node);
    }
    free(bot->_data->timers);

    free(bot->_data);

    pthread_mutex_destroy(&bot->bot_mutex);

    free(bot);
}

void free_list(function *list)
{
    if (list) {
        free_list(list->next);
        free(list);
    }
}

function *register_event(bot_t *bot, uint32_t state, uint32_t packet_id,
                         void (*f)(bot_t *, void *))
{
    // Find the last node.
    function *new_node = &bot->_data->callbacks[state][packet_id];
    function *prev_node = NULL;
    while(new_node->next) {
        prev_node = new_node;
        new_node = new_node->next;
    }
    // Store the new callback in the last node.
    new_node->f = f;
    new_node->next = calloc(1, sizeof(function));
    new_node->prev = prev_node;

    // Use the returned node to unregister the callback later.
    return new_node;
}

void unregister_event(bot_t *bot, function *callback)
{
    if (callback->prev) {
        callback->next->prev = callback->prev;
        callback->prev->next = callback->next;
        free(callback);
    } else { // First element is a special case.
        // Overwrite next element into first element.
        callback->f = callback->next->f;
        callback->next = callback->next->next;
        callback->prev = callback->next->prev;
        // Delete next element.
        unregister_event(bot, callback->next);
    }
}

timed_function *register_timer(bot_t *bot, struct timeval delay,
                               int count, void (*f)(bot_t *, void *))
{
    timed_function **old_head = bot->_data->timers;

    // Create a new node representing the timer to add.
    timed_function *new_node = calloc(1, sizeof(timed_function));
    new_node->f = f;
    new_node->next = *old_head;
    new_node->prev = NULL;
    new_node->last_time_called = calloc(1, sizeof(struct timeval));
    gettimeofday(new_node->last_time_called, NULL);
    new_node->interval = calloc(1, sizeof(struct timeval));
    memcpy(new_node->interval, &delay, sizeof(struct timeval));
    new_node->repeat_count = count;

    // Always insert to the head.
    (*old_head)->prev = new_node;
    *(bot->_data->timers) = new_node;

    // Use the returned node to unregister the timer later.
    return new_node;
}

void unregister_timer(bot_t *bot, timed_function *timer)
{
    if (timer->prev) {
        timer->next->prev = timer->prev;
        timer->prev->next = timer->next;
    } else { // First element is a special case.
        timer->next->prev = NULL;
        *(bot->_data->timers) = timer->next;
    }
    free(timer->last_time_called);
    free(timer->interval);
    free(timer);
}

// Initializes a bot structure with a socket. The socket is bound to the
// local address on some port and is connected to the server specified by the
// server_host and server_port the socket descriptor is returned by the
// function. If -1 is returned, then an error occured, and a message will
// have been printed out.

int join_server(bot_t *bot, char* server_host, int port_number)
{
    struct addrinfo hints, *res;
    int sockfd;
    char server_port[8];
    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    sprintf(server_port, "%d", port_number);
    getaddrinfo(server_host, server_port, &hints, &res);

    // make a socket and connect
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sockfd, res->ai_addr, res->ai_addrlen);

    bot->_data->socketfd = sockfd;
    return sockfd;
}

int disconnect(bot_t *bot)
{
    return close(bot->_data->socketfd);
}

int send_str(bot_t *bot, char *str)
{
    //TODO: send is not guaranteed to send all the data. Need to make loop
    size_t len = strlen(str) + 1; // to include null character
    return send(bot->_data->socketfd, str, len, 0);
}

int send_raw(bot_t *bot, void *data, size_t len)
{
    return send(bot->_data->socketfd, data, len, 0);
}

int receive_raw(bot_t *bot, void *data, size_t len)
{
    return recv(bot->_data->socketfd, data, len, 0);
}

int receive_packet(bot_t *bot)
{
    vint32_t packet_size;
    uint32_t received;
    uint32_t ret;
    uint32_t len;
    uint32_t i;

    memset(bot->_data->buf, 0, bot->_data->packet_threshold);
    for (i = 0; i < 5; i++) {
        ret = receive_raw(bot, bot->_data->buf + i, 1);
        if (ret <= 0)
            return -1;
        if (!expect_more(bot->_data->buf[i]))
            break;
    }

    len = varint32(bot->_data->buf, &packet_size);
    if (packet_size == 0)
        return -2;

    assert(i != len);

    packet_size += len;
    received = i + 1;
    if (packet_size <= bot->_data->packet_threshold) {
        while (received < packet_size) {
            ret = receive_raw(bot, bot->_data->buf + received, packet_size - received);
            if (ret <= 0)
                return -1;
            received += ret;
        }

        ret = peek_packet(bot, bot->_data->buf);
        return ret;
    } else {
        // read in a huge buffer, packet_threshold at a time
        while (received < packet_size - bot->_data->packet_threshold) {
            ret = receive_raw(bot, bot->_data->buf, bot->_data->packet_threshold);
            if (ret <= 0)
                return -1;
            received += ret;
        }
        // read the last portion of the packet
        while (received < packet_size) {
            ret = receive_raw(bot, bot->_data->buf, packet_size - received);
            if (ret <= 0)
                return -1;
            received += ret;
        }
        return -2;
    }
}
