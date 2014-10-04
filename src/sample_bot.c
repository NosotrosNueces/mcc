#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include "bot.h"
#include "client.h"

void sample_main(void* bot) {
    //struct timespec req = {1, 50000000};
    //struct timespec rem;

    //join_server(bot, "25567", "10.10.2.16", "25565");
    //send_handshaking_serverbound_handshake(bot, 47, "localhost", 25565, 2);
    //send_login_serverbound_login(bot, bot->name);

    //while(1) {
    //    printf("Doing something???");
    //    nanosleep(&req, &rem);
    //}
}

int main() {
    bot_t *an_bot = init_bot("an_bot", *sample_main);
    client_run(an_bot, 1);
    return 0;
}
