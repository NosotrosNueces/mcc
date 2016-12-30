#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "bot.h"
#include "protocol.h"
#include "capture.h"
#include "break.h"
#include "throttle.h"

#define DEFAULT_SERVER_NAME "localhost"
#define DEFAULT_SERVER_PORT "25565"

struct player {
    char uuid[16];
    char *name;
};

struct list_node {
    void *data;
    struct list_node *prev;
    struct list_node *next;
};

struct list {
    struct list_node head;
    struct list_node tail;
};

struct list player_list;

void list_init(struct list *l) {
    l->head.next = &l->tail;
    l->tail.prev = &l->head;
}

void list_append(struct list *l, void *data) {
    struct list_node *n = malloc(sizeof(struct list_node));
    n->data = data;
    n->prev = l->tail.prev;
    l->tail.prev->next = n;
    n->next = &l->tail;
    l->tail.prev = n;
}

void *list_find(struct list *l, int (*constraint) (void *, void *), void *compare) {
    struct list_node *current = l->head.next;
    while (current != &l->tail) {
        if (constraint(compare, current->data)) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

int compare_uuid(void *_uuid, void *_data) {
    char *uuid = _uuid;
    struct player *p = _data;
    return !memcmp(uuid, p->uuid, sizeof(p->uuid));
}

void spawn_player(
            struct bot_agent *bot,
            vint32_t entity_id,
            char *uuid,
            double x,
            double y,
            double z,
            uint8_t yaw,
            uint8_t pitch,
            struct entity_metadata *metadata
            ) {
    struct player *p = list_find(&player_list, compare_uuid, uuid);
    char *message = malloc(strlen(p->name) + strlen("Hello !") + 1);
    snprintf(message, strlen(p->name) + strlen("Hello !") + 1, "Hello %s!", p->name);
    send_play_serverbound_chat_message(bot, message);
    free(message);
}

void player_list_item (
            struct bot_agent *bot,
            vint32_t action_type,
            vint32_t number_of_players,
            struct player_list_action *player_actions
            ) {
    for (int i = 0; i < number_of_players; i++) {
        switch(action_type) {
            case PLAYER_LIST_ADD_PLAYER:
                printf("Hello %s!\n", player_actions[i].add_player.name);
                char *message = malloc(strlen(player_actions[i].add_player.name) + strlen("Hello !") + 1);
                snprintf(message, strlen(player_actions[i].add_player.name) + strlen("Hello !") + 1, "Hello %s!", player_actions[i].add_player.name);
                send_play_serverbound_chat_message(bot, message);
                free(message);
                struct player *new_player = malloc(sizeof(struct player));
                memcpy(new_player->uuid, player_actions[i].uuid, sizeof(new_player->uuid));
                new_player->name = malloc(strlen(player_actions[i].add_player.name) + 1);
                strcpy(new_player->name, player_actions[i].add_player.name);
                list_append(&player_list, new_player);
                break;
            case PLAYER_LIST_UPDATE_GAMEMODE:

                break;
            case PLAYER_LIST_UPDATE_LATENCY:
                break;
            case PLAYER_LIST_UPDATE_DISPLAY_NAME:
                break;
            case PLAYER_LIST_REMOVE_PLAYER:
                break;
        }
    }
}

void chunk_data(
        struct bot_agent *bot,
        int32_t chunk_x,
        int32_t chunk_z,
        bool gound_up_continuous,
        vint32_t primary_bit_mask,
        int32_t number_of_sections,
        struct chunk_section *data,
        struct biome_record *biomes,
        vint32_t number_of_block_entities,
        struct nbt_tag *block_entities
        ) {

    printf("Xcoord, Zcoord: %d, %d\n", chunk_x, chunk_z);
    int chunk_index = 0;
    for (int i = 0; i < 16; i++) { 
        if (((1 << i) & primary_bit_mask) == 0) {
            continue;
        }
        printf("YCoord: %d\n", i);
        for (int y = 0; y < 16; y++) {
            printf("Y: %d\n", y);
            for (int z = 0; z < 16; z++) {
                printf("    Z: %d\n", z);
                printf("        X: ");
                for (int x = 0; x < 16; x++) {
                    printf("%d, ", x);
                }
                printf("\n           ");
                for (int x = 0; x < 16; x++) {
                    printf("%d, ", data[chunk_index].data_array[x][y][z]);
                }
                printf("\n");
            }
        }
        chunk_index++;
    }
    /* 
    printf("Biomes");
    for (int z = 0; z < 16; z++) {
        printf("");
        for (int x = 0; x < 16; x++) {
            
        }
    }
    */
}

void update_health (
            struct bot_agent *bot,
            float health,
            vint32_t food,
            float food_saturation
            ) {
    if (health <= 0.0) {
        send_play_serverbound_client_status(bot, 0);
    }
    printf("Update health: %f\n", health);
}

double player_x, player_y, player_z;

static inline double distance(struct mc_position p1, struct mc_position p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dz = p1.z - p2.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

#define POSITION_X(p)   ((int64_t)(p) >> 38)
#define POSITION_Y(p)   (((int64_t)(p) >> 26) & 0xFF)
#define POSITION_Z(p)   ((int64_t)(p) << 38 >> 38)

void break_stop_cb (uv_timer_t* handle) {
     /* start break timer */
    struct bot_agent *bot = handle->data;
    send_play_serverbound_player_digging(
        bot,
        2,
        bot->block_break_location,
        0
        );
    uv_timer_stop(handle);
    bot->is_breaking = 0;
}

void start_block_break(struct bot_agent *bot, int32_t block_id, struct mc_position location) {
    /* start break timer */
    if (bot->is_breaking) {
        return;
    }
    double break_time = block_break_time_hand(block_id);
    if (break_time == INFINITY) {
        return;
    }
    uint64_t break_time_milli = (uint64_t)(break_time * 1000);

    bot->block_break_location = location;
    
    send_play_serverbound_player_digging(
        bot,
        0,
        location,
        1
        );

    uv_timer_init(&bot->loop, &bot->block_break_timer);
    uv_timer_start(&bot->block_break_timer, break_stop_cb, break_time_milli, 0);
    bot->is_breaking = 1;
}

void stop_block_break(struct bot_agent *bot) {
    uv_timer_stop(&bot->block_break_timer);
    bot->is_breaking = 0;
}

void block_change_cb(
            struct bot_agent *bot,
            struct mc_position location,
            vint32_t block_id
            ) {
    if (distance(location, bot->position) < 10) {
        const char *block_name_str = block_name(block_id);
        char message[512];
        snprintf(message,
                sizeof(message),
                "%s: (%d, %d, %d)\n",
                block_name_str,
                (int)location.x,
                (int)location.y,
                (int)location.z
                );
        printf("%s", message);
        send_play_serverbound_chat_message(bot, message);

        snprintf(message,
                sizeof(message),
                "    Hardness: %f\n",
                block_hardness(block_id)
                );
        printf("%s", message);
        send_play_serverbound_chat_message(bot, message);

        snprintf(message,
                sizeof(message),
                "    Break time by hand: %f\n",
                block_break_time_hand(block_id)
                );
        printf("%s", message);
        send_play_serverbound_chat_message(bot, message);
        printf("    metadata: 0x%02x\n", block_id & 15);
        
        start_block_break(bot, block_id, location);
    }
}

void player_position_and_look_cb (
            struct bot_agent *bot,
            double x,
            double y,
            double z,
            float yaw,
            float pitch,
            int8_t flags,
            vint32_t teleport_id
            ) {
    player_x = x;
    player_y = y;
    player_z = z;
}

int main(int argc, char *argv[], char **envp)
{
    struct bot_agent bot;
    char *server_name = DEFAULT_SERVER_NAME;
    char *server_port = DEFAULT_SERVER_PORT;
    char *capture_file = NULL; 

    switch (argc) {
        case 4:
            capture_file = argv[3]; 
        case 3:
            server_port = argv[2];
            break;
        case 2:
            server_name = argv[1];
            break;
        default:
            printf("Expected arguments: "
                   "./mcc [<server> [<port> [<debug dump>]]\n");
            return -1;
    }

    init_bot(&bot, "Sintralin");
    bot.callbacks.clientbound_play_player_list_item_cb = player_list_item;
    //bot.callbacks.clientbound_play_chunk_data_cb = chunk_data;
    bot.callbacks.clientbound_play_spawn_player_cb = spawn_player;
    bot.callbacks.clientbound_play_update_health_cb = update_health;    
    bot.callbacks.clientbound_play_block_change_cb = block_change_cb;
    bot.callbacks.clientbound_play_player_position_and_look_cb = player_position_and_look_cb;
    list_init(&player_list);

    /* packet capture */
    bot.capture_enabled = 1;
    init_capture(&bot, capture_file);

    join_server_hostname(&bot, server_name, server_port);
    init_throttle(&bot, 1000);
    uv_run(&bot.loop, UV_RUN_DEFAULT);
    while(1);
    uv_loop_close(&bot.loop);
}
