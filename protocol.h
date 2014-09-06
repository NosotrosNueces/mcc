
typedef uint64_t position_t;

/*
 * Handshaking serverbound structs
 */

struct handshaking_serverbound_handshake {
    char*         format;
    int           packet_id;

    uint32_t      protocol_version;
    char*         server_addr;
    uint16_t      server_port;
    uint32_t      next_state;
};

/*
 * Login clientbound structs
 */

struct login_clientbound_disconnect {
    char*         format;
    int           packet_id;

    char*         json;
};

struct login_clientbound_success {
    char*         format;
    int           packet_id;

    char*         uuid;
    char*         username;
};

struct login_clientbound_set_compression {
    char*         format;
    int           packet_id;

    uint32_t      threshold;
};

/*
 * Login serverbound structs
 */

struct login_serverbound_login {
    char*         format;
    int           packet_id;

    char*         username;
};

/*
 * Status clientbound structs
 */

struct status_clientbound_response {
    char*         format;
    int           packet_id;

    char*         json;
};

struct status_clientbound_ping {
    char*         format;
    int           packet_id;

    int64_t      time;
};

/*
 * Status serverbound structs
 */

struct status_serverbound_request {
    char*         format;
    int           packet_id;
};

struct status_clientbound_ping {
    char*         format;
    int           packet_id;

    int64_t      time;
};

/*
 * Play clientbound structs
 */

struct play_clientbound_keepalive {
    char*         format;
    int           packet_id;

    uint32_t      keepalive_id;
};

struct play_clientbound_join_game {
    char*         format;
    int           packet_id;

    int32_t       entity_id;
    uint8_t       gamemode;
    int8_t        dimension;
    uint8_t       difficulty;
    uint8_t       max_players;
    char*         level_type;
};

struct play_clientbound_chat {
    char*         format;
    int           packet_id;

    char*         json;
    int8_t        position;
};

struct play_clientbound_time_update {
    char*         format;
    int           packet_id;

    int64_t       age;
    int64_t       time;
};

struct play_clientbound_entity_equipment {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    uint16_t      slot;
    slot_t        item;
};

struct play_clientbound_spawn_position {
    char*         format;
    int           packet_id;

    position_t    location;
};

struct play_clientbound_update_health {
    char*         format;
    int           packet_id;

    float         health;
    uint32_t      food;
    float         saturation;
};

struct play_clientbound_respawn {
    char*         format;
    int           packet_id;

    int32_t       dimension;
    uint8_t       difficulty;
    uint8_t       gamemode;
    char*         level_type;
};

struct play_clientbound_position {
    char*         format;
    int           packet_id;

    double        x;
    double        y;
    double        z;
    float         yaw;
    float         pitch;
    int8_t        flags;
};

struct play_clientbound_item_change {
    char*         format;
    int           packet_id;

    int8_t        slot;
};

struct play_clientbound_use_bed {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    position_t    location;
};

struct play_clientbound_animation {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    uint8_t      animation;
};

struct play_clientbound_spawn_player {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    __uint128_t   uuid;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    int16_t       item;
    metadata_t    metadata;
};

struct play_clientbound_collect {
    char*         format;
    int           packet_id;

    uint32_t      collected_entity_id;
    uint32_t      collector_entity_id;
};

struct play_clientbound_spawn_object {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        type;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    data_t        data;
};

struct play_clientbound_spawn_mob {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
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
};

struct play_clientbound_spawn_painting {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    char*         title;
    position_t    location;
    uint8_t       direction;
};

struct play_clientbound_spawn_xp {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int16_t       count;
};

struct play_clientbound_entity_velocity {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int16_t       dx;
    int16_t       dy;
    int16_t       dz;
};

struct play_clientbound_entity_destroy_entities {
    char*         format;
    int           packet_id;

    int32_t       count;
    uint32_t*     entity_ids;
};

struct play_clientbound_entity {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
};

struct play_clientbound_entity_move {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        dx;
    int8_t        dy;
    int8_t        dz;
    bool          on_ground;
};

struct play_clientbound_entity_look {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
};

struct play_clientbound_entity_look_move {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        dx;
    int8_t        dy;
    int8_t        dz;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
};

struct play_clientbound_entity_teleport {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    int8_t        yaw;
    int8_t        pitch;
    bool          on_ground;
};

struct play_clientbound_entity_head_look {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        yaw;
};

struct play_clientbound_entity_status {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        status;
};

struct play_clientbound_entity_attach {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int32_t       vehicle_id;
    bool          leash;
};

struct play_clientbound_entity_effect {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        effect_id;
    int8_t        amplifier;
    uint32_t      duration;
    bool          hide;
};

struct play_clientbound_entity_clear_effect {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int32_t       effect_id;
};

struct play_clientbound_entity_properties {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int32_t       count;
    property_t*   properties;
};

struct play_clientbound_set_xp {
    char*         format;
    int           packet_id;

    float         xp_bar;
    int32_t       level;
    int32_t       xp;
};

struct play_clientbound_chunk_data {
    char*         format;
    int           packet_id;

    int32_t       chunk_x;
    int32_t       chunk_z;
    bool          continuous;
    uint16_t      bitmap;
    uint32_t      size;
    int8_t*       data;
};

