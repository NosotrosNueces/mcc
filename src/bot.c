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

/* Populates the bot struct with default values */
void init_bot(struct bot_agent *bot, char *name) {
    bot->name = name;
    bot->packet_threshold = DEFAULT_THRESHOLD;
    bot->current_state = LOGIN;
}

void join_server(struct bot_agent *bot, char* server_host, int port_number)
{
	bot->socket = malloc(sizeof(uv_tcp_t));
	uv_tcp_init(bot->loop, bot->socket);

	uv_connect_t *connect = malloc(sizeof(uv_connect_t));

	struct sockaddr_in dest;
	uv_ip4_addr(server_host, 25565, &dest);
    
    /* synchronous */
	uv_tcp_connect(connect, bot->socket, (const struct sockaddr*)&dest, NULL);
}

void disconnect(struct bot_agent *bot)
{
    uv_close((uv_handle_t *)bot->socket, NULL);
}

int send_string(struct bot_agent *bot, char *str)
{
    //TODO: send is not guaranteed to send all the data. Need to make loop
    size_t len = strlen(str) + 1; // to include null character
    uv_buf_t buf;
    buf.base = str;
    buf.len = len;
    return uv_write(NULL, (uv_stream_t *)bot->socket, &buf, 1, NULL);
}

int send_raw(struct bot_agent *bot, void *data, size_t len)
{
    uv_buf_t buf;
    buf.base = data;
    buf.len = len;
    return uv_write(NULL, (uv_stream_t *)bot->socket, &buf, 1, NULL);
}
