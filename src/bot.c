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
#include <uv.h>
#include "bot.h"
#include "protocol.h"
#include "marshal.h"
#include "types.h"

// Initializes a bot structure with a socket. The socket is bound to the
// local address on some port and is connected to the server specified by the
// server_host and server_port the socket descriptor is returned by the
// function. If -1 is returned, then an error occured, and a message will
// have been printed out.

uv_loop_t *loop;

void on_connect(uv_connect_t *req, int status) {
}

int join_server(struct bot_agent *bot, char* server_host, int port_number)
{
	bot->socket = malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, bot->socket);

	uv_connect_t *connect = malloc(sizeof(uv_connect_t));

	struct sockaddr_in dest;
	uv_ip4_addr(server_host, 25565, &dest);

	uv_tcp_connect(connect, bot->socket, (const struct sockaddr*)&dest, on_connect);
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
