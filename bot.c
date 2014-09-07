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


// initializes a bot structure with a socket. The socket is bound to the local address on
// some port and is connected to the server specified by the server_host and server_port
// the socket descriptor is returned by the function. If -1 is returned, then an error
// occured, and a message will have been printed out.

int join_server(bot_t *your_bot, char *local_port, char* server_host, char* server_port){
    int status;
    struct addrinfo hints, *res;
    int sockfd;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if(status = getaddrinfo(NULL, local_port, &hints, &res)){
        fprintf(stderr, "Your computer is literally haunted: %s\n", gai_strerror(status));
        return -1;
    }
    if((sockfd = socket(res -> ai_family, res -> ai_socktype, res -> ai_protocol)) == -1){
        fprintf(stderr, "Your computer is literally haunted. Could not create socket. Why...?!?!?!\n");
        return -1;
    }
    if((status = bind(sockfd, res -> ai_addr,res -> ai_addrlen)) == -1){
        fprintf(stderr, "Your computer is literally haunted. Could not bind socket to your OWN ADDRESS?!!?!?."
                "Acquire new Internet plz.\n");
        return -1;
    }
    freeaddrinfo(res);
    // socket bound to local address/port

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(status = getaddrinfo(server_host, server_port, &hints, &res)){
        fprintf(stderr, "Server could not be resolved: %s\n", gai_strerror(status));
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
