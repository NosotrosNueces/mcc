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


int main() {
    struct bot_agent bot;
    init_bot(&bot, "foo");
    bot.callbacks.clientbound_play_player_list_item_cb = player_list_item;
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
