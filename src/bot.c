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

void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}

// initializes the bot with a name, and default callbacks
bot_t *init_bot(char *name){
    // set the bot name
    bot_t *bot = calloc(1, sizeof(bot_t));
    size_t len = strlen(name);
    bot -> name = calloc(len + 1, sizeof(char));
    strncpy(bot -> name, name, len + 1);
    // initialize the callback data structure
    bot -> callbacks = calloc(NUM_STATES, sizeof(function **));
    bot -> callbacks[HANDSHAKE] = calloc(HANDSHAKE_PACKETS, sizeof(function *));
    bot -> callbacks[LOGIN] = calloc(LOGIN_PACKETS, sizeof(function *));
    bot -> callbacks[PLAY] = calloc(PLAY_PACKETS, sizeof(function *));
    return bot;
}

void free_bot(bot_t *bot){
    free(bot -> name);
    // free all handshake callback structs
    // unrolled outer loop just cuz
    int i;
    for(i = 0; i < HANDSHAKE_PACKETS; i++){
        function *func = bot -> callbacks[HANDSHAKE][i];
        free_list(func);
    }
    for(i = 0; i < LOGIN_PACKETS; i++){
        function *func = bot -> callbacks[LOGIN][i];
        free_list(func);
    }
    for(i = 0; i < PLAY_PACKETS; i++){
        function *func = bot -> callbacks[PLAY][i];
        free_list(func);
    }
    free(bot);
}

void free_list(function *list){
    if(list)
        free_list(list -> next);
    free(list);
}


void register_event(bot_t *bot, uint32_t state, uint32_t packet_id, 
        void (*f)(void *)){
    function *current = bot -> callbacks[state][packet_id];
    while(current)
        current = current -> next;
    current = calloc(1, sizeof(function));
    current -> f = f;
}

// initializes a bot structure with a socket. The socket is bound to the local address on
// some port and is connected to the server specified by the server_host and server_port
// the socket descriptor is returned by the function. If -1 is returned, then an error
// occured, and a message will have been printed out.

int join_server(bot_t *your_bot, char *local_port, char* server_host,
                char* server_port){
    int status;
    struct addrinfo hints, *res;
    int sockfd;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(status = getaddrinfo(NULL, local_port, &hints, &res)){
        fprintf(stderr, "Your computer is literally haunted: %s\n",
                gai_strerror(status));
        return -1;
    }
    if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
        fprintf(stderr, "Could not create socket for unknown reason.\n");
        return -1;
    }
    freeaddrinfo(res);
    // socket bound to local address/port

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(status = getaddrinfo(server_host, server_port, &hints, &res)){
        fprintf(stderr, "Server could not be resolved: %s\n",
                gai_strerror(status));
        return -1;
    }
    connect(sockfd, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    // connected to server
    your_bot -> socketfd = sockfd;
    return sockfd;
}

int disconnect(bot_t *your_bot){
    close(your_bot -> socketfd);
}

int send_str(bot_t *your_bot, char *str){
    //TODO: send is not guaranteed to send all the data. Need to make loop
    size_t len = strlen(str) + 1; // to include null character
    return send(your_bot -> socketfd, str, len, 0);
}

int send_raw(bot_t *your_bot, void *data, size_t len){
    return send(your_bot -> socketfd, data, len, 0);
}

int receive_raw(bot_t *your_bot, void *data, size_t len){
    return recv(your_bot -> socketfd, data, len, 0);
}

// int main(int argc, char **argv){
//     //if(argc < 4){
//     //    fprintf(stderr, "Specify local port, remote hostname/ip, and remote port\n");
//     //    exit(1);
//     //}
//     //bot_t test_bot;
//     //if(join_server(&test_bot, argv[1], argv[2], argv[3]) == -1){
//     //    return -1;
//     //}
//     char handshake[20] = {0x0F, 0x00, 47, 0x09, 'l', 'o', 'c', 'a', 'l', 'h', 'o', 's', 't', 0x63, 0xDD, 0x02};
//     char login_start[20] = {0x08, 0x00, 6, 'a', 'n', '_', 'g', 'u', 'y'};
//     char test[2] = {0x80, 0x02};
//     uint64_t val;
//     // uvarint64(test, &val);
//     printf("varint: %d\n");
//     char buf[100] = {0};
//     // int len = uvarint64_encode(256, buf, 1);
//     printf("len: %d, %hhx %hhx\n", len, buf[0], buf[1]);
//     //int bytes_sent = send_raw(&test_bot, handshake, 16);
//     //printf("sent %d bytes\n", bytes_sent);
//     //bytes_sent = send_raw(&test_bot, login_start, 9);
//     //int bytes_received = receive_raw(&test_bot, buf, 100);
//     //printf("received %d bytes\n", bytes_received);
//     //printf("%hhX %hhX %hhX %hhX\n", buf[0], buf[1], buf[2], buf[3]);
//     //disconnect(&test_bot);
//     //char var[2] = {0xAC, 0x02};
//     //printf("%d\n", varint64(var));
// }

int main() {
    char* buf = calloc(sizeof(char), DEFAULT_THRESHOLD);
    vint32_t packet_size;
    uint32_t received;
    uint32_t len;
    int ret, i;

    bot_t bot;
    bot.packet_threshold = DEFAULT_THRESHOLD;

    join_server(&bot, "25567", "10.10.2.16", "25565");

    send_handshaking_serverbound_handshake(&bot, 47, "localhost", 25565, 2);
    send_login_serverbound_login(&bot, "an_guy");

    bot.packet_threshold = 256;

    struct timeval tv = {2, 0};
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(bot.socketfd, &readfds);

    while (1) {
        select(bot.socketfd + 1, &readfds, NULL, NULL, &tv);

        memset(buf, 0, DEFAULT_THRESHOLD);
        for (i = 0; i < 5; i++) {
            ret = receive_raw(&bot, buf + i, 1);
            if (ret <= 0) return 1;
            if ((buf[i] & 0x80) == 0) break;
        }

        len = varint32(buf, &packet_size);
        if (packet_size == 0) continue;

        packet_size += len;
        received = i + 1;
        if (packet_size <= DEFAULT_THRESHOLD) {
            while (received < packet_size) {
                ret = receive_raw(&bot, buf + received, packet_size - received);
                if (ret <= 0) return 1;
                received += ret;
            }

            ret = peek_packet(&bot, buf);
            if (ret > 0x49) {
                return 1;
            }
            if (ret == 0x00) {
                play_clientbound_keepalive_t *p;
                p = recv_play_clientbound_keepalive(&bot, buf);

                printf("ping: %x\n", p->keepalive_id);
                send_play_serverbound_keepalive(&bot, p->keepalive_id);
                free_packet(p);
            }
            printf("%x\n", ret);
            hexDump("buffer", buf, packet_size);
        } else {
            printf("we've got a whopper %d\n", packet_size);
            while (received < packet_size) {
                ret = receive_raw(&bot, buf, DEFAULT_THRESHOLD);
                if (ret <= 0) return 1;
                received += ret;
            }
        }
    }

    free(buf);
}
