#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include "bot.h"

typedef uint8_t bool;
typedef uint64_t position_t;
typedef uint32_t vint32_t;
typedef void* chat_t;
typedef void* data_t;
typedef void* meta_t;
typedef void* metadata_t;
typedef void* property_t;
typedef void* record_t;
typedef void* slot_t;

/**
 **
 ** Struct declarations
 **
 **/

/*
 * Handshaking serverbound structs
 */

typedef struct handshaking_serverbound_handshake {
    char*         format;
    vint32_t      packet_id;

    vint32_t      protocol_version;
    char*         server_addr;
    uint16_t      server_port;
    vint32_t      next_state;
} handshaking_serverbound_handshake_t;

/*
 * Login clientbound structs
 */

typedef struct login_clientbound_disconnect {
    char*         format;
    vint32_t      packet_id;

    char*         json;
} login_clientbound_disconnect_t;

typedef struct login_clientbound_success {
    char*         format;
    vint32_t      packet_id;

    char*         uuid;
    char*         username;
} login_clientbound_success_t;

typedef struct login_clientbound_set_compression {
    char*         format;
    vint32_t      packet_id;

    vint32_t      threshold;
} login_clientbound_set_compression_t;

/*
 * Login serverbound structs
 */

typedef struct login_serverbound_login {
    char*         format;
    vint32_t      packet_id;

    char*         username;
} login_serverbound_login_t;

/*
 * Status clientbound structs
 */

typedef struct status_clientbound_response {
    char*         format;
    vint32_t      packet_id;

    char*         json;
} status_clientbound_response_t;

typedef struct status_clientbound_ping {
    char*         format;
    vint32_t      packet_id;

    int64_t      time;
} status_clientbound_ping_t;

/*
 * Status serverbound structs
 */

typedef struct status_serverbound_request {
    char*         format;
    vint32_t      packet_id;
} status_serverbound_request_t;

typedef struct status_serverbound_ping {
    char*         format;
    vint32_t      packet_id;

    int64_t      time;
} status_serverbound_ping_t;

/*
 * Play clientbound structs
 */

typedef struct play_clientbound_keepalive {
    char*         format;
    vint32_t      packet_id;

    vint32_t      keepalive_id;
} play_clientbound_keepalive_t;

typedef struct play_clientbound_join_game {
    char*         format;
    vint32_t      packet_id;

    int32_t       entity_id;
    uint8_t       gamemode;
    int8_t        dimension;
    uint8_t       difficulty;
    uint8_t       max_players;
    char*         level_type;
} play_clientbound_join_game_t;

typedef struct play_clientbound_chat {
    char*         format;
    vint32_t      packet_id;

    char*         json;
    int8_t        position;
} play_clientbound_chat_t;

typedef struct play_clientbound_time_update {
    char*         format;
    vint32_t      packet_id;

    int64_t       age;
    int64_t       time;
} play_clientbound_time_update_t;

typedef struct play_clientbound_entity_equipment {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int16_t       slot;
    slot_t        item;
} play_clientbound_entity_equipment_t;

typedef struct play_clientbound_spawn_position {
    char*         format;
    vint32_t      packet_id;

    position_t    location;
} play_clientbound_spawn_position_t;

typedef struct play_clientbound_update_health {
    char*         format;
    vint32_t      packet_id;

    float         health;
    vint32_t      food;
    float         saturation;
} play_clientbound_update_health_t;

typedef struct play_clientbound_respawn {
    char*         format;
    vint32_t      packet_id;

    int32_t       dimension;
    uint8_t       difficulty;
    uint8_t       gamemode;
    char*         level_type;
} play_clientbound_respawn_t;

typedef struct play_clientbound_position {
    char*         format;
    vint32_t      packet_id;

    double        x;
    double        y;
    double        z;
    float         yaw;
    float         pitch;
    int8_t        flags;
} play_clientbound_position_t;

typedef struct play_clientbound_item_change {
    char*         format;
    vint32_t      packet_id;

    int8_t        slot;
} play_clientbound_item_change_t;

