#include <stdint.h>

typedef uint8_t bool;
typedef uint64_t position_t;
typedef uint64_t vint32_t;
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

typedef struct handshaking_serverbound_handshake_t {
    char*         format;
    int           packet_id;

    vint32_t      protocol_version;
    char*         server_addr;
    uint16_t      server_port;
    vint32_t      next_state;
} handshaking_serverbound_handshake;

/*
 * Login clientbound structs
 */

typedef struct login_clientbound_disconnect_t {
    char*         format;
    int           packet_id;

    char*         json;
} login_clientbound_disconnect;

typedef struct login_clientbound_success_t {
    char*         format;
    int           packet_id;

    char*         uuid;
    char*         username;
} login_clientbound_success;

typedef struct login_clientbound_set_compression_t {
    char*         format;
    int           packet_id;

    vint32_t      threshold;
} login_clientbound_set_compression;

/*
 * Login serverbound structs
 */

typedef struct login_serverbound_login_t {
    char*         format;
    int           packet_id;

    char*         username;
} login_serverbound_login;

/*
 * Status clientbound structs
 */

typedef struct status_clientbound_response_t {
    char*         format;
    int           packet_id;

    char*         json;
} status_clientbound_response;

typedef struct status_clientbound_ping_t {
    char*         format;
    int           packet_id;

    int64_t      time;
} status_clientbound_ping;

/*
 * Status serverbound structs
 */

typedef struct status_serverbound_request_t {
    char*         format;
    int           packet_id;
} status_serverbound_request;

typedef struct status_serverbound_ping_t {
    char*         format;
    int           packet_id;

    int64_t      time;
} status_serverbound_ping;

/*
 * Play clientbound structs
 */

typedef struct play_clientbound_keepalive_t {
    char*         format;
    int           packet_id;

    vint32_t      keepalive_id;
} play_clientbound_keepalive;

typedef struct play_clientbound_join_game_t {
    char*         format;
    int           packet_id;

    int32_t       entity_id;
    uint8_t       gamemode;
    int8_t        dimension;
    uint8_t       difficulty;
    uint8_t       max_players;
    char*         level_type;
} play_clientbound_join_game;

typedef struct play_clientbound_chat_t {
    char*         format;
    int           packet_id;

    char*         json;
    int8_t        position;
} play_clientbound_chat;

typedef struct play_clientbound_time_update_t {
    char*         format;
    int           packet_id;

    int64_t       age;
    int64_t       time;
} play_clientbound_time_update;

typedef struct play_clientbound_entity_equipment_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int16_t       slot;
    slot_t        item;
} play_clientbound_entity_equipment;

typedef struct play_clientbound_spawn_position_t {
    char*         format;
    int           packet_id;

    position_t    location;
} play_clientbound_spawn_position;

typedef struct play_clientbound_update_health_t {
    char*         format;
    int           packet_id;

    float         health;
    vint32_t      food;
    float         saturation;
} play_clientbound_update_health;

typedef struct play_clientbound_respawn_t {
    char*         format;
    int           packet_id;

    int32_t       dimension;
    uint8_t       difficulty;
    uint8_t       gamemode;
    char*         level_type;
} play_clientbound_respawn;

typedef struct play_clientbound_position_t {
    char*         format;
    int           packet_id;

    double        x;
    double        y;
    double        z;
    float         yaw;
    float         pitch;
    int8_t        flags;
} play_clientbound_position;

typedef struct play_clientbound_item_change_t {
    char*         format;
    int           packet_id;

    int8_t        slot;
} play_clientbound_item_change;

typedef struct play_clientbound_use_bed_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    position_t    location;
} play_clientbound_use_bed;

typedef struct play_clientbound_animation_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    uint8_t      animation;
} play_clientbound_animation;

typedef struct play_clientbound_spawn_player_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    __uint128_t   uuid;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    int16_t       item;
    metadata_t    metadata;
} play_clientbound_spawn_player;

typedef struct play_clientbound_collect_t {
    char*         format;
    int           packet_id;

    vint32_t      collected_entity_id;
    vint32_t      collector_entity_id;
} play_clientbound_collect;

