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

void set_pos(bot_t *bot, double x, double y, double z)
{
    pthread_mutex_lock(&bot->bot_mutex);

    bot->x = x;
    bot->y = y;
    bot->z = z;

    pthread_mutex_unlock(&bot->bot_mutex);

    send_play_serverbound_player_move(bot, x, y, z, true);
}


void register_login_clientbound_disconnect(bot_t *bot, 
        void (*f)(bot_t *, char *)){
    function *parent = &bot->_data->callbacks[LOGIN][0x00];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_login_clientbound_success(bot_t *bot,
        void (*f)(bot_t *, char *, char *)) {

    function *parent = &bot->_data->callbacks[LOGIN][0x02];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_login_clientbound_set_compression(bot_t *bot,
        void (*f)(bot_t *, vint32_t)) {
    function *parent = &bot->_data->callbacks[LOGIN][0x03];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_status_clientbound_response(bot_t *bot,
        void (*f)(bot_t *, char *)) {
    function *parent = &bot->_data->callbacks[STATUS][0x00];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_status_clientbound_ping(bot_t *bot,
        void (*f)(bot_t *, int64_t)) {
    function *parent = &bot->_data->callbacks[STATUS][0x01];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_keepalive(bot_t *bot,
        void (*f)(bot_t *, vint32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x00];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_join_game(bot_t *bot,
        void (*f)(bot_t *, int32_t, uint8_t, int8_t, uint8_t, uint8_t, char *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x01];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_chat(bot_t *bot,
        void (*f)(bot_t *, char *, int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x02];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_time_update(bot_t *bot,
        void (*f)(bot_t *, int64_t, int64_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x03];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_equipment(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int16_t, slot_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x04];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_spawn_position(bot_t *bot,
        void (*f)(bot_t *, position_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x05];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_update_health(bot_t *bot,
        void (*f)(bot_t *, float, vint32_t, float)) {
    function *parent = &bot->_data->callbacks[PLAY][0x06];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_respawn(bot_t *bot,
        void (*f)(bot_t *, int32_t, uint8_t, uint8_t, char *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x07];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_position(bot_t *bot,
        void (*f)(bot_t *, double, double, double, float, float, int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x08];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_item_change(bot_t *bot,
        void (*f)(bot_t *, int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x09];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_use_bed(bot_t *bot,
        void (*f)(bot_t *, vint32_t, position_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x0a];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_animation(bot_t *bot,
        void (*f)(bot_t *, vint32_t, uint8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x0b];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_spawn_player(bot_t *bot,
        void (*f)(bot_t *, vint32_t, 
            __uint128_t, 
            int32_t, 
            int32_t, 
            int32_t, 
            int8_t, 
            int8_t, 
            int16_t,
            metadata_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x0c];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_collect(bot_t *bot,
        void (*f)(bot_t *, vint32_t, vint32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x0d];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_spawn_object(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int32_t,
            int32_t,
            int32_t,
            int8_t,
            int8_t,
            data_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x0e];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_spawn_mob(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            uint8_t,
            int32_t,
            int32_t,
            int32_t,
            int8_t,
            int8_t,
            int8_t,
            int16_t,
            int16_t,
            int16_t,
            metadata_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x0f];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_spawn_painting(bot_t *bot,
        void (*f)(bot_t *, vint32_t, char *, position_t, uint8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x10];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_spawn_xp(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int32_t, int32_t, int32_t, int16_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x11];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_velocity(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int16_t, int16_t, int16_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x12];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_destroy_entities(bot_t *bot,
        void (*f)(bot_t *, vint32_t, vint32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x13];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity(bot_t *bot,
        void (*f)(bot_t *, vint32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x14];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_move(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            int8_t,
            bool)) {
    function *parent = &bot->_data->callbacks[PLAY][0x15];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_look(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            bool,
            int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x16];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_look_move(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            int8_t,
            int8_t,
            int8_t,
            bool)) {
    function *parent = &bot->_data->callbacks[PLAY][0x17];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_teleport(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int32_t,
            int32_t,
            int32_t,
            int8_t,
            int8_t,
            bool)) {
    function *parent = &bot->_data->callbacks[PLAY][0x18];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_head_look(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x19];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_status(bot_t *bot,
        void (*f)(bot_t *, int32_t, int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x1a];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_attach(bot_t *bot,
        void (*f)(bot_t *, int32_t, int32_t, bool)) {
    function *parent = &bot->_data->callbacks[PLAY][0x1b];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_effect(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            vint32_t,
            bool)) {
    function *parent = &bot->_data->callbacks[PLAY][0x1d];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_clear_effect(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x1e];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_set_xp(bot_t *bot,
        void (*f)(bot_t *, float, int32_t, int32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x1f];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_properties(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int32_t,
            property_t *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x20];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_chunk_data(bot_t *bot,
        void (*f)(bot_t *, int32_t,
            int32_t,
            bool,
            uint16_t,
            vint32_t,
            int8_t *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x21];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_multi_block_change(bot_t *bot,
        void (*f)(bot_t *, int32_t, int32_t, vint32_t, record_t *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x22];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_block_change(bot_t *bot,
        void (*f)(bot_t *, position_t, vint32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x23];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_block_action(bot_t *bot,
        void (*f)(bot_t *, position_t, uint8_t, uint8_t, vint32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x24];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_block_break_animation(bot_t *bot,
        void (*f)(bot_t *, vint32_t, position_t, int8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x25];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_chunk_bulk(bot_t *bot,
        void (*f)(bot_t *, bool,
            vint32_t,
            int32_t,
            int32_t,
            uint16_t,
            int8_t *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x26];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_explosion(bot_t *bot,
        void (*f)(bot_t *, float,
            float,
            float,
            float,
            int32_t,
            record_t *,
            float,
            float,
            float)) {
    function *parent = &bot->_data->callbacks[PLAY][0x27];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_effect(bot_t *bot,
        void (*f)(bot_t *, int32_t, position_t, int32_t, bool)) {
    function *parent = &bot->_data->callbacks[PLAY][0x28];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_sound_effect(bot_t *bot,
        void (*f)(bot_t *, char *,
            int32_t,
            int32_t,
            int32_t,
            float,
            uint8_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x29];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_entity_spawn_global(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int32_t,
            int32_t,
            int32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x2c];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_update_sign(bot_t *bot,
        void (*f)(bot_t *, position_t,
            chat_t,
            chat_t,
            chat_t,
            chat_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x33];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_plugin_message(bot_t *bot,
        void (*f)(bot_t *, char *, int8_t *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x3f];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_plugin_disconnect(bot_t *bot,
        void (*f)(bot_t *, char *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x40];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_plugin_difficulty(bot_t *bot,
        void (*f)(bot_t *, char *)) {
    function *parent = &bot->_data->callbacks[PLAY][0x41];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}

void register_play_clientbound_set_compression(bot_t *bot,
        void (*f)(bot_t *, vint32_t)) {
    function *parent = &bot->_data->callbacks[PLAY][0x46];
    while(parent->next)
        parent = parent->next;
    function *child = calloc(1, sizeof(function));
    parent->f = f;
    parent->next = child;
}
