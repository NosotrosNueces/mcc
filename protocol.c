
#include "protocol.h"
#include "marshal.h"

/*
 * Handshaking serverbound functions
 */

int32_t send_handshaking_serverbound_handshake(
    bot_t         bot,
    vint32_t      protocol_version,
    char*         server_addr,
    uint16_t      server_port,
    vint32_t      next_state
)
{
    uint32_t length;
    handshaking_serverbound_handshake_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

/*
 * Login serverbound functions
 */

int32_t send_login_serverbound_login(
    bot_t         bot,
    char*         username
)
{
    uint32_t length;
    login_serverbound_login_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
    bot_t         bot
)
{
    uint32_t length;
    status_serverbound_request_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_status_clientbound_ping(
    bot_t         bot,
    int64_t       time
)
{
    uint32_t length;
    status_clientbound_ping_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

/*
 * Play serverbound functions
 */

int32_t send_play_serverbound_keepalive(
    bot_t         bot,
    vint32_t      keepalive_id
)
{
    uint32_t length;
    play_serverbound_keepalive_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_chat(
    bot_t         bot,
    char*         message
)
{
    uint32_t length;
    play_serverbound_chat_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_entity_use(
    bot_t         bot,
    vint32_t      target,
    vint32_t      type,
    float         x,
    float         y,
    float         z
)
{
    uint32_t length;
    play_serverbound_entity_use_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player(
    bot_t         bot,
    bool          on_ground
)
{
    uint32_t length;
    play_serverbound_player_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player_move(
    bot_t         bot,
    double        x,
    double        y,
    double        z,
    bool          on_ground
)
{
    uint32_t length;
    play_serverbound_player_move_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player_look(
    bot_t         bot,
    float         yaw,
    float         pitch,
    bool          on_ground
)
{
    uint32_t length;
    play_serverbound_player_look_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player_move_look(
    bot_t         bot,
    double        x,
    double        y,
    double        z,
    float         yaw,
    float         pitch,
    bool          on_ground
)
{
    uint32_t length;
    play_serverbound_player_move_look_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player_dig(
    bot_t         bot,
    int8_t        status,
    position_t    location,
    int8_t        face
)
{
    uint32_t length;
    play_serverbound_player_dig_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player_block_place(
    bot_t         bot,
    position_t    location,
    int8_t        direction,
    slot_t        item,
    int8_t        x,
    int8_t        y,
    int8_t        z
)
{
    uint32_t length;
    play_serverbound_player_block_place_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_item_change(
    bot_t         bot,
    int16_t       slot
)
{
    uint32_t length;
    play_serverbound_item_change_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_animation(
    bot_t         bot
)
{
    uint32_t length;
    play_serverbound_animation_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_entity_action(
    bot_t         bot,
    vint32_t      entity_id,
    uint8_t       action_id,
    vint32_t      jump_boost
)
{
    uint32_t length;
    play_serverbound_entity_action_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_steer_vehicle(
    bot_t         bot,
    float         sideways,
    float         forward,
    uint8_t       flags
)
{
    uint32_t length;
    play_serverbound_steer_vehicle_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_update_sign(
    bot_t         bot,
    position_t    location,
    chat_t        line1,
    chat_t        line2,
    chat_t        line3,
    chat_t        line4
)
{
    uint32_t length;
    play_serverbound_update_sign_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player_abilities(
    bot_t         bot,
    int8_t        flags,
    float         flying_speed,
    float         walking_speed
)
{
    uint32_t length;
    play_serverbound_player_abilities_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_client_settings(
    bot_t         bot,
    char*         locale,
    int8_t        view_distance,
    int8_t        chat_flags,
    bool          chat_colors,
    uint8_t       skin
)
{
    uint32_t length;
    play_serverbound_client_settings_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_player_status(
    bot_t         bot,
    vint32_t      action_id
)
{
    uint32_t length;
    play_serverbound_player_status_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_plugin_message(
    bot_t         bot,
    char*         channel,
    int8_t*       data
)
{
    uint32_t length;
    play_serverbound_plugin_message_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

int32_t send_play_serverbound_spectate(
    bot_t         bot,
    __uint128_t   target
)
{
    uint32_t length;
    play_serverbound_spectate_t p;
    // TODO: add all the juice
    p.format = "";
    p.packet_id = 0x00;

    void *packet;
    length = format_packet(&bot, &p, &packet);
    send_raw(&bot, packet, length);
    free(packet);

    return length;
}

/*
 * Login clientbound structs
 */

login_clientbound_disconnect_t*
recv_login_clientbound_disconnect(bot_t bot, void *packet)
{
    login_clientbound_disconnect_t *p;
    p = calloc(1, sizeof(login_clientbound_disconnect_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

login_clientbound_success_t*
recv_login_clientbound_success(bot_t bot, void *packet)
{
    login_clientbound_success_t *p;
    p = calloc(1, sizeof(login_clientbound_success_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

login_clientbound_set_compression_t*
recv_login_clientbound_set_compression(bot_t bot, void *packet)
{
    login_clientbound_set_compression_t *p;
    p = calloc(1, sizeof(login_clientbound_set_compression_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

/*
 * Status clientbound structs
 */

status_clientbound_response_t*
recv_status_clientbound_response(bot_t bot, void *packet)
{
    status_clientbound_response_t *p;
    p = calloc(1, sizeof(status_clientbound_response_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

status_clientbound_ping_t*
recv_status_clientbound_ping(bot_t bot, void *packet)
{
    status_clientbound_ping_t *p;
    p = calloc(1, sizeof(status_clientbound_ping_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

/*
 * Play clientbound structs
 */

play_clientbound_keepalive_t*
recv_play_clientbound_keepalive(bot_t bot, void *packet)
{
    play_clientbound_keepalive_t *p;
    p = calloc(1, sizeof(play_clientbound_keepalive_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_join_game_t*
recv_play_clientbound_join_game(bot_t bot, void *packet)
{
    play_clientbound_join_game_t *p;
    p = calloc(1, sizeof(play_clientbound_join_game_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_chat_t*
recv_play_clientbound_chat(bot_t bot, void *packet)
{
    play_clientbound_chat_t *p;
    p = calloc(1, sizeof(play_clientbound_chat_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_time_update_t*
recv_play_clientbound_time_update(bot_t bot, void *packet)
{
    play_clientbound_time_update_t *p;
    p = calloc(1, sizeof(play_clientbound_time_update_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_equipment_t*
recv_play_clientbound_entity_equipment(bot_t bot, void *packet)
{
    play_clientbound_entity_equipment_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_equipment_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_spawn_position_t*
recv_play_clientbound_spawn_position(bot_t bot, void *packet)
{
    play_clientbound_spawn_position_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_position_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_update_health_t*
recv_play_clientbound_update_health(bot_t bot, void *packet)
{
    play_clientbound_update_health_t *p;
    p = calloc(1, sizeof(play_clientbound_update_health_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_respawn_t*
recv_play_clientbound_respawn(bot_t bot, void *packet)
{
    play_clientbound_respawn_t *p;
    p = calloc(1, sizeof(play_clientbound_respawn_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_position_t*
recv_play_clientbound_position(bot_t bot, void *packet)
{
    play_clientbound_position_t *p;
    p = calloc(1, sizeof(play_clientbound_position_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_item_change_t*
recv_play_clientbound_item_change(bot_t bot, void *packet)
{
    play_clientbound_item_change_t *p;
    p = calloc(1, sizeof(play_clientbound_item_change_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_use_bed_t*
recv_play_clientbound_use_bed(bot_t bot, void *packet)
{
    play_clientbound_use_bed_t *p;
    p = calloc(1, sizeof(play_clientbound_use_bed_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_animation_t*
recv_play_clientbound_animation(bot_t bot, void *packet)
{
    play_clientbound_animation_t *p;
    p = calloc(1, sizeof(play_clientbound_animation_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_spawn_player_t*
recv_play_clientbound_spawn_player(bot_t bot, void *packet)
{
    play_clientbound_spawn_player_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_player_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_collect_t*
recv_play_clientbound_collect(bot_t bot, void *packet)
{
    play_clientbound_collect_t *p;
    p = calloc(1, sizeof(play_clientbound_collect_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_spawn_object_t*
recv_play_clientbound_spawn_object(bot_t bot, void *packet)
{
    play_clientbound_spawn_object_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_object_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_spawn_mob_t*
recv_play_clientbound_spawn_mob(bot_t bot, void *packet)
{
    play_clientbound_spawn_mob_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_mob_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_spawn_painting_t*
recv_play_clientbound_spawn_painting(bot_t bot, void *packet)
{
    play_clientbound_spawn_painting_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_painting_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_spawn_xp_t*
recv_play_clientbound_spawn_xp(bot_t bot, void *packet)
{
    play_clientbound_spawn_xp_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_xp_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_velocity_t*
recv_play_clientbound_entity_velocity(bot_t bot, void *packet)
{
    play_clientbound_entity_velocity_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_velocity_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_destroy_entities_t*
recv_play_clientbound_entity_destroy_entities(bot_t bot, void *packet)
{
    play_clientbound_entity_destroy_entities_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_destroy_entities_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_t*
recv_play_clientbound_entity(bot_t bot, void *packet)
{
    play_clientbound_entity_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_move_t*
recv_play_clientbound_entity_move(bot_t bot, void *packet)
{
    play_clientbound_entity_move_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_move_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_look_t*
recv_play_clientbound_entity_look(bot_t bot, void *packet)
{
    play_clientbound_entity_look_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_look_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_look_move_t*
recv_play_clientbound_entity_look_move(bot_t bot, void *packet)
{
    play_clientbound_entity_look_move_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_look_move_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_teleport_t*
recv_play_clientbound_entity_teleport(bot_t bot, void *packet)
{
    play_clientbound_entity_teleport_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_teleport_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_head_look_t*
recv_play_clientbound_entity_head_look(bot_t bot, void *packet)
{
    play_clientbound_entity_head_look_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_head_look_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_status_t*
recv_play_clientbound_entity_status(bot_t bot, void *packet)
{
    play_clientbound_entity_status_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_status_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_attach_t*
recv_play_clientbound_entity_attach(bot_t bot, void *packet)
{
    play_clientbound_entity_attach_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_attach_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_effect_t*
recv_play_clientbound_entity_effect(bot_t bot, void *packet)
{
    play_clientbound_entity_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_effect_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_clear_effect_t*
recv_play_clientbound_entity_clear_effect(bot_t bot, void *packet)
{
    play_clientbound_entity_clear_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_clear_effect_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_properties_t*
recv_play_clientbound_entity_properties(bot_t bot, void *packet)
{
    play_clientbound_entity_properties_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_properties_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_set_xp_t*
recv_play_clientbound_set_xp(bot_t bot, void *packet)
{
    play_clientbound_set_xp_t *p;
    p = calloc(1, sizeof(play_clientbound_set_xp_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_chunk_data_t*
recv_play_clientbound_chunk_data(bot_t bot, void *packet)
{
    play_clientbound_chunk_data_t *p;
    p = calloc(1, sizeof(play_clientbound_chunk_data_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_multi_block_change_t*
recv_play_clientbound_multi_block_change(bot_t bot, void *packet)
{
    play_clientbound_multi_block_change_t *p;
    p = calloc(1, sizeof(play_clientbound_multi_block_change_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_block_change_t*
recv_play_clientbound_block_change(bot_t bot, void *packet)
{
    play_clientbound_block_change_t *p;
    p = calloc(1, sizeof(play_clientbound_block_change_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_block_action_t*
recv_play_clientbound_block_action(bot_t bot, void *packet)
{
    play_clientbound_block_action_t *p;
    p = calloc(1, sizeof(play_clientbound_block_action_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_block_break_animation_t*
recv_play_clientbound_block_break_animation(bot_t bot, void *packet)
{
    play_clientbound_block_break_animation_t *p;
    p = calloc(1, sizeof(play_clientbound_block_break_animation_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_chunk_bulk_t*
recv_play_clientbound_chunk_bulk(bot_t bot, void *packet)
{
    play_clientbound_chunk_bulk_t *p;
    p = calloc(1, sizeof(play_clientbound_chunk_bulk_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_explosion_t*
recv_play_clientbound_explosion(bot_t bot, void *packet)
{
    play_clientbound_explosion_t *p;
    p = calloc(1, sizeof(play_clientbound_explosion_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_effect_t*
recv_play_clientbound_effect(bot_t bot, void *packet)
{
    play_clientbound_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_effect_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_sound_effect_t*
recv_play_clientbound_sound_effect(bot_t bot, void *packet)
{
    play_clientbound_sound_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_sound_effect_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_particle_t*
recv_play_clientbound_particle(bot_t bot, void *packet)
{
    play_clientbound_particle_t *p;
    p = calloc(1, sizeof(play_clientbound_particle_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_entity_spawn_global_t*
recv_play_clientbound_entity_spawn_global(bot_t bot, void *packet)
{
    play_clientbound_entity_spawn_global_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_spawn_global_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_update_sign_t*
recv_play_clientbound_update_sign(bot_t bot, void *packet)
{
    play_clientbound_update_sign_t *p;
    p = calloc(1, sizeof(play_clientbound_update_sign_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_plugin_message_t*
recv_play_clientbound_plugin_message(bot_t bot, void *packet)
{
    play_clientbound_plugin_message_t *p;
    p = calloc(1, sizeof(play_clientbound_plugin_message_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_plugin_disconnect_t*
recv_play_clientbound_plugin_disconnect(bot_t bot, void *packet)
{
    play_clientbound_plugin_disconnect_t *p;
    p = calloc(1, sizeof(play_clientbound_plugin_disconnect_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_plugin_difficulty_t*
recv_play_clientbound_plugin_difficulty(bot_t bot, void *packet)
{
    play_clientbound_plugin_difficulty_t *p;
    p = calloc(1, sizeof(play_clientbound_plugin_difficulty_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

play_clientbound_set_compression_t*
recv_play_clientbound_set_compression(bot_t bot, void *packet)
{
    play_clientbound_set_compression_t *p;
    p = calloc(1, sizeof(play_clientbound_set_compression_t));
    // TODO: add all the juice
    p->format = "";
    p->packet_id = 0x00;

    decode_packet(packet, &p);

    return p;
}