typedef struct play_clientbound_spawn_object_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        type;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    data_t        data;
} play_clientbound_spawn_object;

typedef struct play_clientbound_spawn_mob_t {
    char*         format;
    int           packet_id;

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
} play_clientbound_spawn_mob;

typedef struct play_clientbound_spawn_painting_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    char*         title;
    position_t    location;
    uint8_t       direction;
} play_clientbound_spawn_painting;

typedef struct play_clientbound_spawn_xp_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int16_t       count;
} play_clientbound_spawn_xp;

typedef struct play_clientbound_entity_velocity_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int16_t       dx;
    int16_t       dy;
    int16_t       dz;
} play_clientbound_entity_velocity;

typedef struct play_clientbound_entity_destroy_entities_t {
    char*         format;
    int           packet_id;

    int32_t       count;
    vint32_t*     entity_ids;
} play_clientbound_entity_destroy_entities;

typedef struct play_clientbound_entity_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
} play_clientbound_entity;

typedef struct play_clientbound_entity_move_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        dx;
    int8_t        dy;
    int8_t        dz;
    bool          on_ground;
} play_clientbound_entity_move;

typedef struct play_clientbound_entity_look_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
} play_clientbound_entity_look;

typedef struct play_clientbound_entity_look_move_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        dx;
    int8_t        dy;
    int8_t        dz;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
} play_clientbound_entity_look_move;

typedef struct play_clientbound_entity_teleport_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
} play_clientbound_entity_teleport;

typedef struct play_clientbound_entity_head_look_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        yaw;
} play_clientbound_entity_head_look;

typedef struct play_clientbound_entity_status_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        status;
} play_clientbound_entity_status;

typedef struct play_clientbound_entity_attach_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int32_t       vehicle_id;
    bool          leash;
} play_clientbound_entity_attach;

typedef struct play_clientbound_entity_effect_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        effect_id;
    int8_t        amplifier;
    vint32_t      duration;
    bool          hide;
} play_clientbound_entity_effect;

typedef struct play_clientbound_entity_clear_effect_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int32_t       effect_id;
} play_clientbound_entity_clear_effect;

typedef struct play_clientbound_entity_properties_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int32_t       count;
    property_t*   properties;
} play_clientbound_entity_properties;

typedef struct play_clientbound_set_xp_t {
    char*         format;
    int           packet_id;

    float         xp_bar;
    int32_t       level;
    int32_t       xp;
} play_clientbound_set_xp;

typedef struct play_clientbound_chunk_data_t {
    char*         format;
    int           packet_id;

    int32_t       chunk_x;
    int32_t       chunk_z;
    bool          continuous;
    uint16_t      bitmap;
    vint32_t      size;
    int8_t*       data;
} play_clientbound_chunk_data;

typedef struct play_clientbound_multi_block_change_t {
    char*         format;
    int           packet_id;

    int32_t       chunk_x;
    int32_t       chunk_z;
    vint32_t      count;
    record_t*     records;
} play_clientbound_multi_block_change;

typedef struct play_clientbound_block_change_t {
    char*         format;
    int           packet_id;

    position_t    location;
    vint32_t      block_id;
} play_clientbound_block_change;

typedef struct play_clientbound_block_action_t {
    char*         format;
    int           packet_id;

    position_t    location;
    uint8_t       byte1;
    uint8_t       byte2;
    vint32_t      type;
} play_clientbound_block_action;

typedef struct play_clientbound_block_break_animation_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    position_t    location;
    int8_t        stage;
} play_clientbound_block_break_animation;

typedef struct play_clientbound_chunk_bulk_t {
    char*         format;
    int           packet_id;

    bool          skylight;
    vint32_t      column_count;
    meta_t        meta;
    int8_t*       data;
} play_clientbound_chunk_bulk;

typedef struct play_clientbound_explosion_t {
    char*         format;
    int           packet_id;

    float         x;
    float         y;
    float         z;
    float         radius;
    int32_t       count;
    record_t*     records;
    float         dx;
    float         dy;
    float         dz;
} play_clientbound_explosion;

