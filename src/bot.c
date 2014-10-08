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

// Initializes the bot with defaults given a name and a main function.
bot_t *init_bot(char *name, void (*bot_main)(void *)){
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
    return bot;
}

void free_bot(bot_t *bot){
    free(bot->name);
    // free all handshake callback structs
    // unrolled outer loop just cuz
    int i;
    for(i = 0; i < HANDSHAKE_PACKETS; i++){
        function *func = &bot->_data->callbacks[HANDSHAKE][i];
        free_list(func);
    }
    for(i = 0; i < LOGIN_PACKETS; i++){
        function *func = &bot->_data->callbacks[LOGIN][i];
        free_list(func);
    }
    for(i = 0; i < PLAY_PACKETS; i++){
        function *func = &bot->_data->callbacks[PLAY][i];
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
    function *parent = &bot->_data->callbacks[state][packet_id];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

// initializes a bot structure with a socket. The socket is bound to the local address on
// some port and is connected to the server specified by the server_host and server_port
// the socket descriptor is returned by the function. If -1 is returned, then an error
// occured, and a message will have been printed out.

int join_server(bot_t *bot, char* server_host, int port_number){
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

int disconnect(bot_t *bot){
    return close(bot->_data->socketfd);
}

int send_str(bot_t *bot, char *str){
    //TODO: send is not guaranteed to send all the data. Need to make loop
    size_t len = strlen(str) + 1; // to include null character
    return send(bot->_data->socketfd, str, len, 0);
}

int send_raw(bot_t *bot, void *data, size_t len){
    return send(bot->_data->socketfd, data, len, 0);
}

int receive_raw(bot_t *bot, void *data, size_t len){
    return recv(bot->_data->socketfd, data, len, 0);
}

int receive_packet(bot_t *bot) {
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
