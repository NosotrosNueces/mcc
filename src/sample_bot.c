#include <stdio.h>
#include <time.h>
#include "bot.h"
#include "client.h"
#include "protocol.h"
#include "api.h"

#define SERVER_NAME "10.10.2.16"
#define DEFAULT_SERVER_PORT 25565

typedef struct entity_node {
    vint32_t entity;
    struct entity_node *next;
} entity_node_t;

entity_node_t targets;

void insert_target(vint32_t eid) {
    entity_node_t *p = &targets;
    while (p->next) {
        p = p->next;
    }
    p->entity = eid;
    p->next = calloc(1, sizeof(entity_node_t));
}

void remove_target(vint32_t eid) {
    entity_node_t *p = &targets;
    while (p->next) {
        if (p->next->entity == eid) {
            p->next = p->next->next;
        }
        p = p->next;
    }    
}

bool exists_target(vint32_t eid) {
    entity_node_t *p = &targets;
    while (p->next) {
        if (p->entity == eid) return true;
        p = p->next;
    }
    return false;
}

void location_handler(bot_t *bot, void *vp) {
    play_clientbound_position_t *p = (play_clientbound_position_t *)vp;
    send_play_serverbound_player_move(bot, p->x, p->y, p->z, true);
}

void health_handler(bot_t *bot, void *vp) {
    play_clientbound_update_health_t *p = (play_clientbound_update_health_t *)vp;
    if(p->health == 0) {
        send_play_serverbound_player_status(bot, 0);
    }
}

void protect(bot_t *bot, vint32_t eid) {
    send_play_serverbound_player_look(bot, 0, 0, true);
    send_play_serverbound_entity_use(bot, eid, 1, 0, 0, 0);
}

void entity_handler(bot_t *bot, void *vp) {
    play_clientbound_spawn_mob_t *p = (play_clientbound_spawn_mob_t *) vp;
    if (p->type >= 50) insert_target(p->entity_id);
}

void entity_move_handler(bot_t *bot, void *vp) {
    play_clientbound_entity_move_t *p = (play_clientbound_entity_move_t *) vp;
    if (exists_target(p->entity_id)) protect(bot, p->entity_id);
}

void entity_status_handler(bot_t *bot, void *vp) {
    play_clientbound_entity_status_t *p = (play_clientbound_entity_status_t *) vp;
    if (p->status == 3) remove_target(p->entity_id);
}

void sample_main(void *vbot) {
    bot_t *bot = (bot_t *)vbot;
    register_defaults(bot);

    while(1) {
        msleep(500);
        send_play_serverbound_player_status(bot, 0);
    }
}

int main() {
    bot_t *bot = init_bot("plants", *sample_main);
    register_event(bot, PLAY, 0x08, location_handler);
    register_event(bot, PLAY, 0x06, health_handler);
    register_event(bot, PLAY, 0x0F, entity_handler);
    register_event(bot, PLAY, 0x15, entity_move_handler);
    register_event(bot, PLAY, 0x1A, entity_status_handler);
    login(bot, SERVER_NAME, DEFAULT_SERVER_PORT);
    client_run(bot, 1);
    free_bot(bot);
    return 0;
}
