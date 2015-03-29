#pragma once

#include "bot.h"
#include "types.h"

/* Briefly suspend thread execution for an interval measured in ms
 *
 *	Call nanosleep until it successfully sleeps for the required interval.
 */
void msleep(uint64_t ns);

/*
 * Registers
 *      keepalive_handler,
 *      login_success_handler,
 *      position_handler.
 */
void register_defaults(bot_t *bot);

/*
 * Connect to the server, initiate the handshake, and login.
 */
void login(bot_t *bot, char *server_address, int port);

/*
 * Execute actions based on a command and arguments.
 * Both inputs are currently strings.
 */
void execute_command(bot_t *bot, char *command, char *strargs);

/*
 * This function parses user or server JSON for sender and message.
 *
 * User chat format:
 * data = {
 *   "with": [{"insertion": "JDongian",
 *             "text": "JDongian"},
 *            "hello"
 *           ]
 * }
 * Desired result (sender, message):
 * (data['with'][0]['text'], data['with'][1])
 *
 * Server chat format:
 * data = {
 *   "with": ["Server",
 *            {"extra": ["\dig", " ", "0", " ", "0", " ", "0"]}
 *           ]
 * }
 * Desired result (sender, message):
 * (data['with'][0], ''.join(data['with']['extra']))
 *
 * N.B.
 * This function allocates memory for *msg and *sender_name.
 * Please free it.
 */
void decode_chat_json(char *raw_json, char **msg, char **sender_name);

/*
 * This is the correct way to set the bot's position.
 */
void set_pos(bot_t *bot, double x, double y, double z);

/* Register callback handlers */

void register_login_clientbound_disconnect(bot_t *bot, 
        void (*f)(bot_t *, char *));

void register_login_clientbound_success(bot_t *bot,
        void (*f)(bot_t *, char *, char *));

void register_login_clientbound_set_compression(bot_t *bot,
        void (*f)(bot_t *, vint32_t));

void register_status_clientbound_response(bot_t *bot,
        void (*f)(bot_t *, char *));

void register_status_clientbound_ping(bot_t *bot,
        void (*f)(bot_t *, int64_t));

void register_play_clientbound_keepalive(bot_t *bot,
        void (*f)(bot_t *, vint32_t));

void register_play_clientbound_join_game(bot_t *bot,
        void (*f)(bot_t *, int32_t, uint8_t, int8_t, uint8_t, uint8_t, char *));

void register_play_clientbound_chat(bot_t *bot,
        void (*f)(bot_t *, char *, int8_t));

void register_play_clientbound_time_update(bot_t *bot,
        void (*f)(bot_t *, int64_t, int64_t));

void register_play_clientbound_entity_equipment(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int16_t, slot_t));

void register_play_clientbound_spawn_position(bot_t *bot,
        void (*f)(bot_t *, position_t));

void register_play_clientbound_update_health(bot_t *bot,
        void (*f)(bot_t *, float, vint32_t, float));

void register_play_clientbound_respawn(bot_t *bot,
        void (*f)(bot_t *, int32_t, uint8_t, uint8_t, char *));

void register_play_clientbound_position(bot_t *bot,
        void (*f)(bot_t *, double, double, double, float, float, int8_t));

void register_play_clientbound_item_change(bot_t *bot,
        void (*f)(bot_t *, int8_t));

void register_play_clientbound_use_bed(bot_t *bot,
        void (*f)(bot_t *, vint32_t, position_t));

void register_play_clientbound_animation(bot_t *bot,
        void (*f)(bot_t *, vint32_t, uint8_t));

void register_play_clientbound_spawn_player(bot_t *bot,
        void (*f)(bot_t *, vint32_t, 
            __uint128_t, 
            int32_t, 
            int32_t, 
            int32_t, 
            int8_t, 
            int8_t, 
            int16_t,
            metadata_t));

void register_play_clientbound_collect(bot_t *bot,
        void (*f)(bot_t *, vint32_t, vint32_t));

void register_play_clientbound_spawn_object(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int32_t,
            int32_t,
            int32_t,
            int8_t,
            int8_t,
            data_t));

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
            metadata_t));

