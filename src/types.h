#pragma once

#include <uv.h>
#include <stdbool.h>
#include "nbt.h"

typedef enum {HANDSHAKE, LOGIN, STATUS, PLAY, NUM_STATES} state;

typedef uint64_t position_t;
typedef int32_t vint32_t;
typedef int64_t vint64_t;
typedef char* chat_t;
typedef int32_t data_t;
typedef void* metadata_t;
typedef void* property_t;
typedef void* record_t;
typedef void* slot_t;

struct bot_agent;

enum NBT_TYPE {
    NBT_TREE,
    NBT_BINARY
};

struct slot_type {
    int16_t block_id;
    int8_t count;
    int16_t damage;
    enum NBT_TYPE type;
    union {
        struct nbt_tag *tree;
        struct nbt_buffer {
            uint32_t length;
            char *data;
        } nbt_binary;
    };
};

enum ENTITY_METADATA_ENTRY_TYPE {
    ENTITY_METADATA_BYTE,
    ENTITY_METADATA_VARINT,
    ENTITY_METADATA_FLOAT,
    ENTITY_METADATA_STRING,
    ENTITY_METADATA_CHAT,
    ENTITY_METADATA_SLOT,
    ENTITY_METADATA_BOOLEAN,
    ENTITY_METADATA_ROTATION,
    ENTITY_METADATA_POSITION,
    ENTITY_METADATA_OPTPOSITION,
    ENTITY_METADATA_DIRECTION,
    ENTITY_METADATA_OPTUUID,
    ENTITY_METADATA_BLOCKID
};

struct entity_rotation_type {
    float x;
    float y;
    float z;
};

struct opt_position_type {
    bool present;
    position_t position;
};

struct opt_uuid_type {
    bool present;
    char uuid[16];
};

struct entity_metadata_entry {
    uint8_t index;
    enum ENTITY_METADATA_ENTRY_TYPE type;
    union {
        int8_t entity_byte;
        vint32_t entity_varint;
        float entity_float;
        char *entity_string;
        char *entity_chat;
        struct slot_type entity_slot;
        bool entity_boolean;
        struct entity_rotation_type entity_rotation;
        position_t entity_position;
        struct opt_position_type entity_opt_position;
        vint32_t entity_direction;
        struct opt_uuid_type entity_opt_uuid;
        vint32_t entity_blockid;
    };
    struct entity_metadata_entry *next;
};

struct entity_metadata {
    uint32_t length;
    struct entity_metadata_entry *entries;
};

enum BOSS_BAR_ACTION_TYPE {
    BOSS_BAR_ADD,
    BOSS_BAR_REMOVE,
    BOSS_BAR_UPDATE_HEALTH,
    BOSS_BAR_UPDATE_TITLE,
    BOSS_BAR_UPDATE_STYLE,
    BOSS_BAR_UPDATE_FLAGS
};

struct boss_bar_add {
    char *title;
    float health;
    vint32_t color;
    vint32_t division;
    uint8_t flags;
};

struct boss_bar_remove {
    /* empty */
};

struct boss_bar_update_health {
    float health;
};

struct boss_bar_update_title {
    char *title;
};

struct boss_bar_update_style {
    vint32_t color;
    vint32_t dividers;
};

struct boss_bar_update_flags {
    uint8_t flags;
};

struct boss_bar_action {
    enum BOSS_BAR_ACTION_TYPE action_type;
    union {
        struct boss_bar_add add;
        struct boss_bar_remove remove;
        struct boss_bar_update_health update_health;
        struct boss_bar_update_title update_title;
        struct boss_bar_update_style update_style;
        struct boss_bar_update_flags update_flags;
    };
};



struct statistic_type {
    char *name;
    vint32_t value;
};

struct chunk_record {
    uint8_t horizontal_position;
    uint8_t y_coord;
    vint32_t block_id;
};

struct explosion_record {
    int8_t x;
    int8_t y;
    int8_t z;
};

struct chunk_section {
    int32_t data_array[16][16][16];
    int8_t block_light[16][16][16];
    int8_t sky_light[16][16][16];
};

struct biome_record {
    int8_t biome[16][16];
};

struct map_icon {
    int8_t direction_and_type;
    int8_t x;
    int8_t z;
};

