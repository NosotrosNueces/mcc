#ifndef BOT_H
#define BOT_H

#include <stdint.h>
#include <stdlib.h>

typedef enum {HANDSHAKE, LOGIN, STATUS, PLAY, NUM_STATES} state;

typedef struct _function {
    void (*f)(void *);
    struct _function *next;
} function;

typedef struct bot {
    int socketfd;
    size_t packet_threshold;
    char *buf;
    char *name;
    state current_state;
    /* registered callbacks */
    function ***callbacks; // triple indirection hooray!
} bot_t;

extern struct bot context;

bot_t *init_bot(char *);
void free_bot(bot_t *);
void register_event(bot_t *, uint32_t, uint32_t, void (*)(void *));
int join_server(bot_t *, char *, char *, char *);
int send_str(bot_t *, char *);
int send_raw(bot_t *, void *, size_t);
int receive_packet(bot_t *bot);

#endif /* BOT_H */