typedef struct play_clientbound_use_bed {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    position_t    location;
} play_clientbound_use_bed_t;

typedef struct play_clientbound_animation {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    uint8_t      animation;
} play_clientbound_animation_t;

typedef struct play_clientbound_spawn_player {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    __uint128_t   uuid;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    int16_t       item;
    metadata_t    metadata;
} play_clientbound_spawn_player_t;

typedef struct play_clientbound_collect {
    char*         format;
    vint32_t      packet_id;

    vint32_t      collected_entity_id;
    vint32_t      collector_entity_id;
} play_clientbound_collect_t;

typedef struct play_clientbound_spawn_object {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        type;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    data_t        data;
} play_clientbound_spawn_object_t;

typedef struct play_clientbound_spawn_mob {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    uint8_t       type;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    int8_t        head_pitch;
    int16_t       dx;
    int16_t       dy;
    int16_t       dz;
    metadata_t    metadata;
} play_clientbound_spawn_mob_t;

typedef struct play_clientbound_spawn_painting {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    char*         title;
    position_t    location;
    uint8_t       direction;
} play_clientbound_spawn_painting_t;

typedef struct play_clientbound_spawn_xp {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int16_t       count;
} play_clientbound_spawn_xp_t;

typedef struct play_clientbound_entity_velocity {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int16_t       dx;
    int16_t       dy;
    int16_t       dz;
} play_clientbound_entity_velocity_t;

typedef struct play_clientbound_entity_destroy_entities {
    char*         format;
    vint32_t      packet_id;

    int32_t       count;
    vint32_t*     entity_ids;
} play_clientbound_entity_destroy_entities_t;

typedef struct play_clientbound_entity {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
} play_clientbound_entity_t;

typedef struct play_clientbound_entity_move {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        dx;
    int8_t        dy;
    int8_t        dz;
    bool          on_ground;
} play_clientbound_entity_move_t;

typedef struct play_clientbound_entity_look {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
} play_clientbound_entity_look_t;

typedef struct play_clientbound_entity_look_move {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        dx;
    int8_t        dy;
    int8_t        dz;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
} play_clientbound_entity_look_move_t;

typedef struct play_clientbound_entity_teleport {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
} play_clientbound_entity_teleport_t;

typedef struct play_clientbound_entity_head_look {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        yaw;
} play_clientbound_entity_head_look_t;

typedef struct play_clientbound_entity_status {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        status;
} play_clientbound_entity_status_t;

typedef struct play_clientbound_entity_attach {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int32_t       vehicle_id;
    bool          leash;
} play_clientbound_entity_attach_t;

typedef struct play_clientbound_entity_effect {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        effect_id;
    int8_t        amplifier;
    vint32_t      duration;
    bool          hide;
} play_clientbound_entity_effect_t;

typedef struct play_clientbound_entity_clear_effect {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int32_t       effect_id;
} play_clientbound_entity_clear_effect_t;

typedef struct play_clientbound_entity_properties {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int32_t       count;
    property_t*   properties;
} play_clientbound_entity_properties_t;

typedef struct play_clientbound_set_xp {
    char*         format;
    vint32_t      packet_id;

    float         xp_bar;
    int32_t       level;
    int32_t       xp;
} play_clientbound_set_xp_t;

typedef struct play_clientbound_chunk_data {
    char*         format;
    vint32_t      packet_id;

    int32_t       chunk_x;
    int32_t       chunk_z;
    bool          continuous;
    uint16_t      bitmap;
    vint32_t      size;
    int8_t*       data;
} play_clientbound_chunk_data_t;

typedef struct play_clientbound_multi_block_change {
    char*         format;
    vint32_t      packet_id;

    int32_t       chunk_x;
    int32_t       chunk_z;
    vint32_t      count;
    record_t*     records;
} play_clientbound_multi_block_change_t;

typedef struct play_clientbound_block_change {
    char*         format;
    vint32_t      packet_id;

    position_t    location;
    vint32_t      block_id;
} play_clientbound_block_change_t;

