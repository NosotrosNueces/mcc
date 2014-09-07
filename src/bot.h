#ifndef BOT_H
#define BOT_H

#include <stdint.h>
#include <stdlib.h>

typedef struct bot {
    int socketfd;
    size_t packet_threshold;
} bot_t;

extern struct bot context;

int join_server(bot_t *, char *, char *, char *);
int send_str(bot_t *, char *);
int send_raw(bot_t *, void *, size_t);

#endif /* BOT_H */
