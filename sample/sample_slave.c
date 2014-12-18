#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include "bot.h"
#include "protocol.h"
#include "json.h"
#include "api.h"
#include "timers.h"

typedef struct bot_globals {
    int status;
    sem_t dig_sem;
    position_t current_block;
} bot_globals_t;

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

position_t encode_location(uint64_t x, uint64_t  y, uint64_t z) {
    position_t location = ((x & 0x3FFFFFF) << 38) |
                          ((y & 0xFFF) << 26) |
                          (z & 0x3FFFFFF);
    return location;
}

void block_update_handler(bot_t *bot, void *vp) {
    play_clientbound_block_change_t *p =
        (play_clientbound_block_change_t *)vp;
    //printf("BLOCK UPDATE (loc, id) = (%lx, %lx).\n", p->location, p->block_id);
    if ((p->block_id >> 4) == 0) {
        if (((bot_globals_t *)bot->state)->current_block == p->location) {
            //printf("Finished mining current_block.\n");
    
            sem_post(&((bot_globals_t *)bot->state)->dig_sem);
            ((bot_globals_t *)bot->state)->current_block = 0;
        }
    }
}

void dig(bot_t *bot, int x, int y, int z) {
    //int sem_val;
    //printf("exec dig @ (%d, %d, %d)\n", x, y, z);
    //sem_getvalue(&((bot_globals_t *)bot->state)->dig_sem, &sem_val);
    //printf("Semaphore value: %d.\n", sem_val);

    position_t location = encode_location(x, y, z);
    ((bot_globals_t *)bot->state)->current_block = location;

    send_play_serverbound_player_dig(bot, 0x00, location, 0x01);
    send_play_serverbound_player_dig(bot, 0x02, location, 0x01);
}

struct clear_data {
    bot_t *bot;
    int x1, y1, z1, x2, y2, z2;
    pthread_t *self;
};

void *clear_area(void *d) {
    int x_start, dx, y_start, dy, z_start, dz;
    struct clear_data *data = d;
    x_start = data->x1 < data->x2 ? data->x1 : data->x2;
    dx = abs(data->x1 - data->x2);
    y_start = data->y1 < data->y2 ? data->y1 : data->y2;
    dy = abs(data->y1 - data->y2);
    z_start = data->z1 < data->z2 ? data->z1 : data->z2;
    dz = abs(data->z1 - data->z2);
    for (int x = 0; x <= dx; ++x) {
        for (int y = 0; y <= dy; ++y) {
            for (int z = 0; z <= dz; ++z) {
                dig(data->bot, x_start + x, y_start + y, z_start + z);
                sem_wait(&((bot_globals_t *)data->bot->state)->dig_sem);
            }
        }
    }
    free(d);
    return NULL;
}

