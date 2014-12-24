#include <stdio.h>
#include <string.h>
#include <time.h>
#include "bot.h"
#include "protocol.h"
#include "handlers.h"
#include "json.h"

#define PROTOCOL_VERSION 47

void msleep(uint64_t ms)
{
    int finished;
    struct timespec req;
    struct timespec rem;

    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;

    do {
        finished = nanosleep(&req, &rem);
        req = rem;
    } while (finished == -1);
}

void register_defaults(bot_t *bot)
{
    // Basic event handling that you should always want
    register_event(bot, LOGIN, 0x02, login_success_handler);
    register_event(bot, PLAY, 0x00, keepalive_handler);
    register_event(bot, PLAY, 0x01, join_game_handler);
    register_event(bot, PLAY, 0x06, update_health_handler);
    register_event(bot, PLAY, 0x08, position_handler);
}

void login(bot_t *bot, char *server_address, int port)
{
    join_server(bot, server_address, port);
    send_handshaking_serverbound_handshake(bot, PROTOCOL_VERSION, "", port, 2);
    send_login_serverbound_login(bot, bot->name);
}

position_t encode_pos(int64_t x, int64_t y, int64_t z)
{
    // Literally http://wiki.vg/Protocol#Position.
    position_t pos = ((x & 0x3FFFFFF) << 38) |
                     ((y & 0xFFF) << 26) |
                     (z & 0x3FFFFFF);
    return pos;
}

void decode_pos(position_t pos, int64_t *x, int64_t *y, int64_t *z)
{
    // Literally http://wiki.vg/Protocol#Position.
    *x = pos >> 38;
    *y = (pos >> 26) & 0xFFF;
    *z = pos << 38 >> 38;
}

bool next_int_token(int* value, char *string, char **saveptr)
{
    char *token = strtok_r(string, " ", saveptr);
    if (token) {
        // Does not validate range or integer-ness.
        *value = strtol(token, NULL, 0);
        return true;
    }
    return false;
}

void execute_command(bot_t *bot, char *command, char *strargs)
{
    if (strcmp(command, "slot") == 0) {
        char **saveptr = calloc(1, sizeof(char *));
        bool valid_input = true;
        char *token = NULL;
        int slot_no;

        valid_input &= next_int_token(&slot_no, strargs, saveptr);

        // Input checking
        if (slot_no < 1 || 9 < slot_no)
            valid_input = false;
        // Actual packet is 0-indexed.
        --slot_no;

        // Ensure there are no trailing tokens.
        token = strtok_r(*saveptr, " ", saveptr);
        if (token)
            valid_input = false;

        free(saveptr);
        if (valid_input) {
            printf("SLOT CHANGE: (%d)\n", slot_no);
            send_play_serverbound_item_change(bot, slot_no);
        } else {
            send_play_serverbound_chat(bot,
                                       "Invalid arguments for SLOT command "
                                       "(1 <= slot <= 9).");
        }
    } else {
        printf("Command not implemented: %s\n", command);
        send_play_serverbound_chat(bot,
                                   "Command not implemented.");
    }
}

void decode_chat_json(char *raw_json, char **msg, char **sender_name)
{
    json_value *json_data = json_parse((json_char *)raw_json,
                                       strlen(raw_json));

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
                            json_value * sender =
                                curr->u.object.values[k].value;
                            *sender_name = malloc(sender->u.string.length + 1);
                            memcpy(*sender_name, sender->u.string.ptr,
                                   sender->u.string.length + 1);
                            break;
                        }
                        if (strcmp(name, "extra") == 0) {
                            json_value * msg_list =
                                curr->u.object.values[k].value;
                            if (msg_list->type == json_array) {
                                *msg = calloc(1, sizeof(char));
                                for (int l = 0;
                                     l <  msg_list->u.array.length;
                                     l++) {
                                    *msg = realloc(*msg,
                                                   strlen(*msg) +
                                                   msg_list->
                                                   u.array.values[l]->
                                                   u.string.length + 1);
                                    strcat(*msg,
                                           msg_list->u.array.values[l]->
                                           u.string.ptr);
                                }
                                break;
                            }
                        }
                    }
                    break;
                case json_string:
                    ;
                    if (strcmp("Server", curr->u.string.ptr) == 0) {
                        *sender_name = malloc(curr->u.string.length + 1);
                        memcpy(*sender_name, curr->u.string.ptr,
                               curr->u.string.length + 1);
                    } else {
                        *msg = malloc(curr->u.string.length + 1);
                        memcpy(*msg, curr->u.string.ptr,
                               curr->u.string.length + 1);
                    }
                    break;
                default:
                    printf("Invalid JSON message error.");
                    break;
                }

            }
            // Short circuit on key == 'with'.
            break;
        }
    }
    json_value_free(json_data);
}
