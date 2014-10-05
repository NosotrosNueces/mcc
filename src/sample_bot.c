#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "bot.h"
#include "client.h"
#include "protocol.h"

void sample_main(void* vbot) {
    bot_t *bot = (bot_t *)vbot;
    struct timespec req = {1, 50000000};
    struct timespec rem;

    join_server(bot, "lf.lc", "25565");
    send_handshaking_serverbound_handshake(bot, 47, "localhost", 25565, 2);
    send_login_serverbound_login(bot, bot->name);

    while(1) {
        const void* buf = (const void*) ".";
        write(STDOUT_FILENO, buf, strlen(buf));
        nanosleep(&req, &rem);
    }
}

int main() {
    bot_t *an_bot = init_bot("an_bot", *sample_main);
    client_run(an_bot, 1);
    return 0;
}