enum COMBAT_EVENT_TYPE {
    COMBAT_EVENT_ENTER_COMBAT,
    COMBAT_EVENT_END_COMBAT,
    COMBAT_EVENT_ENTITY_DEAD
};

struct combat_event_enter_combat {
    /* empty */
};

struct combat_event_end_combat {
    vint32_t duration;
    int32_t entity_id;
};

struct combat_event_entity_dead {
    vint32_t player_id;
    int32_t entity_id;
    char *message;
};

struct combat_event {
    enum COMBAT_EVENT_TYPE type;
    union {
        struct combat_event_enter_combat enter_combat;
        struct combat_event_end_combat end_combat;
        struct combat_event_entity_dead entity_dead;
    };
};

enum PLAYER_LIST_ACTION_TYPE {
    PLAYER_LIST_ADD_PLAYER,
    PLAYER_LIST_UPDATE_GAMEMODE,
    PLAYER_LIST_UPDATE_LATENCY,
    PLAYER_LIST_UPDATE_DISPLAY_NAME,
    PLAYER_LIST_REMOVE_PLAYER
};

struct player_property {
    char *name;
    char *value;
    bool is_signed;
    char *signature;
};

struct player_list_action_add_player {
    char *name;
    vint32_t number_of_properties;
    struct player_property *properties;
    vint32_t gamemode;
    vint32_t ping;
    bool has_display_name;
    char *display_name;
};

struct player_list_action_update_gamemode {
    vint32_t gamemode;
};

struct player_list_action_update_latency {
    vint32_t ping;
};

struct player_list_action_update_display_name {
    bool has_display_name;
    char *display_name;
};

struct player_list_action_remove_player {
    /* empty */
};

struct player_list_action {
    char uuid[16];
    union {
        struct player_list_action_add_player add_player;
        struct player_list_action_update_gamemode update_gamemode;
        struct player_list_action_update_latency update_latency;
        struct player_list_action_update_display_name update_display_name;
        struct player_list_action_remove_player remove_player;
    };
};

enum WORLD_BORDER_ACTION_TYPE {
    WORLD_BORDER_SET_SIZE,
    WORLD_BORDER_LERP_SIZE,
    WORLD_BORDER_SET_CENTER,
    WORLD_BORDER_INITIALIZE,
    WORLD_BORDER_SET_WARNING_TIME,
    WORLD_BORDER_SET_WARNING_BLOCKS
};

struct world_border_action_set_size {
    double diameter;
};

struct world_border_action_lerp_size {
    double old_diameter;
    double new_diameter;
    vint64_t speed;
};

struct world_border_action_set_center {
    double x;
    double z;
};

struct world_border_action_initialize {
    double x;
    double z;
    double old_diameter;
    double new_diameter;
    vint64_t speed;
    vint32_t portal_teleport_boundary;
    vint32_t warning_time;
    vint32_t warning_blocks;
};

struct world_border_action_set_warning_time {
    vint32_t warning_time;
};

struct world_border_action_set_warning_blocks {
    vint32_t warning_blocks;
};

struct world_border_action {
    enum WORLD_BORDER_ACTION_TYPE type;
    union {
        struct world_border_action_set_size set_size;
        struct world_border_action_lerp_size lerp_size;
        struct world_border_action_set_center set_center;
        struct world_border_action_initialize initialize;
        struct world_border_action_set_warning_time set_warning_time;
        struct world_border_action_set_warning_blocks set_warning_blocks;
    };
};

enum TEAM_ACTION_TYPE {
    TEAM_ACTION_CREATE_TEAM,
    TEAM_ACTION_REMOVE_TEAM,
    TEAM_ACTION_UPDATE_TEAM_INFO,
    TEAM_ACTION_ADD_PLAYERS_TO_TEAM,
    TEAM_ACTION_REMOVE_PLAYERS_FROM_TEAM
};

struct team_action_create_team {
    char *team_display_name;
    char *team_prefix;
    char *team_suffix;
    int8_t friendly_flags;
    char *name_tag_visibility;
    char *collision_rule;
    int8_t color;
    vint32_t player_count;
    char **players;
};

struct team_action_remove_team {
    /* empty */
};