typedef struct play_clientbound_effect_t {
    char*         format;
    int           packet_id;

    vint32_t      effect_id;
    position_t    location;
    int32_t       data;
    bool          relative;
} play_clientbound_effect;

typedef struct play_clientbound_sound_effect_t {
    char*         format;
    int           packet_id;

    char*         sound_name;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    float         volume;
    uint8_t       pitch;
} play_clientbound_sound_effect;

typedef struct play_clientbound_particle_t {
    char*         format;
    int           packet_id;

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
} play_clientbound_particle;

typedef struct play_clientbound_entity_spawn_global_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    int8_t        type;
    int32_t       x;
    int32_t       y;
    int32_t       z;
} play_clientbound_entity_spawn_global;

typedef struct play_clientbound_update_sign_t {
    char*         format;
    int           packet_id;

    position_t    location;
    chat_t        line1;
    chat_t        line2;
    chat_t        line3;
    chat_t        line4;
} play_clientbound_update_sign;

typedef struct play_clientbound_plugin_message_t {
    char*         format;
    int           packet_id;

    char*         channel;
    int8_t*       data;
} play_clientbound_plugin_message;

typedef struct play_clientbound_plugin_disconnect_t {
    char*         format;
    int           packet_id;

    char*         reason;
} play_clientbound_plugin_disconnect;

typedef struct play_clientbound_plugin_difficulty_t {
    char*         format;
    int           packet_id;

    uint8_t       difficulty;
} play_clientbound_plugin_difficulty;

typedef struct play_clientbound_set_compression_t {
    char*         format;
    int           packet_id;

    vint32_t      threshold;
} play_clientbound_set_compression;

/*
 * Play serverbound structs
 */

typedef struct play_serverbound_keepalive_t {
    char*         format;
    int           packet_id;

    vint32_t      keepalive_id;
} play_serverbound_keepalive;

typedef struct play_serverbound_chat_t {
    char*         format;
    int           packet_id;

    char*         message;
} play_serverbound_chat;

typedef struct play_serverbound_entity_use_t {
    char*         format;
    int           packet_id;

    vint32_t      target;
    vint32_t      type;
    float         x;
    float         y;
    float         z;
} play_serverbound_entity_use;

typedef struct play_serverbound_player_t {
    char*         format;
    int           packet_id;

    bool          on_ground;
} play_serverbound_player;

typedef struct play_serverbound_player_move_t {
    char*         format;
    int           packet_id;

    double        x;
    double        y;
    double        z;
    bool          on_ground;
} play_serverbound_player_move;

typedef struct play_serverbound_player_look_t {
    char*         format;
    int           packet_id;

    float         yaw;
    float         pitch;
    bool          on_ground;
} play_serverbound_player_look;

typedef struct play_serverbound_player_move_look_t {
    char*         format;
    int           packet_id;

    double        x;
    double        y;
    double        z;
    float         yaw;
    float         pitch;
    bool          on_ground;
} play_serverbound_player_move_look;

typedef struct play_serverbound_player_dig_t {
    char*         format;
    int           packet_id;

    int8_t        status;
    position_t    location;
    int8_t        face;
} play_serverbound_player_dig;

typedef struct play_serverbound_player_block_place_t {
    char*         format;
    int           packet_id;

    position_t    location;
    int8_t        direction;
    slot_t        item;
    int8_t        x;
    int8_t        y;
    int8_t        z;
} play_serverbound_player_block_place;

typedef struct play_serverbound_item_change_t {
    char*         format;
    int           packet_id;

    int16_t       slot;
} play_serverbound_item_change;

typedef struct play_serverbound_animation_t {
    char*         format;
    int           packet_id;
} play_serverbound_animation;

typedef struct play_serverbound_entity_action_t {
    char*         format;
    int           packet_id;

    vint32_t      entity_id;
    uint8_t       action_id;
    vint32_t      jump_boost;
} play_serverbound_entity_action;

typedef struct play_serverbound_steer_vehicle_t {
    char*         format;
    int           packet_id;

    float         sideways;
    float         forward;
    uint8_t       flags;
} play_serverbound_steer_vehicle;

