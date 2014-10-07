#include <stdio.h>
#include "protocol.h"
#include "marshal.h"
#include "bot.h"

// Macro for defining packet recv functions
#define _render_recv(NAME, FORMAT, ID) NAME ## _t* recv_ ## NAME(bot_t* bot){ \
    NAME ## _t *p;                                                            \
    p = calloc(1, sizeof(NAME ## _t));                                        \
    p->format = FORMAT;                                                       \
    p->packet_id = ID;                                                        \
    decode_packet(bot, bot->buf, p);                                          \
    return p;                                                                 \
}

// Macro to fill structs in the callback switch
#define _render_callback(NAME) {                                        \
    recv_struct = recv_ ## NAME(bot);                                   \
    while(func->next) {                                                 \
        (func->f)(bot, recv_struct);                                    \
        func = func->next;                                              \
    }                                                                   \
    break;                                                              \
}

// Macro to send packets to server
#define _render_send(BOT, PACKET)                             \
    int8_t packet[BOT->packet_threshold];                     \
    length = format_packet(BOT, &PACKET, (void *) &packet);   \
    send_raw(bot, packet, length);                            \

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

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

    _render_send(bot, p);

    return length;
}

/*
 * Login clientbound structs
 */

_render_recv(login_clientbound_disconnect, "vs", 0x00);
_render_recv(login_clientbound_success, "vss", 0x02);
_render_recv(login_clientbound_set_compression, "vv", 0x03);

/*
 * Status clientbound structs
 */

_render_recv(status_clientbound_response, "vs", 0x00);
_render_recv(status_clientbound_ping, "vl", 0x01);

/*
 * Play clientbound structs
 */

_render_recv(play_clientbound_keepalive, "vv", 0x00);
_render_recv(play_clientbound_join_game, "vwbbbbs", 0x01);
_render_recv(play_clientbound_chat, "vsb", 0x02);
_render_recv(play_clientbound_time_update, "vll", 0x03);
_render_recv(play_clientbound_entity_equipment, "vvhw", 0x04);
_render_recv(play_clientbound_spawn_position, "vl", 0x05);
_render_recv(play_clientbound_update_health, "vwvw", 0x06);
_render_recv(play_clientbound_respawn, "vwbbs", 0x07);
_render_recv(play_clientbound_position, "vlllwwb", 0x08);
_render_recv(play_clientbound_item_change, "vb", 0x09);
_render_recv(play_clientbound_use_bed, "vvl", 0x0A);
_render_recv(play_clientbound_animation, "vvb", 0x0B);
_render_recv(play_clientbound_spawn_player, "vvqwwwbbhw", 0x0C);
_render_recv(play_clientbound_collect, "vvv", 0x0D);
_render_recv(play_clientbound_spawn_object, "vvbwwwbbw", 0x0E);
_render_recv(play_clientbound_spawn_mob, "vvbwwwbbbhhhw", 0x0F);
_render_recv(play_clientbound_spawn_painting, "vvslb", 0x10);
_render_recv(play_clientbound_spawn_xp, "vvwwwh", 0x11);
_render_recv(play_clientbound_entity_velocity, "vvhhh", 0x12);
_render_recv(play_clientbound_entity_destroy_entities, "vv*v", 0x13);
_render_recv(play_clientbound_entity, "vv", 0x14);
_render_recv(play_clientbound_entity_move, "vvbbbb", 0x15);
_render_recv(play_clientbound_entity_look, "vvbbbb", 0x16);
_render_recv(play_clientbound_entity_look_move, "vvbbbbbb", 0x17);
_render_recv(play_clientbound_entity_teleport, "vvwwwbbb", 0x18);
_render_recv(play_clientbound_entity_head_look, "vvb", 0x19);
_render_recv(play_clientbound_entity_status, "vwb", 0x1A);
_render_recv(play_clientbound_entity_attach, "vwwb", 0x1B);
_render_recv(play_clientbound_entity_effect, "vvbbvb", 0x1D);
_render_recv(play_clientbound_entity_clear_effect, "vvb", 0x1E);
_render_recv(play_clientbound_entity_properties, "vvw*w", 0x20);
_render_recv(play_clientbound_set_xp, "vwww", 0x1F);
_render_recv(play_clientbound_chunk_data, "vwwbhv*b", 0x21);
_render_recv(play_clientbound_multi_block_change, "vwwv*w", 0x22);
_render_recv(play_clientbound_block_change, "vlv", 0x23);
_render_recv(play_clientbound_block_action, "vlbbv", 0x24);
_render_recv(play_clientbound_block_break_animation, "vvlb", 0x25);
_render_recv(play_clientbound_chunk_bulk, "vbvwwh*b", 0x26);
_render_recv(play_clientbound_explosion, "vwwwww*wwww", 0x27);
_render_recv(play_clientbound_effect, "vwlwb", 0x28);
_render_recv(play_clientbound_sound_effect, "vswwwwb", 0x29);
_render_recv(play_clientbound_entity_spawn_global, "vvbwww", 0x2C);
_render_recv(play_clientbound_update_sign, "vlssss", 0x33);
_render_recv(play_clientbound_plugin_message, "vs*b", 0x3F);
_render_recv(play_clientbound_plugin_disconnect, "vs", 0x40);
_render_recv(play_clientbound_plugin_difficulty, "vb", 0x41);
_render_recv(play_clientbound_set_compression, "vv", 0x46);