void register_play_clientbound_spawn_painting(bot_t *bot,
        void (*f)(bot_t *, vint32_t, char *, position_t, uint8_t));

void register_play_clientbound_spawn_xp(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int32_t, int32_t, int32_t, int16_t));

void register_play_clientbound_entity_velocity(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int16_t, int16_t, int16_t));

void register_play_clientbound_entity_destroy_entities(bot_t *bot,
        void (*f)(bot_t *, vint32_t, vint32_t));

void register_play_clientbound_entity(bot_t *bot,
        void (*f)(bot_t *, vint32_t));

void register_play_clientbound_entity_move(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            int8_t,
            bool));

void register_play_clientbound_entity_look(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            bool,
            int8_t));

void register_play_clientbound_entity_look_move(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            int8_t,
            int8_t,
            int8_t,
            bool));

void register_play_clientbound_entity_teleport(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int32_t,
            int32_t,
            int32_t,
            int8_t,
            int8_t,
            bool));

void register_play_clientbound_entity_head_look(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int8_t));

void register_play_clientbound_entity_status(bot_t *bot,
        void (*f)(bot_t *, int32_t, int8_t));

void register_play_clientbound_entity_attach(bot_t *bot,
        void (*f)(bot_t *, int32_t, int32_t, bool));

void register_play_clientbound_entity_effect(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int8_t,
            vint32_t,
            bool));

void register_play_clientbound_entity_clear_effect(bot_t *bot,
        void (*f)(bot_t *, vint32_t, int8_t));

void register_play_clientbound_set_xp(bot_t *bot,
        void (*f)(bot_t *, float, int32_t, int32_t));

void register_play_clientbound_entity_properties(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int32_t,
            property_t *));

void register_play_clientbound_chunk_data(bot_t *bot,
        void (*f)(bot_t *, int32_t,
            int32_t,
            bool,
            uint16_t,
            vint32_t,
            int8_t *));

void register_play_clientbound_multi_block_change(bot_t *bot,
        void (*f)(bot_t *, int32_t, int32_t, vint32_t, record_t *));

void register_play_clientbound_block_change(bot_t *bot,
        void (*f)(bot_t *, position_t, vint32_t));

void register_play_clientbound_block_action(bot_t *bot,
        void (*f)(bot_t *, position_t, uint8_t, uint8_t, vint32_t));

void register_play_clientbound_block_break_animation(bot_t *bot,
        void (*f)(bot_t *, vint32_t, position_t, int8_t));

void register_play_clientbound_chunk_bulk(bot_t *bot,
        void (*f)(bot_t *, bool,
            vint32_t,
            int32_t,
            int32_t,
            uint16_t,
            int8_t *));

void register_play_clientbound_explosion(bot_t *bot,
        void (*f)(bot_t *, float,
            float,
            float,
            float,
            int32_t,
            record_t *,
            float,
            float,
            float));

void register_play_clientbound_effect(bot_t *bot,
        void (*f)(bot_t *, int32_t, position_t, int32_t, bool));

void register_play_clientbound_sound_effect(bot_t *bot,
        void (*f)(bot_t *, char *,
            int32_t,
            int32_t,
            int32_t,
            float,
            uint8_t));

void register_play_clientbound_entity_spawn_global(bot_t *bot,
        void (*f)(bot_t *, vint32_t,
            int8_t,
            int32_t,
            int32_t,
            int32_t));

void register_play_clientbound_update_sign(bot_t *bot,
        void (*f)(bot_t *, position_t,
            chat_t,
            chat_t,
            chat_t,
            chat_t));

void register_play_clientbound_plugin_message(bot_t *bot,
        void (*f)(bot_t *, char *, int8_t *));

void register_play_clientbound_plugin_disconnect(bot_t *bot,
        void (*f)(bot_t *, char *));

void register_play_clientbound_plugin_difficulty(bot_t *bot,
        void (*f)(bot_t *, char *));

void register_play_clientbound_set_compression(bot_t *bot,
        void (*f)(bot_t *, vint32_t));
