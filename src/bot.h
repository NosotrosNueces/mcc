#ifndef BOT_H
#define BOT_H

#include <stdint.h>
#include <stdlib.h>

typedef enum {HANDSHAKE, LOGIN, STATUS, PLAY, NUM_STATES} state;

typedef struct bot bot_t;
typedef struct _function function;

struct bot {
    int socketfd;
    size_t packet_threshold;
    char *buf;
    char *name;
    state current_state;
    /* registered callbacks */
    void (*bot_main)(void *);
    function **callbacks;
};

struct _function {
    void (*f)(bot_t *, void *);
    struct _function *next;
};

extern struct bot context;

/** \brief Initialize a bot with a name and main function
 *
 *  This function creates a bot object. The bot_main argument is used for
 *  threading purposes. If bot_main exits, the bot will cease to exist.
 *  bot_main will likely contain a large switch on some sort of global state.
 */
bot_t *init_bot(char *name, void (*bot_main)(void *));

/** \brief Free a bot
 *
 *  Call this function on a bot to completely dispose of it. This cleans up
 *  the socket, and frees the internal buffer, name, and callback table.
 */
void free_bot(bot_t *);

/** \brief Register to recieve callbacks for a specific packet type
 *
 *  Registering for callbacks allows a bot to run a number of functions
 *  upon recieving a certain packet from the server. Functions are stored in a
 *  linked list inside the bot struct and called one by one each time a packet
 *  is recieved.
 *  Note: Currently there is no way to "un-register" a callback.
 */
void register_event(bot_t *bot, uint32_t state, uint32_t packet_id, void (*f)(bot_t *, void *));

/** \brief Open a socket to the specified server
 *
 *  Open a socket connection to a specific server for a particular bot.
 */
int join_server(bot_t *bot, char* server_host, int server_port);

/** \brief Sends a string across the network using a bot's socket
 *
 *  Sends a raw string across the network using a bot's socket. This function
 *  should not be used unless you know what you're doing.
 */
int send_str(bot_t *bot, char *str);

/** \brief Sends bytes across the network using a bot's socket
 *
 *  Sends raw bytes across the network using a bot's socket. This function
 *  should not be used unless you know what you're doing.
 */
int send_raw(bot_t *bot, void *data, size_t len);

/** \brief Recieve bytes from the network using a bot's socket
 *
 *  Recieve raw bytes from the network using a bot's socket. This function
 *  should not be used unless you know what you're doing.
 */
int receive_raw(bot_t *your_bot, void *data, size_t len);

/** \brief Parse data from the server in a protocol intelligent manner
 *
 *  The function fills bot->buf with packet data and returns the packet id.
 *  Parse data from the server, taking into account packet id, packet length
 *  and the packet data sent. This function reads `packet length` bytes from
 *  the server. In the case that `packet length` is larger than the maximum
 *  packet size, this function reads the full packet, but discards the data.
 */
int receive_packet(bot_t *bot);

#endif /* BOT_H */