typedef struct play_clientbound_block_action {
    char*         format;
    vint32_t      packet_id;

    position_t    location;
    uint8_t       byte1;
    uint8_t       byte2;
    vint32_t      type;
} play_clientbound_block_action_t;

typedef struct play_clientbound_block_break_animation {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    position_t    location;
    int8_t        stage;
} play_clientbound_block_break_animation_t;

typedef struct play_clientbound_chunk_bulk {
    char*         format;
    vint32_t      packet_id;

    bool          skylight;
    vint32_t      column_count;
    meta_t        meta;
    int8_t*       data;
} play_clientbound_chunk_bulk_t;

typedef struct play_clientbound_explosion {
    char*         format;
    vint32_t      packet_id;

    float         x;
    float         y;
    float         z;
    float         radius;
    int32_t       count;
    record_t*     records;
    float         dx;
    float         dy;
    float         dz;
} play_clientbound_explosion_t;

typedef struct play_clientbound_effect {
    char*         format;
    vint32_t      packet_id;

    vint32_t      effect_id;
    position_t    location;
    int32_t       data;
    bool          relative;
} play_clientbound_effect_t;

typedef struct play_clientbound_sound_effect {
    char*         format;
    vint32_t      packet_id;

    char*         sound_name;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    float         volume;
    uint8_t       pitch;
} play_clientbound_sound_effect_t;

typedef struct play_clientbound_particle {
    char*         format;
    vint32_t      packet_id;

    vint32_t      particle_id;
    bool          long_distance;
    float         x;
    float         y;
    float         z;
    float         dx;
    float         dy;
    float         dz;
    float         particle_data;
    int32_t       count;
    vint32_t*     data;
} play_clientbound_particle_t;

typedef struct play_clientbound_entity_spawn_global {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    int8_t        type;
    int32_t       x;
    int32_t       y;
    int32_t       z;
} play_clientbound_entity_spawn_global_t;

typedef struct play_clientbound_update_sign {
    char*         format;
    vint32_t      packet_id;

    position_t    location;
    chat_t        line1;
    chat_t        line2;
    chat_t        line3;
    chat_t        line4;
} play_clientbound_update_sign_t;

typedef struct play_clientbound_plugin_message {
    char*         format;
    vint32_t      packet_id;

    char*         channel;
    int8_t*       data;
} play_clientbound_plugin_message_t;

typedef struct play_clientbound_plugin_disconnect {
    char*         format;
    vint32_t      packet_id;

    char*         reason;
} play_clientbound_plugin_disconnect_t;

typedef struct play_clientbound_plugin_difficulty {
    char*         format;
    vint32_t      packet_id;

    uint8_t       difficulty;
} play_clientbound_plugin_difficulty_t;

typedef struct play_clientbound_set_compression {
    char*         format;
    vint32_t      packet_id;

    vint32_t      threshold;
} play_clientbound_set_compression_t;

/*
 * Play serverbound structs
 */

typedef struct play_serverbound_keepalive {
    char*         format;
    vint32_t      packet_id;

    vint32_t      keepalive_id;
} play_serverbound_keepalive_t;

typedef struct play_serverbound_chat {
    char*         format;
    vint32_t      packet_id;

    char*         message;
} play_serverbound_chat_t;

typedef struct play_serverbound_entity_use {
    char*         format;
    vint32_t      packet_id;

    vint32_t      target;
    vint32_t      type;
    float         x;
    float         y;
    float         z;
} play_serverbound_entity_use_t;

typedef struct play_serverbound_player {
    char*         format;
    vint32_t      packet_id;

    bool          on_ground;
} play_serverbound_player_t;

typedef struct play_serverbound_player_move {
    char*         format;
    vint32_t      packet_id;

    double        x;
    double        y;
    double        z;
    bool          on_ground;
} play_serverbound_player_move_t;

typedef struct play_serverbound_player_look {
    char*         format;
    vint32_t      packet_id;

    float         yaw;
    float         pitch;
    bool          on_ground;
} play_serverbound_player_look_t;

