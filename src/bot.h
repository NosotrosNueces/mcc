#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef enum {HANDSHAKE, LOGIN, STATUS, PLAY, NUM_STATES} state;

typedef struct bot bot_t;
typedef struct _function function;
typedef struct _timed_function timed_function;
typedef struct _bot_internal bot_internal;

struct bot {
    /*
     * Join game data
     */
    uint32_t eid;
    // 0: survival, 1: creative, 2: adventure. Bit 3 (0x8) is the hardcore flag
    uint8_t gamemode;
    // -1: nether, 0: overworld, 1: end
    int8_t dimension;
    // 0 thru 3 for Peaceful, Easy, Normal, Hard
    uint8_t difficulty;
    uint8_t max_players;
    // String  default, flat, largeBiomes, amplified, default_1_1
    char * level_type;
    bool reduced_debug_info;

    double x;
    double y;
    double z;
    float yaw;
    float pitch;
    int8_t flags;

    uint8_t animation;

    int32_t health;
    int32_t food;
    float saturation;

    // dummy entry since slot_t is currently a void*
    void* item;

    char *name;

    void* state;

    pthread_mutex_t bot_mutex;

    bot_internal *_data; // Internal use only for networking and threading.
};

struct _bot_internal {
    int socketfd;
    size_t packet_threshold;
    char *buf;
    state current_state;
    void (*bot_main)(void *);
    function **callbacks;
    timed_function **timers;
};

struct _function {
    void *f;
    struct _function *next;
};

struct _timed_function {
    void (*f)(bot_t *, void *);
    struct _timed_function *next;
    struct _timed_function *prev;
    struct timeval *last_time_called;
    // If interval = {0, 0}, never call the function.
    struct timeval *interval;
    // -1 for always repeat, 1 for exec once.
    int repeat_count;
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
void register_event(bot_t *bot, uint32_t state, uint32_t packet_id,
                    void (*f)(bot_t *, void *));

/** \brief Register to set an interval or timeout function.
 *
 *  Registering an interval or timeout works in the same manner as in
 *  Javascript; this function is designed to emulate setTimeout and
 *  setInterval.
 *  Note: Waits delay before the first call.
 */
timed_function *register_timer(bot_t *bot, struct timeval delay,
                               int count, void (*f)(bot_t *, void *));

/** \brief Unregister a set interval or timeout function.
 *
 *  Unregistering an interval or timeout works similarly to Javascript. An
 *  "id" is given, which is really just a node in a doubly-linked list that
 *  gets freed.
 */
void unregister_timer(bot_t *bot, timed_function *id);

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