typedef struct play_serverbound_update_sign_t {
    char*         format;
    int           packet_id;

    position_t    location;
    chat_t        line1;
    chat_t        line2;
    chat_t        line3;
    chat_t        line4;
} play_serverbound_update_sign;

typedef struct play_serverbound_player_abilities_t {
    char*         format;
    int           packet_id;

    int8_t        flags;
    float         flying_speed;
    float         walking_speed;
} play_serverbound_player_abilities;

typedef struct play_serverbound_client_settings_t {
    char*         format;
    int           packet_id;

    char*         locale;
    int8_t        view_distance;
    int8_t        chat_flags;
    bool          chat_colors;
    uint8_t       skin;
} play_serverbound_client_settings;

typedef struct play_serverbound_player_status_t {
    char*         format;
    int           packet_id;

    vint32_t      action_id;
} play_serverbound_player_status;

typedef struct play_serverbound_plugin_message_t {
    char*         format;
    int           packet_id;

    char*         channel;
    int8_t*       data;
} play_serverbound_plugin_message;

typedef struct play_serverbound_spectate_t {
    char*         format;
    int           packet_id;

    __uint128_t   target;
} play_serverbound_spectate;


/**
 **
 ** Function prototypes
 **
 **/

/* varint functions */

// returns bytes written to char array
int varint64(char *, int64_t *);
int uvarint64(char *, uint64_t *);


/*
 * Handshaking serverbound functions
 */

int32_t send_handshaking_serverbound_handshake(
    vint32_t      protocol_version,
    char*         server_addr,
    uint16_t      server_port,
    vint32_t      next_state
);

/*
 * Login serverbound functions
 */

int32_t send_login_serverbound_login(
    char*         username
);

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
    char*         format,
    int           packet_id
);

int32_t send_status_clientbound_ping(
    int64_t      time
);

/*
 * Play serverbound functions
 */

int32_t send_play_serverbound_keepalive(
    vint32_t      keepalive_id
);

int32_t send_play_serverbound_chat(
    char*         message
);

int32_t send_play_serverbound_entity_use(
    vint32_t      target,
    vint32_t      type,
    float         x,
    float         y,
    float         z
);

int32_t send_play_serverbound_player(
    bool          on_ground
);

int32_t send_play_serverbound_player_move(
    double        x,
    double        y,
    double        z,
    bool          on_ground
);

int32_t send_play_serverbound_player_look(
    float         yaw,
    float         pitch,
    bool          on_ground
);

int32_t send_play_serverbound_player_move_look(
    double        x,
    double        y,
    double        z,
    float         yaw,
    float         pitch,
    bool          on_ground
);

int32_t send_play_serverbound_player_dig(
    int8_t        status,
    position_t    location,
    int8_t        face
);

int32_t send_play_serverbound_player_block_place(
    position_t    location,
    int8_t        direction,
    slot_t        item,
    int8_t        x,
    int8_t        y,
    int8_t        z
);

int32_t send_play_serverbound_item_change(
    int16_t       slot
);

int32_t send_play_serverbound_animation(
    char*         format,
    int           packet_id
);

int32_t send_play_serverbound_entity_action(
    vint32_t      entity_id,
    uint8_t       action_id,
    vint32_t      jump_boost
);

int32_t send_play_serverbound_steer_vehicle(
    float         sideways,
    float         forward,
    uint8_t       flags
);

int32_t send_play_serverbound_update_sign(
    position_t    location,
    chat_t        line1,
    chat_t        line2,
    chat_t        line3,
    chat_t        line4
);

int32_t send_play_serverbound_player_abilities(
    int8_t        flags,
    float         flying_speed,
    float         walking_speed
);

int32_t send_play_serverbound_client_settings(
    char*         locale,
    int8_t        view_distance,
    int8_t        chat_flags,
    bool          chat_colors,
    uint8_t       skin
);

int32_t send_play_serverbound_player_status(
    vint32_t      action_id
);

int32_t send_play_serverbound_plugin_message(
    char*         channel,
    int8_t*       data
);