void exec(bot_t *bot, char *command, char *strargs)
{
    if (strcmp(command, "clear") == 0) {
        static pthread_t clear_area_thread = 0;
        char **saveptr = calloc(1, sizeof(char *));
        bool valid_input = true;
        char *token = NULL;
        int x1, y1, z1, x2, y2, z2;

        // Never clear multiple areas at once.
        if (clear_area_thread && !pthread_kill(clear_area_thread, 0)) {
            printf("CANCELLED: CLEAR\n");
            send_play_serverbound_chat(bot,
                                       "CLEAR command is already running.");
            return;
        }

        valid_input &= next_int_token(&x1, strargs, saveptr);
        valid_input &= next_int_token(&y1, *saveptr, saveptr);
        valid_input &= next_int_token(&z1, *saveptr, saveptr);
        valid_input &= next_int_token(&x2, *saveptr, saveptr);
        valid_input &= next_int_token(&y2, *saveptr, saveptr);
        valid_input &= next_int_token(&z2, *saveptr, saveptr);

        // Ensure there are no trailing tokens.
        token = strtok_r(*saveptr, " ", saveptr);
        if (token) {
            valid_input = false;
        }

        free(saveptr);
        if (valid_input) {
            printf("CLEAR: (%d, %d, %d) to (%d, %d, %d)\n",
                   x1, y1, z1,
                   x2, y2, z2);
            struct clear_data *data = calloc(1, sizeof(struct clear_data));
            data->bot = bot;
            data->x1 = x1;
            data->y1 = y1;
            data->z1 = z1;
            data->x2 = x2;
            data->y2 = y2;
            data->z2 = z2;
            pthread_create(&clear_area_thread, NULL, clear_area, data);
        } else {
            send_play_serverbound_chat(bot,
                                       "Invalid arguments for CLEAR command "
                                       "(x1, y1, z1, x2, y2, z2).");
        }
    } else if (strcmp(command, "dig") == 0) {
        char **saveptr = calloc(1, sizeof(char *));
        bool valid_input = true;
        char *token = NULL;
        int x, y, z;

        valid_input &= next_int_token(&x, strargs, saveptr);
        valid_input &= next_int_token(&y, *saveptr, saveptr);
        valid_input &= next_int_token(&z, *saveptr, saveptr);

        // Ensure there are no trailing tokens.
        token = strtok_r(*saveptr, " ", saveptr);
        if (token) {
            valid_input = false;
        }

        free(saveptr);
        if (valid_input) {
            printf("DIG: (%d, %d, %d)\n", x, y, z);
            dig(bot, x, y, z);
        } else {
            send_play_serverbound_chat(bot,
                                       "Invalid arguments for DIG command "
                                       "(x, y, z).");
        }
    } else if (strcmp(command, "sem") == 0) {
        static char msg[64];
        int sem_val;
        sem_getvalue(&((bot_globals_t *)bot->state)->dig_sem, &sem_val);
        sprintf(msg, "Semaphore value: %d\n", sem_val);
        send_play_serverbound_chat(bot, msg);
    } else if (strcmp(command, "slot") == 0) {
        char **saveptr = calloc(1, sizeof(char *));
        bool valid_input = true;
        char *token = NULL;
        int slot_no;

        valid_input &= next_int_token(&slot_no, strargs, saveptr);

        // Ensure there are no trailing tokens.
        token = strtok_r(*saveptr, " ", saveptr);
        if (token) {
            valid_input = false;
        }

        free(saveptr);
        if (valid_input) {
            printf("SLOT CHANGE: (%d)\n", slot_no);
            send_play_serverbound_item_change(bot, slot_no);
        } else {
            send_play_serverbound_chat(bot,
                                       "Invalid arguments for SLOT command "
                                       "(0-indexed slot).");
        }
    } else {
        printf("Command not implemented: %s\n", command);
        send_play_serverbound_chat(bot,
                                   "Command not implemented.");
    }
}

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

void chat_handler(bot_t *bot, void *vp)
{
    play_clientbound_chat_t *p =
        (play_clientbound_chat_t *)vp;
    char *msg = NULL;
    char *sender = NULL;
    decode_chat_json(p->json, &msg, &sender);
    // Ensure that we do not echo ourselves,
    // and that we received a chat message (not a server message).
    if (msg && sender && strcmp(sender, bot->name)) {
        // Commands to bots start with a backslash.
        // Only operate on non-empty commands.
        if (msg[0] == '\\' && msg[1]) {
            // Parse space delimited tokens.
            char **saveptr = calloc(1, sizeof(char *));
            char *command = strtok_r(msg+1, " ", saveptr);
            exec(bot, command, *saveptr);
            free(saveptr);
        } else {
            printf("CHAT: <%s> %s\n", sender, msg);
        }
    }
    free(sender);
    free(msg);
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

bot_t *init_slave(char *name, char *server_name, int port)
{
    bot_t *bot = init_bot(name, *slave_main);
    bot->state = calloc(1, sizeof(bot_globals_t));
    sem_init(&((bot_globals_t *)bot->state)->dig_sem, 0, 0);

    register_defaults(bot);
    register_event(bot, PLAY, 0x02, chat_handler);
    register_event(bot, PLAY, 0x23, block_update_handler);

    login(bot, server_name, port);

    return bot;
}
