#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "api.h"
#include "handlers.h"
#include "timers.h"

typedef struct entity {
    vint32_t eid;
} entity_t;

typedef struct entity_node {
    entity_t *entity;
    struct entity_node *next;
} entity_node_t;

typedef struct bot_globals {
    entity_node_t targets;
} bot_globals_t;

void _insert_target(bot_t *bot, entity_t *entity)
{
    pthread_mutex_lock(&bot->bot_mutex);

    entity_node_t *p = &((bot_globals_t *)bot->state)->targets;
    while (p->next) {
        p = p->next;
    }
    p->entity = entity;
    p->next = calloc(1, sizeof(entity_node_t));

    pthread_mutex_unlock(&bot->bot_mutex);
}

void _remove_target(bot_t *bot, entity_t *entity)
{
    pthread_mutex_lock(&bot->bot_mutex);

    entity_node_t *p = &((bot_globals_t *)bot->state)->targets;
    while (p->next) {
        if (p->next->entity->eid == entity->eid) {
            free(p->next->entity);
            p->next = p->next->next;
        }
        p = p->next;
    }

    pthread_mutex_unlock(&bot->bot_mutex);
}

entity_t *get_target(bot_t *bot, vint32_t eid)
{
    entity_t *target = NULL;
    pthread_mutex_lock(&bot->bot_mutex);

    entity_node_t *p = &((bot_globals_t *)bot->state)->targets;
    while (p->next) {
        if (p->entity->eid == eid) {
            target = p->entity;
            break;
        }
        p = p->next;
    }

    pthread_mutex_unlock(&bot->bot_mutex);
    return target;
}

void slave_main(void *vbot)
{
    bot_t *bot = (bot_t *)vbot;
    struct timeval delay = {0, 500000};
    register_timer(bot, delay, -1, timer_echo_pos);

    msleep(500);
    send_play_serverbound_player_status(bot, 0);
    send_play_serverbound_item_change(bot, 0);

    pause();
}

void chicken_watcher(bot_t *bot, vint32_t entity_id,
                    uint8_t type,
                    int32_t x,
                    int32_t y,
                    int32_t z,
                    int8_t yaw,
                    int8_t pitch,
                    int8_t head_pitch,
                    int16_t dx,
                    int16_t dy,
                    int16_t dz,
                    metadata_t metadata)
{
    if (type == 93) {
        printf("Found new chicken with eid: %d\n", entity_id);
        entity_t *e = calloc(1, sizeof(entity_t));
        e->eid = entity_id;
        _insert_target(bot, e);
    }
}

void move(bot_t *bot, float dx, float dy, float dz)
{
    pthread_mutex_lock(&bot->bot_mutex);

    bot->x += dx;
    bot->y += dy;
    bot->z += dz;

    send_play_serverbound_player_move(bot,
            dx+bot->x, dy+bot->y, dz+bot->z, true);

    pthread_mutex_unlock(&bot->bot_mutex);
}

void target_teleport_handler(bot_t *bot, vint32_t entity_id,
                  int32_t x,
                  int32_t y,
                  int32_t z,
                  int8_t yaw,
                  int8_t pitch,
                  bool on_ground)
{
    printf("Entity %d teleported: %f %f %f.\n", entity_id, x/32., y/32., z/32.);
    set_pos(bot, x/32., y/32., z/32.);
}

void target_move_handler(bot_t *bot, vint32_t entity_id,
                         int8_t dx,
                         int8_t dy,
                         int8_t dz,
                         bool on_ground)
{
    entity_t *target = get_target(bot, entity_id);
    if (target) {
        printf("Entity %d moved: %f %f %f.\n", entity_id,
                dx/32., dy/32., dz/32.);
        move(bot, dx/32., dy/32., dz/32.);
    }
}

void people_watcher(bot_t *bot, vint32_t entity_id,
                    __uint128_t uuid,
                    int32_t x,
                    int32_t y,
                    int32_t z,
                    int8_t yaw,
                    int8_t pitch,
                    int16_t item,
                    metadata_t metadata)
{
    printf("Found player with eid: %d\n", entity_id);

    entity_node_t *p = &((bot_globals_t *)bot->state)->targets;
    // Only follow one player at a time.
    if (p) {
        entity_t *e = calloc(1, sizeof(entity_t));
        e->eid = entity_id;
        _insert_target(bot, e);
    }
}

bot_t *init_slave(char *name, char *server_name, int port)
{
    bot_t *bot = init_bot(name, *slave_main);
    bot->state = calloc(1, sizeof(bot_globals_t));

    register_defaults(bot);
    register_play_clientbound_chat(bot, mcc_chat_handler);
    register_play_clientbound_update_health(bot, mcc_autorespawn_handler);
    //register_play_clientbound_spawn_mob(bot, chicken_watcher);
    register_play_clientbound_spawn_player(bot, people_watcher);
    register_play_clientbound_entity_move(bot, target_move_handler);
    register_play_clientbound_entity_teleport(bot, target_teleport_handler);

    login(bot, server_name, port);

    return bot;
}