typedef struct play_serverbound_player_move_look {
    char*         format;
    vint32_t      packet_id;

    double        x;
    double        y;
    double        z;
    float         yaw;
    float         pitch;
    bool          on_ground;
} play_serverbound_player_move_look_t;

typedef struct play_serverbound_player_dig {
    char*         format;
    vint32_t      packet_id;

    int8_t        status;
    position_t    location;
    int8_t        face;
} play_serverbound_player_dig_t;

typedef struct play_serverbound_player_block_place {
    char*         format;
    vint32_t      packet_id;

    position_t    location;
    int8_t        direction;
    slot_t        item;
    int8_t        x;
    int8_t        y;
    int8_t        z;
} play_serverbound_player_block_place_t;

typedef struct play_serverbound_item_change {
    char*         format;
    vint32_t      packet_id;

    int16_t       slot;
} play_serverbound_item_change_t;

typedef struct play_serverbound_animation {
    char*         format;
    vint32_t      packet_id;
} play_serverbound_animation_t;

typedef struct play_serverbound_entity_action {
    char*         format;
    vint32_t      packet_id;

    vint32_t      entity_id;
    uint8_t       action_id;
    vint32_t      jump_boost;
} play_serverbound_entity_action_t;

typedef struct play_serverbound_steer_vehicle {
    char*         format;
    vint32_t      packet_id;

    float         sideways;
    float         forward;
    uint8_t       flags;
} play_serverbound_steer_vehicle_t;

typedef struct play_serverbound_update_sign {
    char*         format;
    vint32_t      packet_id;

    position_t    location;
    chat_t        line1;
    chat_t        line2;
    chat_t        line3;
    chat_t        line4;
} play_serverbound_update_sign_t;

typedef struct play_serverbound_player_abilities {
    char*         format;
    vint32_t      packet_id;

    int8_t        flags;
    float         flying_speed;
    float         walking_speed;
} play_serverbound_player_abilities_t;

typedef struct play_serverbound_client_settings {
    char*         format;
    vint32_t      packet_id;

    char*         locale;
    int8_t        view_distance;
    int8_t        chat_flags;
    bool          chat_colors;
    uint8_t       skin;
} play_serverbound_client_settings_t;

typedef struct play_serverbound_player_status {
    char*         format;
    vint32_t      packet_id;

    vint32_t      action_id;
} play_serverbound_player_status_t;

typedef struct play_serverbound_plugin_message {
    char*         format;
    vint32_t      packet_id;

    char*         channel;
    int8_t*       data;
} play_serverbound_plugin_message_t;

typedef struct play_serverbound_spectate {
    char*         format;
    vint32_t      packet_id;

    __uint128_t   target;
} play_serverbound_spectate_t;


/**
 **
 ** Function prototypes
 **
 **/


/*
 * Handshaking serverbound functions
 */

int32_t send_handshaking_serverbound_handshake(
    bot_t         bot,
    vint32_t      protocol_version,
    char*         server_addr,
    uint16_t      server_port,
    vint32_t      next_state
);

/*
 * Login serverbound functions
 */

int32_t send_login_serverbound_login(
    bot_t         bot,
    char*         username
);

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
    bot_t         bot
);

int32_t send_status_clientbound_ping(
    bot_t         bot,
    int64_t       time
);

/*
 * Play serverbound functions
 */

int32_t send_play_serverbound_keepalive(
    bot_t         bot,
    vint32_t      keepalive_id
);

int32_t send_play_serverbound_chat(
    bot_t         bot,
    char*         message
);

int32_t send_play_serverbound_entity_use(
    bot_t         bot,
    vint32_t      target,
    vint32_t      type,
    float         x,
    float         y,
    float         z
);

int32_t send_play_serverbound_player(
    bot_t         bot,
    bool          on_ground
);

int32_t send_play_serverbound_player_move(
    bot_t         bot,
    double        x,
    double        y,
    double        z,
    bool          on_ground
);