int32_t send_play_serverbound_spectate(
    __uint128_t   target
);

/*
 * Login clientbound structs
 */

login_clientbound_disconnect
recv_login_clientbound_disconnect();

login_clientbound_success
recv_login_clientbound_success();

login_clientbound_set_compression
recv_login_clientbound_set_compression();

/*
 * Status clientbound structs
 */

status_clientbound_response
recv_status_clientbound_response();

status_clientbound_ping
recv_status_clientbound_ping();

/*
 * Play clientbound structs
 */

play_clientbound_keepalive
recv_play_clientbound_keepalive();

play_clientbound_join_game
recv_play_clientbound_join_game();

play_clientbound_chat
recv_play_clientbound_chat();

play_clientbound_time_update
recv_play_clientbound_time_update();

play_clientbound_entity_equipment
recv_play_clientbound_entity_equipment();

play_clientbound_spawn_position
recv_play_clientbound_spawn_position();

play_clientbound_update_health
recv_play_clientbound_update_health();

play_clientbound_respawn
recv_play_clientbound_respawn();

play_clientbound_position
recv_play_clientbound_position();

play_clientbound_item_change
recv_play_clientbound_item_change();

play_clientbound_use_bed
recv_play_clientbound_use_bed();

play_clientbound_animation
recv_play_clientbound_animation();

play_clientbound_spawn_player
recv_play_clientbound_spawn_player();

play_clientbound_collect
recv_play_clientbound_collect();

play_clientbound_spawn_object
recv_play_clientbound_spawn_object();

play_clientbound_spawn_mob
recv_play_clientbound_spawn_mob();

play_clientbound_spawn_painting
recv_play_clientbound_spawn_painting();

play_clientbound_spawn_xp
recv_play_clientbound_spawn_xp();

play_clientbound_entity_velocity
recv_play_clientbound_entity_velocity();

play_clientbound_entity_destroy_entities
recv_play_clientbound_entity_destroy_entities();

play_clientbound_entity
recv_play_clientbound_entity();

play_clientbound_entity_move
recv_play_clientbound_entity_move();

play_clientbound_entity_look
recv_play_clientbound_entity_look();

play_clientbound_entity_look_move
recv_play_clientbound_entity_look_move();

play_clientbound_entity_teleport
recv_play_clientbound_entity_teleport();

play_clientbound_entity_head_look
recv_play_clientbound_entity_head_look();

play_clientbound_entity_status
recv_play_clientbound_entity_status();

play_clientbound_entity_attach
recv_play_clientbound_entity_attach();

play_clientbound_entity_effect
recv_play_clientbound_entity_effect();

play_clientbound_entity_clear_effect
recv_play_clientbound_entity_clear_effect();

play_clientbound_entity_properties
recv_play_clientbound_entity_properties();

play_clientbound_set_xp
recv_play_clientbound_set_xp();

play_clientbound_chunk_data
recv_play_clientbound_chunk_data();

play_clientbound_multi_block_change
recv_play_clientbound_multi_block_change();

play_clientbound_block_change
recv_play_clientbound_block_change();

play_clientbound_block_action
recv_play_clientbound_block_action();

play_clientbound_block_break_animation
recv_play_clientbound_block_break_animation();

play_clientbound_chunk_bulk
recv_play_clientbound_chunk_bulk();

play_clientbound_explosion
recv_play_clientbound_explosion();

play_clientbound_effect
recv_play_clientbound_effect();

play_clientbound_sound_effect
recv_play_clientbound_sound_effect();

play_clientbound_particle
recv_play_clientbound_particle();

play_clientbound_entity_spawn_global
recv_play_clientbound_entity_spawn_global();

play_clientbound_update_sign
recv_play_clientbound_update_sign();

play_clientbound_plugin_message
recv_play_clientbound_plugin_message();

play_clientbound_plugin_disconnect
recv_play_clientbound_plugin_disconnect();

play_clientbound_plugin_difficulty
recv_play_clientbound_plugin_difficulty();

play_clientbound_set_compression
recv_play_clientbound_set_compression();
