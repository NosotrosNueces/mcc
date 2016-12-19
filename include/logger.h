#include "types.h"

void logger_clientbound_login_disconnect_cb(
        struct bot_agent *bot,
        char *reason
        );
void logger_clientbound_login_encryption_request_cb(
        struct bot_agent *bot,
        char *server_id,
        vint32_t public_key_length,
        unsigned char *public_key,
        vint32_t verify_token_length,
        unsigned char *verify_token
        );
void logger_clientbound_login_login_success_cb(
        struct bot_agent *bot,
        char *uuid,
        char *username
        );
void logger_clientbound_login_set_compression_cb(
        struct bot_agent *bot,
        vint32_t threshold
        );
/* Status */
void logger_clientbound_status_response_cb(
        struct bot_agent *bot,
        char *json_response
        );
void logger_clientbound_status_pong_cb(
        struct bot_agent *bot,
        int64_t payload
        );
/* Play */
void logger_clientbound_play_spawn_object_cb(
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
void logger_clientbound_play_spawn_experience_orb_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        double x,
        double y,
        double z,
        int16_t count
        );
void logger_clientbound_play_spawn_global_entity_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        int8_t type,
        double x,
        double y,
        double z
        );
void logger_clientbound_play_spawn_mob_cb(
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
void logger_clientbound_play_spawn_painting_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        char *uuid,
        char *title,
        struct mc_position location,
        int8_t direction
        );
void logger_clientbound_play_spawn_player_cb(
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
void logger_clientbound_play_animation_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        uint8_t animation
        );
void logger_clientbound_play_statistics_cb(
        struct bot_agent *bot,
        vint32_t count,
        struct statistic_type *statistic
        );
void logger_clientbound_play_block_break_animation_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        struct mc_position location,
        int8_t destroy_stage
        );
void logger_clientbound_play_update_block_entity_cb(
        struct bot_agent *bot,
        struct mc_position location,
        uint8_t action,
        struct nbt_tag *nbt
        );
void logger_clientbound_play_block_action_cb(
        struct bot_agent *bot,
        struct mc_position location,
        uint8_t byte1,
        uint8_t byte2,
        vint32_t block_type
        );
void logger_clientbound_play_block_change_cb(
        struct bot_agent *bot,
        struct mc_position location,
        vint32_t block_id
        );
void logger_clientbound_play_boss_bar_cb(
        struct bot_agent *bot,
        char *uuid,
        struct boss_bar_action *action_data
        );
void logger_clientbound_play_server_difficulty_cb(
        struct bot_agent *bot,
        uint8_t difficulty
        );
void logger_clientbound_play_tab_complete_cb(
        struct bot_agent *bot,
        vint32_t count,
        char **matches
        );
void logger_clientbound_play_chat_message_cb(
        struct bot_agent *bot,
        char *json_data,
        int8_t position
        );
void logger_clientbound_play_multi_block_change_cb(
        struct bot_agent *bot,
        int32_t chunk_x,
        int32_t chunk_z,
        vint32_t record_count,
        struct chunk_record *record
        );
void logger_clientbound_play_confirm_transaction_cb(
        struct bot_agent *bot,
        int8_t window_id,
        int16_t action_number,
        bool accepted
        );
void logger_clientbound_play_close_window_cb(
        struct bot_agent *bot,
        uint8_t window_id
        );
void logger_clientbound_play_open_window_cb(
        struct bot_agent *bot,
        uint8_t window_id,
        char *window_type,
        char *window_title,
        uint8_t number_of_slots,
        int32_t entity_id
        );
void logger_clientbound_play_window_items_cb(
        struct bot_agent *bot,
        uint8_t window_id,
        int16_t count,
        struct slot_type *slot_data
        );
void logger_clientbound_play_window_property_cb(
        struct bot_agent *bot,
        uint8_t window_id,
        int16_t property,
        int16_t value
        );
