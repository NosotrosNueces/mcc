#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "bot.h"
#include "client.h"
#include "protocol.h"

void sample_login_handler(bot_t *bot, void *vp) {
    login_clientbound_success_t *p = (login_clientbound_success_t *)vp;
    printf("OMG THAT'S SO GREAT! %s\n", p->username);
    bot->current_state = PLAY;
}

void sample_keepalive_handler(bot_t *bot, void *vp) {
    play_clientbound_keepalive_t *p = (play_clientbound_keepalive_t *)vp;
    printf("OMG THAT'S SO GREAT: %d\n", p->keepalive_id);
    send_play_serverbound_keepalive(bot, p->keepalive_id);
}

void sample_main(void* vbot) {
    bot_t *bot = (bot_t *)vbot;
    struct timespec req = {1, 50000000};
    struct timespec rem;

    send_handshaking_serverbound_handshake(bot, 47, "localhost", 25565, 2);
    bot->current_state = LOGIN;
    send_login_serverbound_login(bot, bot->name);

    while(1) {
        const void* buf = (const void*) ".";
        write(STDOUT_FILENO, buf, strlen(buf));
        nanosleep(&req, &rem);
    }
}

int main() {
    bot_t *an_bot = init_bot("an_bot", *sample_main);
    register_event(an_bot, PLAY, 0x0, sample_keepalive_handler);
    register_event(an_bot, HANDSHAKE, 0x0, sample_login_handler);
    join_server(an_bot, "lf.lc", "25565");
    client_run(an_bot, 1);
    return 0;
}