void callback_decode(bot_t *bot) {
    int32_t pid = receive_packet(bot);
    if (pid < 0) {
        if (pid == -1) exit(123);
        return;
    }
    function *func = &bot->callbacks[bot->current_state][pid];
    void *recv_struct;
    switch (bot->current_state) {
        case HANDSHAKE:
            switch (pid) {
            }
            break;
        case LOGIN:
            switch (pid) {
                case 0x00: _render_callback(login_clientbound_disconnect);
                case 0x02: _render_callback(login_clientbound_success);
                case 0x03: _render_callback(login_clientbound_set_compression);
            }
            break;
        case STATUS:
            switch (pid) {
                case 0x00: _render_callback(status_clientbound_response);
                case 0x01: _render_callback(status_clientbound_ping);
            }
            break;
        case PLAY:
            switch (pid) {
                case 0x00: _render_callback(play_clientbound_keepalive);
                case 0x01: _render_callback(play_clientbound_join_game);
                case 0x02: _render_callback(play_clientbound_chat);
                case 0x03: _render_callback(play_clientbound_time_update);
                case 0x04: _render_callback(play_clientbound_entity_equipment);
                case 0x05: _render_callback(play_clientbound_spawn_position);
                case 0x06: _render_callback(play_clientbound_update_health);
                case 0x07: _render_callback(play_clientbound_respawn);
                case 0x08: _render_callback(play_clientbound_position);
                case 0x09: _render_callback(play_clientbound_item_change);
                case 0x0A: _render_callback(play_clientbound_use_bed);
                case 0x0B: _render_callback(play_clientbound_animation);
                case 0x0C: _render_callback(play_clientbound_spawn_player);
                case 0x0D: _render_callback(play_clientbound_collect);
                case 0x0E: _render_callback(play_clientbound_spawn_object);
                case 0x0F: _render_callback(play_clientbound_spawn_mob);
                case 0x10: _render_callback(play_clientbound_spawn_painting);
                case 0x11: _render_callback(play_clientbound_spawn_xp);
                case 0x12: _render_callback(play_clientbound_entity_velocity);
                case 0x13: _render_callback(play_clientbound_entity_destroy_entities);
                case 0x14: _render_callback(play_clientbound_entity);
                case 0x15: _render_callback(play_clientbound_entity_move);
                case 0x16: _render_callback(play_clientbound_entity_look);
                case 0x17: _render_callback(play_clientbound_entity_look_move);
                case 0x18: _render_callback(play_clientbound_entity_teleport);
                case 0x19: _render_callback(play_clientbound_entity_head_look);
                case 0x1A: _render_callback(play_clientbound_entity_status);
                case 0x1B: _render_callback(play_clientbound_entity_attach);
                case 0x1D: _render_callback(play_clientbound_entity_effect);
                case 0x1E: _render_callback(play_clientbound_entity_clear_effect);
                case 0x20: _render_callback(play_clientbound_entity_properties);
                case 0x1F: _render_callback(play_clientbound_set_xp);
                case 0x21: _render_callback(play_clientbound_chunk_data);
                case 0x22: _render_callback(play_clientbound_multi_block_change);
                case 0x23: _render_callback(play_clientbound_block_change);
                case 0x24: _render_callback(play_clientbound_block_action);
                case 0x25: _render_callback(play_clientbound_block_break_animation);
                case 0x26: _render_callback(play_clientbound_chunk_bulk);
                case 0x27: _render_callback(play_clientbound_explosion);
                case 0x28: _render_callback(play_clientbound_effect);
                case 0x29: _render_callback(play_clientbound_sound_effect);
                case 0x2C: _render_callback(play_clientbound_entity_spawn_global);
                case 0x33: _render_callback(play_clientbound_update_sign);
                case 0x3F: _render_callback(play_clientbound_plugin_message);
                case 0x40: _render_callback(play_clientbound_plugin_disconnect);
                case 0x41: _render_callback(play_clientbound_plugin_difficulty);
                case 0x46: _render_callback(play_clientbound_set_compression);
            }
            break;
        default:
            break;
    }
}