void logger_clientbound_play_set_slot_cb(
        struct bot_agent *bot,
        int8_t window_id,
        int16_t slot,
        struct slot_type *slot_data
        );
void logger_clientbound_play_set_cooldown_cb(
        struct bot_agent *bot,
        vint32_t item_id,
        vint32_t cooldown_ticks
        );
void logger_clientbound_play_plugin_message_cb(
        struct bot_agent *bot,
        char *channel,
        uint32_t data_length,
        char *data
        );
void logger_clientbound_play_named_sound_effect_cb(
        struct bot_agent *bot,
        char *sound_name,
        vint32_t sound_category,
        int32_t effect_position_x,
        int32_t effect_position_y,
        int32_t effect_position_z,
        float volume,
        float pitch
        );
void logger_clientbound_play_disconnect_cb(
        struct bot_agent *bot,
        char *reason
        );
void logger_clientbound_play_entity_status_cb(
        struct bot_agent *bot,
        int32_t entity_id,
        int8_t entity_status
        );
void logger_clientbound_play_explosion_cb(
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
void logger_clientbound_play_unload_chunk_cb(
        struct bot_agent *bot,
        int32_t chunk_x,
        int32_t chunk_z
        );
void logger_clientbound_play_change_game_state_cb(
        struct bot_agent *bot,
        uint8_t reason,
        float value
        );
void logger_clientbound_play_keep_alive_cb(
        struct bot_agent *bot,
        vint32_t keep_alive_id
        );
void logger_clientbound_play_chunk_data_cb(
        struct bot_agent *bot,
        int32_t chunk_x,
        int32_t chunk_z,
        bool gound_up_continuous,
        vint32_t primary_bit_mask,
        int32_t number_of_sections,
        struct chunk_section *data,
        struct biome_record *biomes,
        vint32_t number_of_block_entities,
        struct nbt_tag *block_entities
        );
void logger_clientbound_play_effect_cb(
        struct bot_agent *bot,
        int32_t effect_id,
        struct mc_position location,
        int32_t data,
        bool disable_relative_volume
        );
void logger_clientbound_play_particle_cb(
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
void logger_clientbound_play_join_game_cb(
        struct bot_agent *bot,
        int32_t entity_id,
        uint8_t gamemode,
        int32_t dimension,
        uint8_t difficulty,
        uint8_t max_players,
        char *level_type,
        bool reduced_debug_info
        );
void logger_clientbound_play_map_cb(
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
void logger_clientbound_play_entity_relative_move_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        int16_t delta_x,
        int16_t delta_y,
        int16_t delta_z,
        bool on_ground
        );
void logger_clientbound_play_entity_look_and_relative_move_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        int16_t delta_x,
        int16_t delta_y,
        int16_t delta_z,
        uint8_t yaw,
        uint8_t pitch,
        bool on_ground
        );
void logger_clientbound_play_entity_look_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        uint8_t yaw,
        uint8_t pitch,
        bool on_ground
        );
void logger_clientbound_play_entity_cb(
        struct bot_agent *bot,
        vint32_t entity_id
        );
void logger_clientbound_play_vehicle_move_cb(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        uint8_t yaw,
        uint8_t pitch
        );
void logger_clientbound_play_open_sign_editor_cb(
        struct bot_agent *bot,
        struct mc_position location
        );
void logger_clientbound_play_player_abilities_cb(
        struct bot_agent *bot,
        int8_t flags,
        float flying_speed,
        float field_of_view_modifier
        );
void logger_clientbound_play_combat_event_cb(
        struct bot_agent *bot,
        struct combat_event *event
        );
void logger_clientbound_play_player_list_item_cb(
        struct bot_agent *bot,
        vint32_t action_type,
        vint32_t number_of_players,
        struct player_list_action *player_actions
        );
void logger_clientbound_play_player_position_and_look_cb(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch,
        int8_t flags,
        vint32_t teleport_id
        );
void logger_clientbound_play_use_bed_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        struct mc_position location
        );