struct team_action_update_team_info {
    char *team_display_name;
    char *team_prefix;
    char *team_suffix;
    int8_t friendly_flags;
    char *name_tag_visibility;
    char *collision_rule;
    int8_t color;
};

struct team_action_add_players_to_team {
    vint32_t player_count;
    char **players;
};

struct team_action_remove_players_from_team {
    vint32_t player_count;
    char **players;
};

struct team_action {
    enum TEAM_ACTION_TYPE type;
    union {
        struct team_action_create_team create_team;
        struct team_action_remove_team remove_team;
        struct team_action_update_team_info update_team_info;
        struct team_action_add_players_to_team add_players_to_team;
        struct team_action_remove_players_from_team remove_players_from_team;
    };
};

enum TITLE_ACTION_TYPE {
    TITLE_ACTION_SET_TITLE,
    TITLE_ACTION_SET_SUBTITLE,
    TITLE_ACTION_SET_TIMES_AND_DISPLAY,
    TITLE_ACTION_HIDE,
    TITLE_ACTION_RESET
};

struct title_action_set_title {
    char *title_text;
};

struct title_action_set_subtitle {
    char *subtitle_text;
};

struct title_action_set_times_and_display {
    int32_t fade_in;
    int32_t stay;
    int32_t fade_out;
};

struct title_action_hide {
    /* empty */
};

struct title_action_reset {
    /* empty */
};

struct title_action {
    enum TITLE_ACTION_TYPE type;
    union {
        struct title_action_set_title set_title;
        struct title_action_set_subtitle set_subtitle;
        struct title_action_set_times_and_display set_times_and_display;
        struct title_action_hide hide;
        struct title_action_reset reset;
    };
};

struct entity_modifier {
    char uuid[16];
    double amount;
    int8_t operation;
};

struct entity_property {
    char *key;
    double value;
    vint32_t number_of_modifiers;
    struct entity_modifier *modifiers;
};

enum SOCKET_READ_STATE {
    SOCKET_READ_PACKET_START,
    SOCKET_READ_PACKET_MIDDLE
};