int32_t send_play_serverbound_player_look(
    bot_t         bot,
    float         yaw,
    float         pitch,
    bool          on_ground
);

int32_t send_play_serverbound_player_move_look(
    bot_t         bot,
    double        x,
    double        y,
    double        z,
    float         yaw,
    float         pitch,
    bool          on_ground
);

int32_t send_play_serverbound_player_dig(
    bot_t         bot,
    int8_t        status,
    position_t    location,
    int8_t        face
);

int32_t send_play_serverbound_player_block_place(
    bot_t         bot,
    position_t    location,
    int8_t        direction,
    slot_t        item,
    int8_t        x,
    int8_t        y,
    int8_t        z
);

int32_t send_play_serverbound_item_change(
    bot_t         bot,
    int16_t       slot
);

int32_t send_play_serverbound_animation(
    bot_t         bot
);

int32_t send_play_serverbound_entity_action(
    bot_t         bot,
    vint32_t      entity_id,
    uint8_t       action_id,
    vint32_t      jump_boost
);

int32_t send_play_serverbound_steer_vehicle(
    bot_t         bot,
    float         sideways,
    float         forward,
    uint8_t       flags
);

int32_t send_play_serverbound_update_sign(
    bot_t         bot,
    position_t    location,
    chat_t        line1,
    chat_t        line2,
    chat_t        line3,
    chat_t        line4
);

int32_t send_play_serverbound_player_abilities(
    bot_t         bot,
    int8_t        flags,
    float         flying_speed,
    float         walking_speed
);

int32_t send_play_serverbound_client_settings(
    bot_t         bot,
    char*         locale,
    int8_t        view_distance,
    int8_t        chat_flags,
    bool          chat_colors,
    uint8_t       skin
);

int32_t send_play_serverbound_player_status(
    bot_t         bot,
    vint32_t      action_id
);

int32_t send_play_serverbound_plugin_message(
    bot_t         bot,
    char*         channel,
    int8_t*       data
);

int32_t send_play_serverbound_spectate(
    bot_t         bot,
    __uint128_t   target
);

/*
 * Login clientbound structs
 */

login_clientbound_disconnect_t*
recv_login_clientbound_disconnect(bot_t bot, void *packet);

login_clientbound_success_t*
recv_login_clientbound_success(bot_t bot, void *packet);

login_clientbound_set_compression_t*
recv_login_clientbound_set_compression(bot_t bot, void *packet);

/*
 * Status clientbound structs
 */

status_clientbound_response_t*
recv_status_clientbound_response(bot_t bot, void *packet);

status_clientbound_ping_t*
recv_status_clientbound_ping(bot_t bot, void *packet);

/*
 * Play clientbound structs
 */

play_clientbound_keepalive_t*
recv_play_clientbound_keepalive(bot_t bot, void *packet);

play_clientbound_join_game_t*
recv_play_clientbound_join_game(bot_t bot, void *packet);

play_clientbound_chat_t*
recv_play_clientbound_chat(bot_t bot, void *packet);

play_clientbound_time_update_t*
recv_play_clientbound_time_update(bot_t bot, void *packet);

play_clientbound_entity_equipment_t*
recv_play_clientbound_entity_equipment(bot_t bot, void *packet);

play_clientbound_spawn_position_t*
recv_play_clientbound_spawn_position(bot_t bot, void *packet);

play_clientbound_update_health_t*
recv_play_clientbound_update_health(bot_t bot, void *packet);

play_clientbound_respawn_t*
recv_play_clientbound_respawn(bot_t bot, void *packet);

play_clientbound_position_t*
recv_play_clientbound_position(bot_t bot, void *packet);

play_clientbound_item_change_t*
recv_play_clientbound_item_change(bot_t bot, void *packet);

play_clientbound_use_bed_t*
recv_play_clientbound_use_bed(bot_t bot, void *packet);

play_clientbound_animation_t*
recv_play_clientbound_animation(bot_t bot, void *packet);

play_clientbound_spawn_player_t*
recv_play_clientbound_spawn_player(bot_t bot, void *packet);