void logger_clientbound_play_destroy_entities_cb(
        struct bot_agent *bot,
        vint32_t count,
        vint32_t *entity_ids
        );
void logger_clientbound_play_remove_entity_effect_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        int8_t effect_id
        );
void logger_clientbound_play_resource_pack_send_cb(
        struct bot_agent *bot,
        char *url,
        char *hash
        );
void logger_clientbound_play_respawn_cb(
        struct bot_agent *bot,
        int32_t dimension,
        uint8_t difficulty,
        uint8_t gamemode,
        char *level_type
        );
void logger_clientbound_play_entity_head_look_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        uint8_t head_yaw
        );
void logger_clientbound_play_world_border_cb(
        struct bot_agent *bot,
        struct world_border_action *action
        );
void logger_clientbound_play_camera_cb(
        struct bot_agent *bot,
        vint32_t camera_id
        );
void logger_clientbound_play_held_item_change_cb(
        struct bot_agent *bot,
        int8_t slot
        );
void logger_clientbound_play_display_scoreboard_cb(
        struct bot_agent *bot,
        int8_t position,
        char *score_name
        );
void logger_clientbound_play_entity_metadata_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        struct entity_metadata *metadata
        );
void logger_clientbound_play_attach_entity_cb(
        struct bot_agent *bot,
        int32_t attached_entity_id,
        int32_t holding_entity_id
        );
void logger_clientbound_play_entity_velocity_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        int16_t velocity_x,
        int16_t velocity_y,
        int16_t velocity_z
        );
void logger_clientbound_play_entity_equipment_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        vint32_t slot,
        struct slot_type *item
        );
void logger_clientbound_play_set_experience_cb(
        struct bot_agent *bot,
        float experience_bar,
        vint32_t level,
        vint32_t total_experience
        );
void logger_clientbound_play_update_health_cb(
        struct bot_agent *bot,
        float health,
        vint32_t food,
        float food_saturation
        );
void logger_clientbound_play_scoreboard_objective_cb(
        struct bot_agent *bot,
        char *objective_name,
        int8_t mode,
        char *objective_value,
        char *type
        );
void logger_clientbound_play_set_passengers_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        vint32_t passenger_count,
        vint32_t *passengers
        );
void logger_clientbound_play_teams_cb(
        struct bot_agent *bot,
        char *team_name,
        struct team_action *action
        );
void logger_clientbound_play_update_score_cb(
        struct bot_agent *bot,
        char *score_name,
        int8_t action,
        char *objective_name,
        vint32_t value
        );
void logger_clientbound_play_spawn_position_cb(
        struct bot_agent *bot,
        struct mc_position location
        );
void logger_clientbound_play_time_update_cb(
        struct bot_agent *bot,
        int64_t world_age,
        int64_t time_of_day
        );
void logger_clientbound_play_title_cb(
        struct bot_agent *bot,
        struct title_action *action
        );
void logger_clientbound_play_sound_effect_cb(
        struct bot_agent *bot,
        vint32_t sound_id,
        vint32_t sound_category,
        int32_t effect_position_x,
        int32_t effect_position_y,
        int32_t effect_position_z,
        float volume,
        float pitch
        );
void logger_clientbound_play_player_list_header_and_footer_cb(
        struct bot_agent *bot,
        char *header,
        char *footer
        );
void logger_clientbound_play_collect_item_cb(
        struct bot_agent *bot,
        vint32_t collected_entity_id,
        vint32_t collector_entity_id
        );
void logger_clientbound_play_entity_teleport_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        double x,
        double y,
        double z,
        uint8_t yaw,
        uint8_t pitch,
        bool on_ground
        );
void logger_clientbound_play_entity_properties_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        int32_t number_of_properties,
        struct entity_property *properties
        );
void logger_clientbound_play_entity_effect_cb(
        struct bot_agent *bot,
        vint32_t entity_id,
        int8_t effect_id,
        int8_t amplifier,
        vint32_t duration,
        bool hide_particles
        );
