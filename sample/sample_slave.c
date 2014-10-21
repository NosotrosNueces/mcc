#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "json.h"
#include "api.h"

typedef struct bot_globals {
    int status;
} bot_globals_t;

void decode_chat_json(char *raw_json, char **msg, char **sender_name)
{
    json_value *json_data = json_parse((json_char *)raw_json, strlen(raw_json));

    for (int i = 0; i < json_data->u.object.length; i++) {
        char * name = json_data->u.object.values[i].name;
        if (strcmp(name, "with") == 0) {
            json_value * arr = json_data->u.object.values[i].value;
            // Iterate over array data['with'].
            for (int j = 0; j < arr->u.array.length; j++) {
                json_value * curr = arr->u.array.values[j];
                switch (curr->type) {
                // The sender is in the object data.
                case json_object:
                    ;
                    for (int k = 0; k < curr->u.object.length; k++) {
                        char * name = curr->u.object.values[k].name;
                        if (strcmp(name, "text") == 0) {
                            // sender_name = data['with'][0]['text']
                            json_value * sender = curr->u.object.values[k].value;
                            *sender_name = malloc(sender->u.string.length + 1);
                            memcpy(*sender_name, sender->u.string.ptr,
                                   sender->u.string.length + 1);
                            break;
                        }
                    }
                    break;
                // The actual message is the string.
                case json_string:
                    ;
                    // msg = data['with'][1]
                    *msg = malloc(curr->u.string.length + 1);
                    memcpy(*msg, curr->u.string.ptr, curr->u.string.length + 1);
                    break;
                default:
                    printf("invalid JSON message error.");
                    break;
                }

            }
            // Short circuit on key == 'with'.
            break;
        }
    }
    json_value_free(json_data);
}

void chat_handler(bot_t *bot, void *vp)
{
    play_clientbound_chat_t *p =
        (play_clientbound_chat_t *)vp;
    char *msg = NULL;
    char *sender = NULL;
    decode_chat_json(p->json, &msg, &sender);
    // Ensure that we do not echo ourselves
    if (msg && strcmp(sender, bot->name)) {
        printf("CHAT: <%s> %s\n", sender, msg);
        send_play_serverbound_chat(bot, msg);
    }
    free(sender);
    free(msg);
}

void slave_main(void *vbot)
{
    bot_t *bot = (bot_t *)vbot;

    while(1) {
        msleep(500);
        //send_play_serverbound_player(bot, true);
        send_play_serverbound_player_status(bot, 0);
    }
}

bot_t *init_slave(char *name, char *server_name, int port)
{
    bot_t *bot = init_bot(name, *slave_main);
    bot->item = calloc(1, sizeof(bot_globals_t));

    register_defaults(bot);
    register_event(bot, PLAY, 0x02, chat_handler);

    login(bot, server_name, port);

    return bot;
}
