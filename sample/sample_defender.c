#include <stdio.h>
#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "api.h"

#define SERVER_NAME "10.10.2.16"
#define DEFAULT_SERVER_PORT 25565

typedef struct entity_node {
    vint32_t entity;
    struct entity_node *next;
} entity_node_t;

entity_node_t targets;

typedef struct bot_globals {
    entity_node_t targets;
} bot_globals_t;

void insert_target(bot_t *bot, vint32_t eid) {
    pthread_mutex_lock(&bot->bot_mutex);
    entity_node_t *p = &((bot_globals_t *)bot->item)->targets;
    while (p->next) {
        p = p->next;
    }
    p->entity = eid;
    p->next = calloc(1, sizeof(entity_node_t));
    pthread_mutex_unlock(&bot->bot_mutex);
}

void remove_target(bot_t *bot, vint32_t eid) {
    pthread_mutex_lock(&bot->bot_mutex);
    entity_node_t *p = &((bot_globals_t *)bot->item)->targets;
    while (p->next) {
        if (p->next->entity == eid) {
            p->next = p->next->next;
        }
        p = p->next;
    }
    pthread_mutex_unlock(&bot->bot_mutex);
}

bool exists_target(bot_t *bot, vint32_t eid) {
    bool ret = false;
    pthread_mutex_lock(&bot->bot_mutex);
    entity_node_t *p = &((bot_globals_t *)bot->item)->targets;
    while (p->next) {
        if (p->entity == eid) {
            ret = true;
            break;
        }
        p = p->next;
    }
    pthread_mutex_unlock(&bot->bot_mutex);
    return ret;
}

void protect(bot_t *bot, vint32_t eid) {
    send_play_serverbound_player_look(bot, 0, 0, true);
    send_play_serverbound_entity_use(bot, eid, 1, 0, 0, 0);
}

void entity_handler(bot_t *bot, void *vp) {
    play_clientbound_spawn_mob_t *p = (play_clientbound_spawn_mob_t *) vp;
    if (p->type >= 50) insert_target(bot, p->entity_id);
}

void entity_move_handler(bot_t *bot, void *vp) {
    play_clientbound_entity_move_t *p = (play_clientbound_entity_move_t *) vp;
    if (exists_target(bot, p->entity_id)) protect(bot, p->entity_id);
}

void entity_status_handler(bot_t *bot, void *vp) {
    play_clientbound_entity_status_t *p = (play_clientbound_entity_status_t *) vp;
    if (p->status == 3) remove_target(bot, p->entity_id);
}

void defender_main(void *vbot) {
    bot_t *bot = (bot_t *)vbot;

    while(1) {
        msleep(500);
        send_play_serverbound_player_status(bot, 0);
    }
}

bot_t *defender_init(char *name) {
    bot_t *bot = init_bot(name, *defender_main);
    bot->item = calloc(1, sizeof(bot_globals_t));

    register_defaults(bot);
    register_event(bot, PLAY, 0x0F, entity_handler);
    register_event(bot, PLAY, 0x15, entity_move_handler);
    register_event(bot, PLAY, 0x1A, entity_status_handler);

    login(bot, SERVER_NAME, DEFAULT_SERVER_PORT);

    return bot;
}
