#include <stdio.h>
#include <time.h>
#include "bot.h"
#include "client.h"
#include "api.h"

#define SERVER_NAME "localhost"
#define DEFAULT_SERVER_PORT 25565

void sample_main(void *vbot) {
    bot_t *bot = (bot_t *)vbot;
    struct timespec req = {0, 500000000}; //tv_sec, tv_nsec
    struct timespec rem;
   
    register_defaults(bot);

    while(1) {
        // Broken: time intervals are irregular.
        nanosleep(&req, &rem);
    }
}

int main() {
    bot_t *bot = init_bot("an_guy", *sample_main);
    login(bot, SERVER_NAME, DEFAULT_SERVER_PORT);
    client_run(bot, 1);
    free_bot(bot);
    return 0;
}