struct _callbacks {
    /* login */
    void (*clientbound_login_disconnect_cb)(
            struct bot_agent *bot,
            int32_t reason_length,
            char *reason
            );
    void (*clientbound_login_encryption_request_cb)(
            struct bot_agent *bot,
            int32_t server_id_length,
            char *server_id,
            vint32_t public_key_length,
            char *public_key,
            vint32_t verify_token_length,
            char *verify_token
            );
    void (*clientbound_login_login_success_cb)(
            struct bot_agent *bot,
            int32_t uuid_length,
            char *uuid,
            int32_t username_length,
            char *username
            );
    void (*clientbound_login_set_compression_cb)(
            struct bot_agent *bot,
            vint32_t threshold
            );
    /* Status */
    void (*clientbound_status_response_cb)(
            struct bot_agent *bot,
            int32_t json_length,
            char *json_response
            );
    void (*clientbound_status_pong_cb)(
            struct bot_agent *bot,
            int64_t payload
            );
    /* Play */
    void (*clientbound_play_spawn_object_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            char *uuid,
            int8_t type,
            double x,
            double y,
            double z,
            uint8_t pitch,
            uint8_t yaw,
            int32_t data,
            int16_t velocity_x,
            int16_t velocity_y,
            int16_t velocity_z
            );
    void (*clientbound_play_spawn_experience_orb_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            double x,
            double y,
            double z,
            int16_t count
            );
    void (*clientbound_play_spawn_global_entity_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            int8_t type,
            double x,
            double y,
            double z
            );
    void (*clientbound_play_spawn_mob_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            char *uuid,
            uint8_t type,
            double x,
            double y,
            double z,
            uint8_t yaw,
            uint8_t pitch,
            uint8_t head_pitch,
            int16_t v_x,
            int16_t v_y,
            int16_t v_z,
            struct entity_metadata *metadata
            );
    void (*clientbound_play_spawn_painting_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            char *uuid,
            int32_t title_length,
            char *title,
            position_t location,
            int8_t direction
            );
    void (*clientbound_play_spawn_player_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            char *uuid,
            double x,
            double y,
            double z,
            uint8_t yaw,
            uint8_t pitch,
            struct entity_metadata *metadata
            );
    void (*clientbound_play_animation_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            uint8_t animation
            );
    void (*clientbound_play_statistics_cb)(
            struct bot_agent *bot,
            vint32_t count,
            struct statistic_type *statistic
            );
    void (*clientbound_play_block_break_animation_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            position_t location,
            int8_t destroy_stage
            );
    void (*clientbound_play_update_block_entity_cb)(
            struct bot_agent *bot,
            position_t location,
            uint8_t action,
            struct nbt_tag *nbt
            );
    void (*clientbound_play_block_action_cb)(
            struct bot_agent *bot,
            position_t location,
            uint8_t byte1,
            uint8_t byte2,
            vint32_t block_type
            );
    void (*clientbound_play_block_change_cb)(
            struct bot_agent *bot,
            position_t location,
            vint32_t block_id
            );
    void (*clientbound_play_boss_bar_cb)(
            struct bot_agent *bot,
            char *uuid,
            struct boss_bar_action *action_data
            );
    void (*clientbound_play_server_difficulty_cb)(
            struct bot_agent *bot,
            uint8_t difficulty
            );
    void (*clientbound_play_tab_complete_cb)(
            struct bot_agent *bot,
            vint32_t count,
            char **matches
            );
    void (*clientbound_play_chat_message_cb)(
            struct bot_agent *bot,
            char *json_data,
            int8_t position
            );
    void (*clientbound_play_multi_block_change_cb)(
            struct bot_agent *bot,
            int32_t chunk_x,
            int32_t chunk_z,
            vint32_t record_count,
            struct chunk_record *record
            );
    void (*clientbound_play_confirm_transaction_cb)(
            struct bot_agent *bot,
            int8_t window_id,
            int16_t action_number,
            bool accepted
            );
    void (*clientbound_play_close_window_cb)(
            struct bot_agent *bot,
            uint8_t window_id
            );
    void (*clientbound_play_open_window_cb)(
            struct bot_agent *bot,
            uint8_t window_id,
            char *window_type,
            char *window_title,
            uint8_t number_of_slots,
            int32_t entity_id
            );
    void (*clientbound_play_window_items_cb)(
            struct bot_agent *bot,
            uint8_t window_id,
            int16_t count,
            struct slot_type *slot_data
            );
    void (*clientbound_play_window_property_cb)(
            struct bot_agent *bot,
            uint8_t window_id,
            int16_t property,
            int16_t value
            );
    void (*clientbound_play_set_slot_cb)(
            struct bot_agent *bot,
            int8_t window_id,
            int16_t slot,
            struct slot_type *slot_data
            );
    void (*clientbound_play_set_cooldown_cb)(
            struct bot_agent *bot,
            vint32_t item_id,
            vint32_t cooldown_ticks
            );
    void (*clientbound_play_plugin_message_cb)(
            struct bot_agent *bot,
            char *channel,
            uint32_t data_length,
            char *data
            );
    void (*clientbound_play_named_sound_effect_cb)(
            struct bot_agent *bot,
            char *sound_name,
            vint32_t sound_category,
            int32_t effect_position_x,
            int32_t effect_position_y,
            int32_t effect_position_z,
            float volume,
            uint8_t pitch
            );
    void (*clientbound_play_disconnect_cb)(
            struct bot_agent *bot,
            char *reason
            );
    void (*clientbound_play_entity_status_cb)(
            struct bot_agent *bot,
            int32_t entity_id,
            int8_t entity_status
            );
    void (*clientbound_play_explosion_cb)(
            struct bot_agent *bot,
            float x,
            float y,
            float z,
            float radius,
            int32_t record_count,
            struct explosion_record *records,
            float player_motion_x,
            float player_motion_y,
            float player_motion_z
            );
    void (*clientbound_play_unload_chunk_cb)(
            struct bot_agent *bot,
            int32_t chunk_x,
            int32_t chunk_z
            );
    void (*clientbound_play_change_game_state_cb)(
            struct bot_agent *bot,
            uint8_t reason,
            float value
            );
    void (*clientbound_play_keep_alive_cb)(
            struct bot_agent *bot,
            vint32_t keep_alive_id
            );
    void (*clientbound_play_chunk_data_cb)(
            struct bot_agent *bot,
            int32_t chunk_x,
            int32_t chunk_z,
            bool gound_up_continuous,
            vint32_t primary_bit_mask,
            int32_t number_of_sections,
            struct chunk_section *data,
            struct biome_record *biomes,
            vint32_t number_of_block_entities,
            struct nbt_tag **block_entities
            );
    void (*clientbound_play_effect_cb)(
            struct bot_agent *bot,
            int32_t effect_id,
            position_t location,
            int32_t data,
            bool disable_relative_volume
            );
    void (*clientbound_play_particle_cb)(
            struct bot_agent *bot,
            int32_t particle_id,
            bool long_distance,
            float x,
            float y,
            float z,
            float offset_x,
            float offset_y,
            float offset_z,
            float particle_data,
            int32_t particle_count,
            uint32_t data_length,
            vint32_t *data
            );
    void (*clientbound_play_join_game_cb)(
            struct bot_agent *bot,
            int32_t entity_id,
            uint8_t gamemode,
            int32_t dimension,
            uint8_t difficulty,
            uint8_t max_players,
            char *level_type,
            bool reduced_debug_info
            );
    void (*clientbound_play_map_cb)(
            struct bot_agent *bot,
            vint32_t item_damage,
            int8_t scale,
            bool tracking_position,
            vint32_t icon_count,
            struct map_icon *icons,
            int8_t columns,
            int8_t rows,
            int8_t x,
            int8_t z,
            vint32_t length,
            uint8_t *data
            );
    void (*clientbound_play_entity_relative_move_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            int16_t delta_x,
            int16_t delta_y,
            int16_t delta_z,
            bool on_ground
            );
    void (*clientbound_play_entity_look_and_relative_move_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            int16_t delta_x,
            int16_t delta_y,
            int16_t delta_z,
            uint8_t yaw,
            uint8_t pitch,
            bool on_ground
            );
    void (*clientbound_play_entity_look_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            uint8_t yaw,
            uint8_t pitch,
            bool on_ground
            );
    void (*clientbound_play_entity_cb)(
            struct bot_agent *bot,
            vint32_t entity_id
            );
    void (*clientbound_play_vehicle_move_cb)(
            struct bot_agent *bot,
            double x,
            double y,
            double z,
            uint8_t yaw,
            uint8_t pitch
            );
    void (*clientbound_play_open_sign_editor_cb)(
            struct bot_agent *bot,
            position_t location
            );
    void (*clientbound_play_player_abilities_cb)(
            struct bot_agent *bot,
            int8_t flags,
            float flying_speed,
            float field_of_view_modifier
            );
    void (*clientbound_play_combat_event_cb)(
            struct bot_agent *bot,
            struct combat_event *event
            );
    void (*clientbound_play_player_list_item_cb)(
            struct bot_agent *bot,
            vint32_t action_type,
            vint32_t number_of_players,
            struct player_list_action *player_actions
            );
    void (*clientbound_play_player_position_and_look_cb)(
            struct bot_agent *bot,
            double x,
            double y,
            double z,
            float yaw,
            float pitch,
            int8_t flags,
            vint32_t teleport_id
            );
    void (*clientbound_play_use_bed_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            position_t location
            );
    void (*clientbound_play_destroy_entities_cb)(
            struct bot_agent *bot,
            vint32_t count,
            vint32_t *entity_ids
            );
    void (*clientbound_play_remove_entity_effect_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            int8_t effect_id
            );
    void (*clientbound_play_resource_pack_send_cb)(
            struct bot_agent *bot,
            char *url,
            char *hash
            );
    void (*clientbound_play_respawn_cb)(
            struct bot_agent *bot,
            int32_t dimension,
            uint8_t difficulty,
            uint8_t gamemode,
            char *level_type
            );
    void (*clientbound_play_entity_head_look_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            uint8_t head_yaw
            );
    void (*clientbound_play_world_border_cb)(
            struct bot_agent *bot,
            struct world_border_action *action
            );
    void (*clientbound_play_camera_cb)(
            struct bot_agent *bot,
            vint32_t camera_id
            );
    void (*clientbound_play_held_item_change_cb)(
            struct bot_agent *bot,
            int8_t slot
            );
    void (*clientbound_play_display_scoreboard_cb)(
            struct bot_agent *bot,
            int8_t position,
            char *score_name
            );
    void (*clientbound_play_entity_metadata_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            struct entity_metadata *metadata
            );
    void (*clientbound_play_attach_entity_cb)(
            struct bot_agent *bot,
            int32_t attached_entity_id,
            int32_t holding_entity_id
            );
    void (*clientbound_play_entity_velocity_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            int16_t velocity_x,
            int16_t velocity_y,
            int16_t velocity_z
            );
    void (*clientbound_play_entity_equipment_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            vint32_t slot,
            struct slot_type *item
            );
    void (*clientbound_play_set_experience_cb)(
            struct bot_agent *bot,
            float experience_bar,
            vint32_t level,
            vint32_t total_experience
            );
    void (*clientbound_play_update_health_cb)(
            struct bot_agent *bot,
            float health,
            vint32_t food,
            float food_saturation
            );
    void (*clientbound_play_scoreboard_objective_cb)(
            struct bot_agent *bot,
            char *objective_name,
            int8_t mode,
            char *objective_value,
            char *type
            );
    void (*clientbound_play_set_passengers_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            vint32_t passenger_count,
            vint32_t *passengers
            );
    void (*clientbound_play_teams_cb)(
            struct bot_agent *bot,
            char *team_name,
            struct team_action *action
            );
    void (*clientbound_play_update_score_cb)(
            struct bot_agent *bot,
            char *score_name,
            int8_t action,
            char *objective_name,
            vint32_t value
            );
    void (*clientbound_play_spawn_position_cb)(
            struct bot_agent *bot,
            position_t location
            );
    void (*clientbound_play_time_update_cb)(
            struct bot_agent *bot,
            int64_t world_age,
            int64_t time_of_day
            );
    void (*clientbound_play_title_cb)(
            struct bot_agent *bot,
            struct title_action *action
            );
    void (*clientbound_play_sound_effect_cb)(
            struct bot_agent *bot,
            vint32_t sound_id,
            vint32_t sound_category,
            int32_t effect_position_x,
            int32_t effect_position_y,
            int32_t effect_position_z,
            float volume,
            uint8_t pitch
            );
    void (*clientbound_play_player_list_header_and_footer_cb)(
            struct bot_agent *bot,
            char *header,
            char *footer
            );
    void (*clientbound_play_collect_item_cb)(
            struct bot_agent *bot,
            vint32_t collected_entity_id,
            vint32_t collector_entity_id
            );
    void (*clientbound_play_entity_teleport_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            double x,
            double y,
            double z,
            uint8_t yaw,
            uint8_t pitch,
            bool on_ground
            );
    void (*clientbound_play_entity_properties_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            int32_t number_of_properties,
            struct entity_property *properties
            );
    void (*clientbound_play_entity_effect_cb)(
            struct bot_agent *bot,
            vint32_t entity_id,
            int8_t effect_id,
            int8_t amplifier,
            vint32_t duration,
            bool hide_particles
            );
};

