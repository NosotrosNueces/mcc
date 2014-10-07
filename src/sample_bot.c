#include <stdio.h>
#include <time.h>
#include "bot.h"
#include "client.h"
#include "protocol.h"
#include "api.h"

#define SERVER_NAME "localhost"
#define DEFAULT_SERVER_PORT 25565
#define ZOMBIE_NUMBER 65536
double x = 0;
double y = 0;
double z = 0;
int32_t zombies[ZOMBIE_NUMBER][4];

void location_handler(bot_t *bot, void *vp) {
    play_clientbound_position_t *p = (play_clientbound_position_t *)vp;
    /* printf("I'm at %f %f %f\n", p->x, p->y, p->z); */
    x = p->x;
    y = p->y;
    z = p->z;
    send_play_serverbound_player_move(bot, p->x, p->y, p->z, true);
}

void health_handler(bot_t *bot, void *vp) {
    play_clientbound_update_health_t *p = (play_clientbound_update_health_t *)vp;
    char words[500];
    /* sprintf(words, "health: %f\n", p->health); */
    /* say(bot, words); */
    printf("health: %f\n", p->health);
    if(p->health == 0) {
        send_play_serverbound_player_status(bot, 0);
        send_play_serverbound_chat(bot, "I'm back!");
    }
}

void protect(bot_t *bot, int32_t *zombie) {
    double zx = ((double) zombie[1] )/ 32;
    double zy = ((double) zombie[2] )/ 32;
    double zz = ((double) zombie[3] )/ 32;

    double dist = fabs(x - zx) + fabs(y - zy) + fabs(z - zz);
    /* printf("P->x:%f,y:%f,z:%f Z->x:%f,y:%f,z:%f D->%f\n", */
    /*     x, y, z, zx, zy, zz, dist); */
    if(dist < 4) {
        send_play_serverbound_player_look(bot, 0, 0, true);
        char words[500];
        sprintf(words, "attack!!! (e:%d d:%f)\n", zombie[0], dist);
        /* say(bot, words); */
        send_play_serverbound_entity_use(bot, zombie[0], 1, 0, 0, 0);
    }
}

void entity_handler(bot_t *bot, void *vp) {
    play_clientbound_spawn_mob_t *p = (play_clientbound_spawn_mob_t *) vp;
    if(p->type == 54) {
        /* char words[500]; */
        /* sprintf(words, "Zombie %d spawned\n", p->entity_id); */
        /* say(bot, words); */
        int32_t *loc = zombies[p->entity_id % ZOMBIE_NUMBER];
        loc[0] = p->entity_id;
        loc[1] = p->x;
        loc[2] = p->y;
        loc[3] = p->z;
        protect(bot, loc);
    }
}

void entity_move_handler(bot_t *bot, void *vp) {
    play_clientbound_entity_move_t *p = (play_clientbound_entity_move_t *) vp;
    /* printf("Entity %d moved\n", p->entity_id); */
    if(zombies[p->entity_id % ZOMBIE_NUMBER][0] == p->entity_id) {
        int32_t *loc = zombies[p->entity_id % ZOMBIE_NUMBER];
        loc[1] += p->dx;
        loc[2] += p->dy;
        loc[3] += p->dz;
        protect(bot, loc);
    }
}

void sample_main(void *vbot) {
    bot_t *bot = (bot_t *)vbot;
    register_defaults(bot);

    while(1) {
        msleep(500);
    }
}

int main() {
    bot_t *bot = init_bot("plants", *sample_main);
    register_event(bot, PLAY, 0x08, location_handler);
    register_event(bot, PLAY, 0x06, health_handler);
    register_event(bot, PLAY, 0x0F, entity_handler);
    register_event(bot, PLAY, 0x15, entity_move_handler);
    login(bot, SERVER_NAME, DEFAULT_SERVER_PORT);
    client_run(bot, 1);
    free_bot(bot);
    return 0;
}