play_clientbound_collect_t*
recv_play_clientbound_collect(bot_t bot, void *packet);

play_clientbound_spawn_object_t*
recv_play_clientbound_spawn_object(bot_t bot, void *packet);

play_clientbound_spawn_mob_t*
recv_play_clientbound_spawn_mob(bot_t bot, void *packet);

play_clientbound_spawn_painting_t*
recv_play_clientbound_spawn_painting(bot_t bot, void *packet);

play_clientbound_spawn_xp_t*
recv_play_clientbound_spawn_xp(bot_t bot, void *packet);

play_clientbound_entity_velocity_t*
recv_play_clientbound_entity_velocity(bot_t bot, void *packet);

play_clientbound_entity_destroy_entities_t*
recv_play_clientbound_entity_destroy_entities(bot_t bot, void *packet);

play_clientbound_entity_t*
recv_play_clientbound_entity(bot_t bot, void *packet);

play_clientbound_entity_move_t*
recv_play_clientbound_entity_move(bot_t bot, void *packet);

play_clientbound_entity_look_t*
recv_play_clientbound_entity_look(bot_t bot, void *packet);

play_clientbound_entity_look_move_t*
recv_play_clientbound_entity_look_move(bot_t bot, void *packet);

play_clientbound_entity_teleport_t*
recv_play_clientbound_entity_teleport(bot_t bot, void *packet);

play_clientbound_entity_head_look_t*
recv_play_clientbound_entity_head_look(bot_t bot, void *packet);

play_clientbound_entity_status_t*
recv_play_clientbound_entity_status(bot_t bot, void *packet);

play_clientbound_entity_attach_t*
recv_play_clientbound_entity_attach(bot_t bot, void *packet);

play_clientbound_entity_effect_t*
recv_play_clientbound_entity_effect(bot_t bot, void *packet);

play_clientbound_entity_clear_effect_t*
recv_play_clientbound_entity_clear_effect(bot_t bot, void *packet);

play_clientbound_entity_properties_t*
recv_play_clientbound_entity_properties(bot_t bot, void *packet);

play_clientbound_set_xp_t*
recv_play_clientbound_set_xp(bot_t bot, void *packet);

play_clientbound_chunk_data_t*
recv_play_clientbound_chunk_data(bot_t bot, void *packet);

play_clientbound_multi_block_change_t*
recv_play_clientbound_multi_block_change(bot_t bot, void *packet);

play_clientbound_block_change_t*
recv_play_clientbound_block_change(bot_t bot, void *packet);

play_clientbound_block_action_t*
recv_play_clientbound_block_action(bot_t bot, void *packet);

play_clientbound_block_break_animation_t*
recv_play_clientbound_block_break_animation(bot_t bot, void *packet);

play_clientbound_chunk_bulk_t*
recv_play_clientbound_chunk_bulk(bot_t bot, void *packet);

play_clientbound_explosion_t*
recv_play_clientbound_explosion(bot_t bot, void *packet);

play_clientbound_effect_t*
recv_play_clientbound_effect(bot_t bot, void *packet);

play_clientbound_sound_effect_t*
recv_play_clientbound_sound_effect(bot_t bot, void *packet);

play_clientbound_particle_t*
recv_play_clientbound_particle(bot_t bot, void *packet);

play_clientbound_entity_spawn_global_t*
recv_play_clientbound_entity_spawn_global(bot_t bot, void *packet);

play_clientbound_update_sign_t*
recv_play_clientbound_update_sign(bot_t bot, void *packet);

play_clientbound_plugin_message_t*
recv_play_clientbound_plugin_message(bot_t bot, void *packet);

play_clientbound_plugin_disconnect_t*
recv_play_clientbound_plugin_disconnect(bot_t bot, void *packet);

play_clientbound_plugin_difficulty_t*
recv_play_clientbound_plugin_difficulty(bot_t bot, void *packet);

play_clientbound_set_compression_t*
recv_play_clientbound_set_compression(bot_t bot, void *packet);

#endif /* PROTOCOL_H */