struct play_clientbound_multi_block_change {
    char*         format;
    int           packet_id;

    int32_t       chunk_x;
    int32_t       chunk_z;
    uint32_t      count;
    record_t*     records;
};

struct play_clientbound_block_change {
    char*         format;
    int           packet_id;

    position_t    location;
    uint32_t      block_id;
};

struct play_clientbound_block_action {
    char*         format;
    int           packet_id;

    position_t    location;
    uint8_t       byte1;
    uint8_t       byte2;
    uint32_t      type;
};

struct play_clientbound_block_break_animation {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    position_t    location;
    int8_t        stage;
};

struct play_clientbound_chunk_bulk {
    char*         format;
    int           packet_id;

    bool          skylight;
    uint32_t      column_count;
    meta_t        meta;
    int8_t*       data;
};

struct play_clientbound_explosion {
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
};

struct play_clientbound_effect {
    char*         format;
    int           packet_id;

    uint32_t      effect_id;
    position_t    location;
    int32_t       data;
    bool          relative;
};

struct play_clientbound_sound_effect {
    char*         format;
    int           packet_id;

    char*         sound_name;
    int32_t       x;
    int32_t       y;
    int32_t       z;
    float         volume;
    uint8_t       pitch;
};

struct play_clientbound_particle {
    char*         format;
    int           packet_id;

    uint32_t      particle_id;
    bool          long_distance;
    float         x;
    float         y;
    float         z;
    float         dx;
    float         dy;
    float         dz;
    float         particle_data;
    int32_t       count;
    uint32_t*     data;
};

struct play_clientbound_entity_spawn_global {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    int8_t        type;
    int32_t       x;
    int32_t       y;
    int32_t       z;
};

struct play_clientbound_update_sign {
    char*         format;
    int           packet_id;

    position_t    location;
    chat_t        line1;
    chat_t        line2;
    chat_t        line3;
    chat_t        line4;
};

struct play_clientbound_plugin_message {
    char*         format;
    int           packet_id;

    char*         channel;
    int8_t*       data;
};

struct play_clientbound_plugin_disconnect {
    char*         format;
    int           packet_id;

    char*         reason;
};

struct play_clientbound_plugin_difficulty {
    char*         format;
    int           packet_id;

    uint8_t       difficulty;
};

struct play_clientbound_set_compression {
    char*         format;
    int           packet_id;

    uint32_t      threshold;
};

/*
 * Play serverbound structs
 */

struct play_serverbound_keepalive {
    char*         format;
    int           packet_id;

    uint32_t      keepalive_id;
};

struct play_serverbound_chat {
    char*         format;
    int           packet_id;

    char*         message;
};

struct play_serverbound_entity_use {
    char*         format;
    int           packet_id;

    uint32_t      target;
    uint32_t      type;
    float         x;
    float         y;
    float         z;
};

struct play_serverbound_player {
    char*         format;
    int           packet_id;

    bool          on_ground;
};

struct play_serverbound_player_move {
    char*         format;
    int           packet_id;

    double        x;
    double        y;
    double        z;
    bool          on_ground;
};

struct play_serverbound_player_look {
    char*         format;
    int           packet_id;

    float         yaw;
    float         pitch;
    bool          on_ground;
};

struct play_serverbound_player_move_look {
    char*         format;
    int           packet_id;

    double        x;
    double        y;
    double        z;
    float         yaw;
    float         pitch;
    bool          on_ground;
};

struct play_serverbound_player_dig {
    char*         format;
    int           packet_id;

    int8_t        status;
    position_t    location;
    int8_t        face;
};

struct play_serverbound_player_block_place {
    char*         format;
    int           packet_id;

    position_t    location;
    int8_t        direction;
    slot_t        item;
    int8_t        x;
    int8_t        y;
    int8_t        z;
};

struct play_serverbound_item_change {
    char*         format;
    int           packet_id;

    uint16_t      slot;
};

struct play_serverbound_animation {
    char*         format;
    int           packet_id;
};

struct play_serverbound_entity_action {
    char*         format;
    int           packet_id;

    uint32_t      entity_id;
    uint8_t       action_id;
    uint32_t      jump_boost;
};

struct play_serverbound_steer_vehicle {
    char*         format;
    int           packet_id;

    float         sideways;
    float         forward;
    uint8_t       flags;
};

struct play_serverbound_update_sign {
    char*         format;
    int           packet_id;

    position_t    location;
    chat_t        line1;
    chat_t        line2;
    chat_t        line3;
    chat_t        line4;
};

struct play_serverbound_player_abilities {
    char*         format;
    int           packet_id;

    int8_t        flags;
    float         flying_speed;
    float         walking_speed;
};

struct play_serverbound_client_settings {
    char*         format;
    int           packet_id;

    char*         locale;
    int8_t        view_distance;
    int8_t        chat_flags;
    bool          chat_colors;
    uint8_t       skin;
};

struct play_serverbound_player_status {
    char*         format;
    int           packet_id;

    uint32_t      action_id;
};

struct play_serverbound_plugin_message {
    char*         format;
    int           packet_id;

    char*         channel;
    int8_t*       data;
};

struct play_serverbound_spectate {
    char*         format;
    int           packet_id;

    __uint128_t   target;
};
