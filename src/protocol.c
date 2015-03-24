#include <stdio.h>
#include "protocol.h"
#include "marshal.h"
#include "bot.h"

// function to send packets to server
uint32_t _render_send(bot_t *bot, void *p)
{
    int8_t packet[bot->_data->packet_threshold];
    uint32_t length = format_packet(bot, p, (void *) &packet);
    send_raw(bot, packet, length);
    return length;
}

/*
 * Handshaking serverbound functions
 */

int32_t send_handshaking_serverbound_handshake(
    bot_t*        bot,
    vint32_t      protocol_version,
    char*         server_addr,
    uint16_t      server_port,
    vint32_t      next_state
)
{
    uint32_t length;
    handshaking_serverbound_handshake_t p;
    p.format = "vvshv";
    p.packet_id = 0x00;

    p.protocol_version = protocol_version;
    p.server_addr = server_addr;
    p.server_port = server_port;
    p.next_state = next_state;

    length = _render_send(bot, &p);

    return length;
}

/*
 * Login serverbound functions
 */

int32_t send_login_serverbound_login(
    bot_t*        bot,
    char*         username
)
{
    uint32_t length;
    login_serverbound_login_t p;
    p.format = "vs";
    p.packet_id = 0x00;

    p.username = username;

    length = _render_send(bot, &p);

    return length;
}

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
    bot_t*        bot
)
{
    uint32_t length;
    status_serverbound_request_t p;
    p.format = "v";
    p.packet_id = 0x00;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_status_serverbound_ping(
    bot_t*        bot,
    int64_t       time
)
{
    uint32_t length;
    status_clientbound_ping_t p;
    p.format = "vl";
    p.packet_id = 0x01;

    p.time = time;

    length = _render_send(bot, &p);

    return length;
}

/*
 * Play serverbound functions
 */

int32_t send_play_serverbound_keepalive(
    bot_t*        bot,
    vint32_t      keepalive_id
)
{
    uint32_t length;
    play_serverbound_keepalive_t p;
    p.format = "vv";
    p.packet_id = 0x00;

    p.keepalive_id = keepalive_id;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_chat(
    bot_t*        bot,
    char*         message
)
{
    uint32_t length;
    play_serverbound_chat_t p;
    p.format = "vs";
    p.packet_id = 0x01;

    p.message = message;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_entity_use(
    bot_t*        bot,
    vint32_t      target,
    vint32_t      type,
    float         x,
    float         y,
    float         z
)
{
    uint32_t length;
    play_serverbound_entity_use_t p;
    if(type == 2) {
        p.format = "vvvwww";
    } else {
        p.format = "vvv";
    }
    p.packet_id = 0x02;

    p.target = target;
    p.type = type;
    p.x = x;
    p.y = y;
    p.z = z;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player(
    bot_t*        bot,
    bool          on_ground
)
{
    uint32_t length;
    play_serverbound_player_t p;
    p.format = "vb";
    p.packet_id = 0x03;

    p.on_ground = on_ground;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player_move(
    bot_t*        bot,
    double        x,
    double        y,
    double        z,
    bool          on_ground
)
{
    uint32_t length;
    play_serverbound_player_move_t p;
    p.format = "vlllb";
    p.packet_id = 0x04;

    p.x = x;
    p.y = y;
    p.z = z;
    p.on_ground = on_ground;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player_look(
    bot_t*        bot,
    float         yaw,
    float         pitch,
    bool          on_ground
)
{
    uint32_t length;
    play_serverbound_player_look_t p;
    p.format = "vwwb";
    p.packet_id = 0x05;

    p.yaw = yaw;
    p.pitch = pitch;
    p.on_ground = on_ground;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player_move_look(
    bot_t*        bot,
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
    p.format = "vlllwwb";
    p.packet_id = 0x06;

    p.x = x;
    p.y = y;
    p.z = z;
    p.yaw = yaw;
    p.pitch = pitch;
    p.on_ground = on_ground;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player_dig(
    bot_t*        bot,
    int8_t        status,
    position_t    location,
    int8_t        face
)
{
    uint32_t length;
    play_serverbound_player_dig_t p;
    p.format = "vblb";
    p.packet_id = 0x07;

    p.status = status;
    p.location = location;
    p.face = face;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player_block_place(
    bot_t*        bot,
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
    p.format = "vlbwbbb";
    p.packet_id = 0x08;

    p.location = location;
    p.direction = direction;
    p.item = item;
    p.x = x;
    p.y = y;
    p.z = z;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_item_change(
    bot_t*        bot,
    int16_t       slot
)
{
    uint32_t length;
    play_serverbound_item_change_t p;
    p.format = "vh";
    p.packet_id = 0x09;

    p.slot = slot;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_animation(
    bot_t*        bot
)
{
    uint32_t length;
    play_serverbound_animation_t p;
    p.format = "v";
    p.packet_id = 0x0A;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_entity_action(
    bot_t*        bot,
    vint32_t      entity_id,
    uint8_t       action_id,
    vint32_t      jump_boost
)
{
    uint32_t length;
    play_serverbound_entity_action_t p;
    p.format = "vvbv";
    p.packet_id = 0x0B;

    p.entity_id = entity_id;
    p.action_id = action_id;
    p.jump_boost = jump_boost;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_steer_vehicle(
    bot_t*        bot,
    float         sideways,
    float         forward,
    uint8_t       flags
)
{
    uint32_t length;
    play_serverbound_steer_vehicle_t p;
    p.format = "vwwb";
    p.packet_id = 0x0C;

    p.sideways = sideways;
    p.forward = forward;
    p.flags = flags;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_update_sign(
    bot_t*        bot,
    position_t    location,
    chat_t        line1,
    chat_t        line2,
    chat_t        line3,
    chat_t        line4
)
{
    uint32_t length;
    play_serverbound_update_sign_t p;
    p.format = "vlssss";
    p.packet_id = 0x12;

    p.location = location;
    p.line1 = line1;
    p.line2 = line2;
    p.line3 = line3;
    p.line4 = line4;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player_abilities(
    bot_t*        bot,
    int8_t        flags,
    float         flying_speed,
    float         walking_speed
)
{
    uint32_t length;
    play_serverbound_player_abilities_t p;
    p.format = "vbww";
    p.packet_id = 0x13;

    p.flags = flags;
    p.flying_speed = flying_speed;
    p.walking_speed = walking_speed;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_client_settings(
    bot_t*        bot,
    char*         locale,
    int8_t        view_distance,
    int8_t        chat_flags,
    bool          chat_colors,
    uint8_t       skin
)
{
    uint32_t length;
    play_serverbound_client_settings_t p;
    p.format = "vsbbbb";
    p.packet_id = 0x15;

    p.locale = locale;
    p.view_distance = view_distance;
    p.chat_flags = chat_flags;
    p.chat_colors = chat_colors;
    p.skin = skin;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_player_status(
    bot_t*        bot,
    vint32_t      action_id
)
{
    uint32_t length;
    play_serverbound_player_status_t p;
    p.format = "vb";
    p.packet_id = 0x16;

    p.action_id = action_id;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_plugin_message(
    bot_t*        bot,
    char*         channel,
    int8_t*       data
)
{
    uint32_t length;
    play_serverbound_plugin_message_t p;
    p.format = "vs*b";
    p.packet_id = 0x17;

    p.channel = channel;
    p.data = data;

    length = _render_send(bot, &p);

    return length;
}

int32_t send_play_serverbound_spectate(
    bot_t*        bot,
    __uint128_t   target
)
{
    uint32_t length;
    play_serverbound_spectate_t p;
    p.format = "vq";
    p.packet_id = 0x18;

    p.target = target;

    length = _render_send(bot, &p);

    return length;
}

/*
 * Login clientbound structs
 */

login_clientbound_disconnect_t* recv_login_clientbound_disconnect(bot_t *bot)
{
    login_clientbound_disconnect_t *p;
    p = calloc(1, sizeof(login_clientbound_disconnect_t));
    p->format = "vs";
    p->packet_id = 0;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

login_clientbound_success_t* recv_login_clientbound_success(bot_t *bot)
{
    login_clientbound_success_t *p;
    p = calloc(1, sizeof(login_clientbound_success_t));
    p->format = "vss";
    p->packet_id = 2;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

login_clientbound_set_compression_t* recv_login_clientbound_set_compression(bot_t *bot)
{
    login_clientbound_set_compression_t *p;
    p = calloc(1, sizeof(login_clientbound_set_compression_t));
    p->format = "vv";
    p->packet_id = 3;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

/*
 * Status clientbound structs
 */

status_clientbound_response_t* recv_status_clientbound_response(bot_t *bot)
{
    status_clientbound_response_t *p;
    p = calloc(1, sizeof(status_clientbound_response_t));
    p->format = "vs";
    p->packet_id = 0;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

status_clientbound_ping_t* recv_status_clientbound_ping(bot_t *bot)
{
    status_clientbound_ping_t *p;
    p = calloc(1, sizeof(status_clientbound_ping_t));
    p->format = "vl";
    p->packet_id = 1;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

/*
 * Play clientbound structs
 */

play_clientbound_keepalive_t* recv_play_clientbound_keepalive(bot_t *bot)
{
    play_clientbound_keepalive_t *p;
    p = calloc(1, sizeof(play_clientbound_keepalive_t));
    p->format = "vv";
    p->packet_id = 0;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_join_game_t* recv_play_clientbound_join_game(bot_t *bot)
{
    play_clientbound_join_game_t *p;
    p = calloc(1, sizeof(play_clientbound_join_game_t));
    p->format = "vwbbbbs";
    p->packet_id = 1;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_chat_t* recv_play_clientbound_chat(bot_t *bot)
{
    play_clientbound_chat_t *p;
    p = calloc(1, sizeof(play_clientbound_chat_t));
    p->format = "vsb";
    p->packet_id = 2;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_time_update_t* recv_play_clientbound_time_update(bot_t *bot)
{
    play_clientbound_time_update_t *p;
    p = calloc(1, sizeof(play_clientbound_time_update_t));
    p->format = "vll";
    p->packet_id = 3;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_equipment_t* recv_play_clientbound_entity_equipment(bot_t *bot)
{
    play_clientbound_entity_equipment_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_equipment_t));
    p->format = "vvhw";
    p->packet_id = 4;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_spawn_position_t* recv_play_clientbound_spawn_position(bot_t *bot)
{
    play_clientbound_spawn_position_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_position_t));
    p->format = "vl";
    p->packet_id = 5;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_update_health_t* recv_play_clientbound_update_health(bot_t *bot)
{
    play_clientbound_update_health_t *p;
    p = calloc(1, sizeof(play_clientbound_update_health_t));
    p->format = "vwvw";
    p->packet_id = 6;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_respawn_t* recv_play_clientbound_respawn(bot_t *bot)
{
    play_clientbound_respawn_t *p;
    p = calloc(1, sizeof(play_clientbound_respawn_t));
    p->format = "vwbbs";
    p->packet_id = 7;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_position_t* recv_play_clientbound_position(bot_t *bot)
{
    play_clientbound_position_t *p;
    p = calloc(1, sizeof(play_clientbound_position_t));
    p->format = "vlllwwb";
    p->packet_id = 8;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_item_change_t* recv_play_clientbound_item_change(bot_t *bot)
{
    play_clientbound_item_change_t *p;
    p = calloc(1, sizeof(play_clientbound_item_change_t));
    p->format = "vb";
    p->packet_id = 9;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_use_bed_t* recv_play_clientbound_use_bed(bot_t *bot)
{
    play_clientbound_use_bed_t *p;
    p = calloc(1, sizeof(play_clientbound_use_bed_t));
    p->format = "vvl";
    p->packet_id = 10;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_animation_t* recv_play_clientbound_animation(bot_t *bot)
{
    play_clientbound_animation_t *p;
    p = calloc(1, sizeof(play_clientbound_animation_t));
    p->format = "vvb";
    p->packet_id = 11;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_spawn_player_t* recv_play_clientbound_spawn_player(bot_t *bot)
{
    play_clientbound_spawn_player_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_player_t));
    p->format = "vvqwwwbbhw";
    p->packet_id = 12;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_collect_t* recv_play_clientbound_collect(bot_t *bot)
{
    play_clientbound_collect_t *p;
    p = calloc(1, sizeof(play_clientbound_collect_t));
    p->format = "vvv";
    p->packet_id = 13;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_spawn_object_t* recv_play_clientbound_spawn_object(bot_t *bot)
{
    play_clientbound_spawn_object_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_object_t));
    p->format = "vvbwwwbbw";
    p->packet_id = 14;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_spawn_mob_t* recv_play_clientbound_spawn_mob(bot_t *bot)
{
    play_clientbound_spawn_mob_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_mob_t));
    p->format = "vvbwwwbbbhhhw";
    p->packet_id = 15;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_spawn_painting_t* recv_play_clientbound_spawn_painting(bot_t *bot)
{
    play_clientbound_spawn_painting_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_painting_t));
    p->format = "vvslb";
    p->packet_id = 16;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_spawn_xp_t* recv_play_clientbound_spawn_xp(bot_t *bot)
{
    play_clientbound_spawn_xp_t *p;
    p = calloc(1, sizeof(play_clientbound_spawn_xp_t));
    p->format = "vvwwwh";
    p->packet_id = 17;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_velocity_t* recv_play_clientbound_entity_velocity(bot_t *bot)
{
    play_clientbound_entity_velocity_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_velocity_t));
    p->format = "vvhhh";
    p->packet_id = 18;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_destroy_entities_t* recv_play_clientbound_entity_destroy_entities(bot_t *bot)
{
    play_clientbound_entity_destroy_entities_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_destroy_entities_t));
    p->format = "vv*v";
    p->packet_id = 19;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_t* recv_play_clientbound_entity(bot_t *bot)
{
    play_clientbound_entity_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_t));
    p->format = "vv";
    p->packet_id = 20;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_move_t* recv_play_clientbound_entity_move(bot_t *bot)
{
    play_clientbound_entity_move_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_move_t));
    p->format = "vvbbbb";
    p->packet_id = 21;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_look_t* recv_play_clientbound_entity_look(bot_t *bot)
{
    play_clientbound_entity_look_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_look_t));
    p->format = "vvbbbb";
    p->packet_id = 22;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_look_move_t* recv_play_clientbound_entity_look_move(bot_t *bot)
{
    play_clientbound_entity_look_move_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_look_move_t));
    p->format = "vvbbbbbb";
    p->packet_id = 23;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_teleport_t* recv_play_clientbound_entity_teleport(bot_t *bot)
{
    play_clientbound_entity_teleport_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_teleport_t));
    p->format = "vvwwwbbb";
    p->packet_id = 24;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_head_look_t* recv_play_clientbound_entity_head_look(bot_t *bot)
{
    play_clientbound_entity_head_look_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_head_look_t));
    p->format = "vvb";
    p->packet_id = 25;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_status_t* recv_play_clientbound_entity_status(bot_t *bot)
{
    play_clientbound_entity_status_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_status_t));
    p->format = "vwb";
    p->packet_id = 26;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_attach_t* recv_play_clientbound_entity_attach(bot_t *bot)
{
    play_clientbound_entity_attach_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_attach_t));
    p->format = "vwwb";
    p->packet_id = 27;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_effect_t* recv_play_clientbound_entity_effect(bot_t *bot)
{
    play_clientbound_entity_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_effect_t));
    p->format = "vvbbvb";
    p->packet_id = 29;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_clear_effect_t* recv_play_clientbound_entity_clear_effect(bot_t *bot)
{
    play_clientbound_entity_clear_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_clear_effect_t));
    p->format = "vvb";
    p->packet_id = 30;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_properties_t* recv_play_clientbound_entity_properties(bot_t *bot)
{
    play_clientbound_entity_properties_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_properties_t));
    p->format = "vvw*w";
    p->packet_id = 32;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_set_xp_t* recv_play_clientbound_set_xp(bot_t *bot)
{
    play_clientbound_set_xp_t *p;
    p = calloc(1, sizeof(play_clientbound_set_xp_t));
    p->format = "vwww";
    p->packet_id = 31;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_chunk_data_t* recv_play_clientbound_chunk_data(bot_t *bot)
{
    play_clientbound_chunk_data_t *p;
    p = calloc(1, sizeof(play_clientbound_chunk_data_t));
    p->format = "vwwbhv*b";
    p->packet_id = 33;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_multi_block_change_t* recv_play_clientbound_multi_block_change(bot_t *bot)
{
    play_clientbound_multi_block_change_t *p;
    p = calloc(1, sizeof(play_clientbound_multi_block_change_t));
    p->format = "vwwv*w";
    p->packet_id = 34;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_block_change_t* recv_play_clientbound_block_change(bot_t *bot)
{
    play_clientbound_block_change_t *p;
    p = calloc(1, sizeof(play_clientbound_block_change_t));
    p->format = "vlv";
    p->packet_id = 35;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_block_action_t* recv_play_clientbound_block_action(bot_t *bot)
{
    play_clientbound_block_action_t *p;
    p = calloc(1, sizeof(play_clientbound_block_action_t));
    p->format = "vlbbv";
    p->packet_id = 36;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_block_break_animation_t* recv_play_clientbound_block_break_animation(bot_t *bot)
{
    play_clientbound_block_break_animation_t *p;
    p = calloc(1, sizeof(play_clientbound_block_break_animation_t));
    p->format = "vvlb";
    p->packet_id = 37;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_chunk_bulk_t* recv_play_clientbound_chunk_bulk(bot_t *bot)
{
    play_clientbound_chunk_bulk_t *p;
    p = calloc(1, sizeof(play_clientbound_chunk_bulk_t));
    p->format = "vbvwwh*b";
    p->packet_id = 38;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_explosion_t* recv_play_clientbound_explosion(bot_t *bot)
{
    play_clientbound_explosion_t *p;
    p = calloc(1, sizeof(play_clientbound_explosion_t));
    p->format = "vwwwww*wwww";
    p->packet_id = 39;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_effect_t* recv_play_clientbound_effect(bot_t *bot)
{
    play_clientbound_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_effect_t));
    p->format = "vwlwb";
    p->packet_id = 40;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_sound_effect_t* recv_play_clientbound_sound_effect(bot_t *bot)
{
    play_clientbound_sound_effect_t *p;
    p = calloc(1, sizeof(play_clientbound_sound_effect_t));
    p->format = "vswwwwb";
    p->packet_id = 41;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_entity_spawn_global_t* recv_play_clientbound_entity_spawn_global(bot_t *bot)
{
    play_clientbound_entity_spawn_global_t *p;
    p = calloc(1, sizeof(play_clientbound_entity_spawn_global_t));
    p->format = "vvbwww";
    p->packet_id = 44;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_update_sign_t* recv_play_clientbound_update_sign(bot_t *bot)
{
    play_clientbound_update_sign_t *p;
    p = calloc(1, sizeof(play_clientbound_update_sign_t));
    p->format = "vlssss";
    p->packet_id = 51;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_plugin_message_t* recv_play_clientbound_plugin_message(bot_t *bot)
{
    play_clientbound_plugin_message_t *p;
    p = calloc(1, sizeof(play_clientbound_plugin_message_t));
    p->format = "vs*b";
    p->packet_id = 63;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_plugin_disconnect_t* recv_play_clientbound_plugin_disconnect(bot_t *bot)
{
    play_clientbound_plugin_disconnect_t *p;
    p = calloc(1, sizeof(play_clientbound_plugin_disconnect_t));
    p->format = "vs";
    p->packet_id = 64;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_plugin_difficulty_t* recv_play_clientbound_plugin_difficulty(bot_t *bot)
{
    play_clientbound_plugin_difficulty_t *p;
    p = calloc(1, sizeof(play_clientbound_plugin_difficulty_t));
    p->format = "vb";
    p->packet_id = 65;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}

play_clientbound_set_compression_t* recv_play_clientbound_set_compression(bot_t *bot)
{
    play_clientbound_set_compression_t *p;
    p = calloc(1, sizeof(play_clientbound_set_compression_t));
    p->format = "vv";
    p->packet_id = 70;
    decode_packet(bot, bot->_data->buf, p);
    return p;
}


void *protocol_decode(bot_t *bot)
{
    int32_t pid = receive_packet(bot);
    if (pid < 0) {
        if (pid == -1) exit(123);
        return NULL;
    }
    void *recv_struct = NULL;
    switch (bot->_data->current_state) {
    case HANDSHAKE:
        switch (pid) {
        }
        break;
    case LOGIN:
        switch (pid) {
        case 0x00:
            recv_struct = recv_login_clientbound_disconnect(bot);
            break;
        case 0x02:
            recv_struct = recv_login_clientbound_success(bot);
            break;
        case 0x03:
            recv_struct = recv_login_clientbound_set_compression(bot);
            break;
        }
        break;
    case STATUS:
        switch (pid) {
        case 0x00:
            recv_struct = recv_status_clientbound_response(bot);
            break;
        case 0x01:
            recv_struct = recv_status_clientbound_ping(bot);
            break;
        }
        break;
    case PLAY:
        switch (pid) {
        case 0x00:
            recv_struct = recv_play_clientbound_keepalive(bot);
            break;
        case 0x01:
            recv_struct = recv_play_clientbound_join_game(bot);
            break;
        case 0x02:
            recv_struct = recv_play_clientbound_chat(bot);
            break;
        case 0x03:
            recv_struct = recv_play_clientbound_time_update(bot);
            break;
        case 0x04:
            recv_struct = recv_play_clientbound_entity_equipment(bot);
            break;
        case 0x05:
            recv_struct = recv_play_clientbound_spawn_position(bot);
            break;
        case 0x06:
            recv_struct = recv_play_clientbound_update_health(bot);
            break;
        case 0x07:
            recv_struct = recv_play_clientbound_respawn(bot);
            break;
        case 0x08:
            recv_struct = recv_play_clientbound_position(bot);
            break;
        case 0x09:
            recv_struct = recv_play_clientbound_item_change(bot);
            break;
        case 0x0A:
            recv_struct = recv_play_clientbound_use_bed(bot);
            break;
        case 0x0B:
            recv_struct = recv_play_clientbound_animation(bot);
            break;
        case 0x0C:
            recv_struct = recv_play_clientbound_spawn_player(bot);
            break;
        case 0x0D:
            recv_struct = recv_play_clientbound_collect(bot);
            break;
        case 0x0E:
            recv_struct = recv_play_clientbound_spawn_object(bot);
            break;
        case 0x0F:
            recv_struct = recv_play_clientbound_spawn_mob(bot);
            break;
        case 0x10:
            recv_struct = recv_play_clientbound_spawn_painting(bot);
            break;
        case 0x11:
            recv_struct = recv_play_clientbound_spawn_xp(bot);
            break;
        case 0x12:
            recv_struct = recv_play_clientbound_entity_velocity(bot);
            break;
        case 0x13:
            recv_struct = recv_play_clientbound_entity_destroy_entities(bot);
            break;
        case 0x14:
            recv_struct = recv_play_clientbound_entity(bot);
            break;
        case 0x15:
            recv_struct = recv_play_clientbound_entity_move(bot);
            break;
        case 0x16:
            recv_struct = recv_play_clientbound_entity_look(bot);
            break;
        case 0x17:
            recv_struct = recv_play_clientbound_entity_look_move(bot);
            break;
        case 0x18:
            recv_struct = recv_play_clientbound_entity_teleport(bot);
            break;
        case 0x19:
            recv_struct = recv_play_clientbound_entity_head_look(bot);
            break;
        case 0x1A:
            recv_struct = recv_play_clientbound_entity_status(bot);
            break;
        case 0x1B:
            recv_struct = recv_play_clientbound_entity_attach(bot);
            break;
        case 0x1D:
            recv_struct = recv_play_clientbound_entity_effect(bot);
            break;
        case 0x1E:
            recv_struct = recv_play_clientbound_entity_clear_effect(bot);
            break;
        case 0x20:
            recv_struct = recv_play_clientbound_entity_properties(bot);
            break;
        case 0x1F:
            recv_struct = recv_play_clientbound_set_xp(bot);
            break;
        case 0x21:
            recv_struct = recv_play_clientbound_chunk_data(bot);
            break;
        case 0x22:
            recv_struct = recv_play_clientbound_multi_block_change(bot);
            break;
        case 0x23:
            recv_struct = recv_play_clientbound_block_change(bot);
            break;
        case 0x24:
            recv_struct = recv_play_clientbound_block_action(bot);
            break;
        case 0x25:
            recv_struct = recv_play_clientbound_block_break_animation(bot);
            break;
        case 0x26:
            recv_struct = recv_play_clientbound_chunk_bulk(bot);
            break;
        case 0x27:
            recv_struct = recv_play_clientbound_explosion(bot);
            break;
        case 0x28:
            recv_struct = recv_play_clientbound_effect(bot);
            break;
        case 0x29:
            recv_struct = recv_play_clientbound_sound_effect(bot);
            break;
        case 0x2C:
            recv_struct = recv_play_clientbound_entity_spawn_global(bot);
            break;
        case 0x33:
            recv_struct = recv_play_clientbound_update_sign(bot);
            break;
        case 0x3F:
            recv_struct = recv_play_clientbound_plugin_message(bot);
            break;
        case 0x40:
            recv_struct = recv_play_clientbound_plugin_disconnect(bot);
            break;
        case 0x41:
            recv_struct = recv_play_clientbound_plugin_difficulty(bot);
            break;
        case 0x46:
            recv_struct = recv_play_clientbound_set_compression(bot);
            break;
        }
        break;
    default:
        break;
    }
    return recv_struct;
}