enum MINECRAFT_DIMENSION {
    MINECRAFT_NETHER = -1,
    MINECRAFT_OVERWORLD,
    MINECRAFT_END
};

struct bot_agent {
    uint32_t eid;
    // 0: survival, 1: creative, 2: adventure. Bit 3 (0x8) is the hardcore flag
    uint8_t gamemode;
    // -1: nether, 0: overworld, 1: end
    enum MINECRAFT_DIMENSION dimension;
    // 0 thru 3 for Peaceful, Easy, Normal, Hard
    uint8_t difficulty;
    uint8_t max_players;
    // String  default, flat, largeBiomes, amplified, default_1_1
    char * level_type;
    bool reduced_debug_info;

    state current_state;

    char *name;
    double x;
    double y;
    double z;
    float yaw;
    float pitch;
    int8_t flags;

    uint8_t animation;

    int32_t health;
    int32_t food;
    float saturation;

    size_t packet_threshold;
    uv_loop_t loop;
    enum SOCKET_READ_STATE socket_read_state;
    uv_tcp_t socket;
    uv_buf_t socket_buffer;
    int32_t packet_capacity;
    int32_t packet_length;
    int32_t packet_bytes_read; /* how many packet bytes have been read */
    char *packet_data;
    /* callbacks */ 
    struct _callbacks callbacks;
};


