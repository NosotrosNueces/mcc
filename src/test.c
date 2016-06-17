#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "bot.h"
#include "protocol.h"

void *_read(void *buffer, void *storage, size_t size);
void *_read_vint32(void *buffer, int32_t *val);
void *_read_string(void *buffer, char **strptr, int32_t *len);

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

    printf("Xcoord, Zcoord: %d, %d\n", chunk_x * 16, chunk_z * 16);
    int chunk_index = 0;
    for (int i = 0; i < 16; i++) { 
        if (((1 << i) & primary_bit_mask) == 0) {
            continue;
        }
        printf("YCoord: %d\n", i * 16);
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
    
    printf("Biomes");
    for (int z = 0; z < 16; z++) {
        printf("");
        for (int x = 0; x < 16; x++) {
            
        }
    }
}


int main() {
    struct bot_agent bot;
    init_bot(&bot, "foo");
    bot.callbacks.clientbound_play_player_list_item_cb = player_list_item;
    bot.callbacks.clientbound_play_chunk_data_cb = chunk_data;
    join_server(&bot, "localhost", 25565);
    uv_run(&bot.loop, UV_RUN_DEFAULT);
    while(1);
    uv_loop_close(&bot.loop);
    

    //FILE *f = fopen("hello_world.nbt", "rb");
    //if (f == NULL) {
    //    return -1;
    //}
    //fseek(f, 0, SEEK_END);
    //long length = ftell(f);
    //fseek(f, 0, SEEK_SET);
    //char *nbt = malloc(length);
    //fread(nbt, length, sizeof(char), f);
    //uint32_t bytes_read;
    //struct nbt_tag *root = nbt_parse(nbt, &bytes_read);
    //nbt_print(root);
    //printf("Bytes read: %u\n", bytes_read);
    //printf("length: %ld\n", length);
}
