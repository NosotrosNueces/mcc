#include <stdio.h>
#include <time.h>
#include "bot.h"
#include "client.h"
#include "api.h"

#define SERVER_NAME "localhost"
#define DEFAULT_SERVER_PORT 25565

void sample_main(void *vbot) {
    bot_t *bot = (bot_t *)vbot;
    register_defaults(bot);

    while(1) {
        msleep(500);
    }
}

int main() {
    bot_t *bot = init_bot("an_guy", *sample_main);
    login(bot, SERVER_NAME, DEFAULT_SERVER_PORT);
    client_run(bot, 1);
    free_bot(bot);
    return 0;
}
