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
#include "types.h"

#define PROTOCOL_VERSION 110

/* Populates the bot struct with default values */
void init_bot(struct bot_agent *bot, char *name) {
    bot->name = name;
    bot->packet_threshold = DEFAULT_THRESHOLD;
    bot->current_state = LOGIN;
    uv_loop_init(&bot->loop);
	uv_tcp_init(&bot->loop, &bot->socket);
    bot->socket.data = bot;
}

void on_connect(uv_connect_t *connect, int status) {
    struct bot_agent *bot = connect->data;
    send_handshaking_serverbound_handshake(bot, PROTOCOL_VERSION, "localhost", 25565, 2);
    send_login_serverbound_login_start(bot, bot->name);
    free(connect);
}

void join_server(struct bot_agent *bot, char* server_host, int port_number)
{
	uv_connect_t *connect = malloc(sizeof(uv_connect_t));
    connect->data = bot;
	struct sockaddr_in dest;
	uv_ip4_addr(server_host, port_number, &dest);
    
    /* synchronous */
	uv_tcp_connect(connect, &bot->socket, (const struct sockaddr*)&dest, on_connect);
}

void disconnect(struct bot_agent *bot)
{
    uv_close((uv_handle_t *)&bot->socket, NULL);
}

void on_write(uv_write_t *req, int status) {
    free(req->data);
    free(req);
}

int send_string(struct bot_agent *bot, char *str)
{
    size_t len = strlen(str) + 1; // to include null character
    uv_write_t *req = malloc(sizeof(uv_write_t));
    uv_buf_t buf = uv_buf_init(malloc(len), len);
    memcpy(buf.base, str, len);
    return uv_write(req, (uv_stream_t *)&bot->socket, &buf, 1, NULL);
}

int send_raw(struct bot_agent *bot, void *data, size_t len)
{
    uv_write_t *req = malloc(sizeof(uv_write_t));
    uv_buf_t buf = uv_buf_init(malloc(len), len);
    req->data = buf.base;
    memcpy(buf.base, data, len);
    uint32_t bytes_written = uv_write(req, (uv_stream_t *)&bot->socket, &buf, 1, on_write);
    return bytes_written;
}
