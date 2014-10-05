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
    printf("Logged in as: %s\n", p->username);
    bot->current_state = PLAY;
}

void sample_keepalive_handler(bot_t *bot, void *vp) {
    play_clientbound_keepalive_t *p = (play_clientbound_keepalive_t *)vp;
    printf("keepalive_id: %d\n", p->keepalive_id);
    send_play_serverbound_keepalive(bot, p->keepalive_id);
}

void time_update_checker(bot_t *bot, void *vp) {
    play_clientbound_time_update_t *p = (play_clientbound_time_update_t *)vp;
    printf("Game time: %lu\n", p->time);
}

void sample_main(void* vbot) {
    bot_t *bot = (bot_t *)vbot;
    struct timespec req = {0, 5000000};
    struct timespec rem;

    send_handshaking_serverbound_handshake(bot, 47, "localhost", 25565, 2);
    bot->current_state = LOGIN;
    send_login_serverbound_login(bot, bot->name);

    while(1) {
        //I'm on the ground, trust me!
        //send_play_serverbound_player(bot, true);
        nanosleep(&req, &rem);
    }
}

int main() {
    bot_t *bot = init_bot("botrbert", *sample_main);

    register_event(bot, PLAY, 0x00, sample_keepalive_handler);
    register_event(bot, LOGIN, 0x02, sample_login_handler);
    register_event(bot, PLAY, 0x03, time_update_checker);

    join_server(bot, "localhost", "25565");

    bot_t* test_bots = calloc(1, sizeof(bot_t));
    test_bots[0] = *bot;

    client_run(test_bots, 1);

    free_bot(bot);
    return 0;
}
