#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
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

// initializes the bot with defaults given a name and a main function.
bot_t *init_bot(char *name, void (*bot_main)(void *)){
    // set the bot name
    bot_t *bot = calloc(1, sizeof(bot_t));
    bot->packet_threshold = DEFAULT_THRESHOLD;
    bot->buf = calloc(1, DEFAULT_THRESHOLD);
    size_t len = strlen(name);
    bot->name = calloc(len + 1, sizeof(char));
    strncpy(bot->name, name, len + 1);
    bot->bot_main = bot_main;
    // initialize the callback data structure
    bot->callbacks = calloc(NUM_STATES, sizeof(function *));
    bot->callbacks[HANDSHAKE] = calloc(HANDSHAKE_PACKETS, sizeof(function));
    bot->callbacks[LOGIN] = calloc(LOGIN_PACKETS, sizeof(function));
    bot->callbacks[PLAY] = calloc(PLAY_PACKETS, sizeof(function));
    return bot;
}

void free_bot(bot_t *bot){
    free(bot->name);
    // free all handshake callback structs
    // unrolled outer loop just cuz
    int i;
    for(i = 0; i < HANDSHAKE_PACKETS; i++){
        function *func = &bot->callbacks[HANDSHAKE][i];
        free_list(func);
    }
    for(i = 0; i < LOGIN_PACKETS; i++){
        function *func = &bot->callbacks[LOGIN][i];
        free_list(func);
    }
    for(i = 0; i < PLAY_PACKETS; i++){
        function *func = &bot->callbacks[PLAY][i];
        free_list(func);
    }
    free(bot);
}

void free_list(function *list){
    if(list)
        free_list(list->next);
    free(list);
}


void register_event(bot_t *bot, uint32_t state, uint32_t packet_id, 
        void (*f)(bot_t *, void *)){
    function *parent = &bot->callbacks[state][packet_id];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    child->f = f;
    parent->next = child;
}

// initializes a bot structure with a socket. The socket is bound to the local address on
// some port and is connected to the server specified by the server_host and server_port
// the socket descriptor is returned by the function. If -1 is returned, then an error
// occured, and a message will have been printed out.

int join_server(bot_t *your_bot, char* server_host, char* server_port){
    struct addrinfo hints, *res;
    int sockfd;
    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(server_host, server_port, &hints, &res);

    // make a socket and connect
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sockfd, res->ai_addr, res->ai_addrlen);

    your_bot->socketfd = sockfd;
    return sockfd;
}

int disconnect(bot_t *your_bot){
    return close(your_bot->socketfd);
}

int send_str(bot_t *your_bot, char *str){
    //TODO: send is not guaranteed to send all the data. Need to make loop
    size_t len = strlen(str) + 1; // to include null character
    return send(your_bot->socketfd, str, len, 0);
}

int send_raw(bot_t *your_bot, void *data, size_t len){
    return send(your_bot->socketfd, data, len, 0);
}

int receive_raw(bot_t *your_bot, void *data, size_t len){
    return recv(your_bot->socketfd, data, len, 0);
}

int receive_packet(bot_t *bot) {
    vint32_t packet_size;
    uint32_t received;
    uint32_t ret;
    uint32_t len;
    uint32_t i;

    memset(bot->buf, 0, bot->packet_threshold);
    for (i = 0; i < 5; i++) {
        ret = receive_raw(bot, bot->buf + i, 1);
        if (ret <= 0)
            return -1;
        if (!expect_more(bot->buf[i]))
            break;
    }

    len = varint32(bot->buf, &packet_size);
    if (packet_size == 0)
        return -2;

    assert(i != len);

    packet_size += len;
    received = i + 1;
    if (packet_size <= bot->packet_threshold) {
        while (received < packet_size) {
            ret = receive_raw(bot, bot->buf + received, packet_size - received);
            if (ret <= 0)
                return -1;
            received += ret;
        }

        ret = peek_packet(bot, bot->buf);
        return ret;
    } else {
        // read in a huge buffer, but throw it away
        while (received < packet_size) {
            ret = receive_raw(bot, bot->buf, bot->packet_threshold);
            if (ret <= 0)
                return -1;
            received += ret;
        }
        return -2;
    }
}
