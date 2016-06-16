#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>
#include "protocol.h"
#include "types.h"
#include "serial.h"

#define PAD_LENGTH(x)           (x = (char *)x + 5)
#define DEFAULT_PACKET_LENGTH   (1 << 10)
#define CHUNK_DIM               16


/* appends length to the buffer as a varint, returns the start of the buffer */
/* writes the new length of packet in len */
char *_prepend_len(char *buf, uint32_t *len) {
    char buf_len[5];
    int offset = varint32_encode(*len, buf_len, sizeof(buf_len));
    *len += offset;
    char *start = (char *)buf + 5 - offset;
    memcpy(start, buf_len, offset);
    return start;
}

/*
 * Handshaking serverbound functions
 */

int32_t send_handshaking_serverbound_handshake(
        struct bot_agent*        bot,
        vint32_t      protocol_version,
        char*         server_addr,
        uint16_t      server_port,
        vint32_t      next_state
        ) {
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x00);
    _push_vint32(&buf, protocol_version);
    _push_string(&buf, server_addr);
    _push_uint16_t(&buf, server_port);
    _push_vint32(&buf, next_state);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

/*
 * Login serverbound functions
 */

int32_t send_login_serverbound_login_start(
        struct bot_agent*        bot,
        char*         username
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;
   
    _push_vint32(&buf, 0x00);
    _push_string(&buf, username);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
        struct bot_agent*        bot
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x00);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;

}

int32_t send_status_serverbound_ping(
        struct bot_agent*        bot,
        int64_t       time
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x01);
    _push_int64_t(&buf, time);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

/*
 * Play serverbound functions
 */

int32_t send_play_serverbound_teleport_confirm(
        struct bot_agent *bot,
        vint32_t teleport_id
        ) 
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x00);
    _push_vint32(&buf, teleport_id);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;

}

int32_t send_play_serverbound_tab_complete(
        struct bot_agent *bot,
        char *text,
        bool assume_command,
        bool has_position,
        position_t looked_at_block
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x01);
    _push_string(&buf, text);
    int8_t b = assume_command ? 1 : 0;
    _push(&buf, &b, sizeof(b));
    b = has_position ? 1 : 0;
    _push(&buf, &b, sizeof(b));
    if (has_position) {
        _push_uint64_t(&buf, looked_at_block);
    }

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_chat_message(
        struct bot_agent *bot,
        char *message
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x02);
    _push_string(&buf, message);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_client_status(
        struct bot_agent *bot,
        vint32_t action_id
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x03);
    _push_vint32(&buf, action_id);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_client_settings(
        struct bot_agent *bot,
        char *locale,
        int8_t view_distance,
        vint32_t chat_mode,
        bool chat_colors,
        uint8_t displayed_skin_parts,
        vint32_t main_hand
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x04);
    _push_string(&buf, locale);
    _push(&buf, &view_distance, sizeof(view_distance));
    _push_vint32(&buf, chat_mode);
    int8_t b = chat_colors ? 1 : 0;
    _push(&buf, &b, sizeof(b));
    _push(&buf, &displayed_skin_parts, sizeof(displayed_skin_parts));
    _push_vint32(&buf, main_hand);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_confirm_transaction(
        struct bot_agent *bot,
        int8_t window_id,
        int16_t action_number,
        bool accepted
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x05);
    _push(&buf, &window_id, sizeof(window_id));
    _push_int16_t(&buf, action_number);
    int8_t b = accepted ? 1 : 0;
    _push(&buf, &b, sizeof(b));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_enchant_item(
        struct bot_agent *bot,
        int8_t window_id,
        int8_t enchantment
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x06);
    _push(&buf, &window_id, sizeof(window_id));
    _push(&buf, &enchantment, sizeof(enchantment));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_click_window(
        struct bot_agent *bot,
        uint8_t window_id,
        int16_t slot,
        int8_t button,
        int16_t action_number,
        vint32_t mode,
        struct slot_type *clicked_item
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x07);
    _push(&buf, &window_id, sizeof(window_id));
    _push_int16_t(&buf, slot);
    _push(&buf, &button, sizeof(button));
    _push_int16_t(&buf, action_number);
    _push_vint32(&buf, mode);
    _push_slot(&buf, clicked_item);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_close_window(
        struct bot_agent *bot,
        uint8_t window_id
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x08);
    _push(&buf, &window_id, sizeof(window_id));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_plugin_message(
        struct bot_agent *bot,
        char *channel,
        uint32_t data_length,
        char *data
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x09);
    _push_string(&buf, channel);
    _push(&buf, data, data_length); 

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_use_entity(
        struct bot_agent *bot,
        vint32_t target,
        vint32_t type,
        float target_x,
        float target_y,
        float target_z,
        vint32_t hand
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x0A);
    _push_vint32(&buf, target);
    _push_vint32(&buf, type);
    switch(type) {
        case 2: /* interact at */
            _push_float(&buf, target_x);
            _push_float(&buf, target_y);
            _push_float(&buf, target_z);
        case 0: /* interact */
            _push_vint32(&buf, hand);
    }

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_keep_alive(
        struct bot_agent*        bot,
        vint32_t      keep_alive_id
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x0B);
    _push_vint32(&buf, keep_alive_id);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_player_position(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        bool on_ground
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x0C);
    _push_double(&buf, x);
    _push_double(&buf, y);
    _push_double(&buf, z);
    int8_t b = on_ground ? 1 : 0;
    _push(&buf, &b, sizeof(b));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_player_position_look(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch,
        bool on_ground
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x0D);
    _push_double(&buf, x);
    _push_double(&buf, y);
    _push_double(&buf, z);
    _push_float(&buf, yaw);
    _push_float(&buf, pitch);
    int8_t b = on_ground ? 1 : 0;
    _push(&buf, &b, sizeof(b));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_player_look(
        struct bot_agent *bot,
        float yaw,
        float pitch,
        bool on_ground
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x0E);
    _push_float(&buf, yaw);
    _push_float(&buf, pitch);
    int8_t b = on_ground ? 1 : 0;
    _push(&buf, &b, sizeof(b));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_player(
        struct bot_agent *bot,
        bool on_ground
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x0F);
    int8_t b = on_ground ? 1 : 0;
    _push(&buf, &b, sizeof(b));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_vehicle_move(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x10);
    _push_double(&buf, x);
    _push_double(&buf, y);
    _push_double(&buf, z);
    _push_float(&buf, yaw);
    _push_float(&buf, pitch);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_steer_boat(
        struct bot_agent *bot,
        bool right_paddle,
        bool left_paddle
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x11);
    int8_t b = right_paddle ? 1 : 0;
    _push(&buf, &b, sizeof(b));
    b = left_paddle ? 1 : 0;
    _push(&buf, &b, sizeof(b));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_player_abilities(
        struct bot_agent *bot,
        int8_t flags,
        float flying_speed,
        float walking_speed
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x12);
    _push(&buf, &flags, sizeof(flags));
    _push_float(&buf, flying_speed);
    _push_float(&buf, walking_speed);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_player_digging(
        struct bot_agent *bot,
        vint32_t status,
        position_t location,
        int8_t face
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x13);
    _push_vint32(&buf, status);
    _push_uint64_t(&buf, location);
    _push(&buf, &face, sizeof(face));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_entity_action(
        struct bot_agent *bot,
        vint32_t entity_id,
        vint32_t action_id,
        vint32_t jump_boost
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x14);
    _push_vint32(&buf, entity_id);
    _push_vint32(&buf, action_id);
    _push_vint32(&buf, jump_boost);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_steer_vehicle(
        struct bot_agent *bot,
        float sideways,
        float forward,
        uint8_t flags
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x15);
    _push_float(&buf, sideways);
    _push_float(&buf, forward);
    _push(&buf, &flags, sizeof(flags));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_resource_pack_status(
        struct bot_agent *bot,
        char *hash,
        vint32_t result
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x16);
    _push_string(&buf, hash);
    _push_vint32(&buf, result);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_held_item_change(
        struct bot_agent *bot,
        int16_t slot
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x17);
    _push_int16_t(&buf, slot);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_creative_inventory_action(
        struct bot_agent *bot,
        int16_t slot,
        struct slot_type *clicked_item
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x18);
    _push_int16_t(&buf, slot);
    _push_slot(&buf, clicked_item);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_update_sign(
        struct bot_agent *bot,
        position_t location,
        char *line1,
        char *line2,
        char *line3,
        char *line4
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x19);
    _push_uint64_t(&buf, location);
    _push_string(&buf, line1);
    _push_string(&buf, line2);
    _push_string(&buf, line3);
    _push_string(&buf, line4);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_animation(
        struct bot_agent *bot,
        vint32_t hand
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x1A);
    _push_vint32(&buf, hand);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_spectate(
        struct bot_agent *bot,
        char *uuid
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x1B);
    _push(&buf, uuid, 16);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_player_block_placement(
        struct bot_agent *bot,
        position_t location,
        vint32_t face,
        vint32_t hand,
        uint8_t x,
        uint8_t y,
        uint8_t z
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x1C);
    _push_uint64_t(&buf, location);
    _push_vint32(&buf, face);
    _push_vint32(&buf, hand);
    _push(&buf, &x, sizeof(x));
    _push(&buf, &y, sizeof(y));
    _push(&buf, &z, sizeof(z));

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

int32_t send_play_serverbound_use_item(
        struct bot_agent *bot,
        vint32_t hand
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x1D);
    _push_vint32(&buf, hand);

    uint32_t length = buf.ptr - data_start;
    char *packet = _prepend_len(buf.base, &length);
    send_raw(bot, packet, length);
    free(buf.base);
    return length;
}

void deserialize_clientbound_login_disconnect(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_login_disconnect_cb != NULL) {
        int32_t length;
        char *reason;
        packet_data = _read_string(packet_data, &reason, &length);
        bot->callbacks.clientbound_login_disconnect_cb(
                bot,
                length,
                reason
                );
    }
}

void deserialize_clientbound_login_encryption_request(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_login_encryption_request_cb != NULL) {
        int32_t server_id_length;
        char *server_id;
        vint32_t public_key_length;
        char *public_key;
        vint32_t verify_token_length;
        char *verify_token;

        packet_data = _read_string(packet_data, &server_id, &server_id_length);
        packet_data = _read_vint32(packet_data, &public_key_length);
        public_key = malloc(public_key_length);
        packet_data = _read(packet_data, public_key, public_key_length);
        packet_data = _read_vint32(packet_data, &verify_token_length);
        verify_token = malloc(verify_token_length);
        packet_data = _read(packet_data, verify_token, verify_token_length);

        bot->callbacks.clientbound_login_encryption_request_cb(
                bot,
                server_id_length,
                server_id,
                public_key_length,
                public_key,
                verify_token_length,
                verify_token
                );
        free(server_id);
        free(public_key);
        free(verify_token);
    }
}

void deserialize_clientbound_login_login_success(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_login_login_success_cb != NULL) {
        int32_t uuid_length; 
        char *uuid;
        int32_t username_length;
        char *username;

        packet_data = _read_string(packet_data, &uuid, &uuid_length);
        packet_data = _read_string(packet_data, &username, &username_length);
        bot->callbacks.clientbound_login_login_success_cb(
                bot,
                uuid_length,
                uuid,
                username_length,
                username
                );
        free(uuid);
        free(username);
    }
}

void deserialize_clientbound_login_set_compression(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_login_set_compression_cb != NULL) {
        vint32_t threshold;

        packet_data = _read_vint32(packet_data, &threshold);

        /* this probably shouldn't be a user callback function */
        bot->callbacks.clientbound_login_set_compression_cb(
                bot,
                threshold
                );
    }
}

void deserialize_clientbound_status_response(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_status_response_cb != NULL) {
        int32_t json_length; 
        char *json;
        packet_data = _read_string(packet_data, &json, &json_length);

        bot->callbacks.clientbound_status_response_cb(
                bot,
                json_length,
                json
                );
        free(json);
    }
}

void deserialize_clientbound_status_pong(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_status_pong_cb != NULL) {
        int64_t payload;
        packet_data = _read(packet_data, &payload, sizeof(payload));
        /* Maybe shouldn't be a user callback */
        bot->callbacks.clientbound_status_pong_cb(
                bot,
                payload
                );
    }
}

void deserialize_clientbound_play_spawn_object(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_spawn_object_cb != NULL) {
        vint32_t entity_id;
        char uuid[16];
        int8_t type;
        double x, y, z;
        uint8_t pitch, yaw;
        int32_t data;
        int16_t velocity_x, velocity_y, velocity_z;
        
        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, uuid, sizeof(uuid)); 
        packet_data = _read(packet_data, &type, sizeof(type));
        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);
        packet_data = _read(packet_data, &pitch, sizeof(pitch));
        packet_data = _read(packet_data, &yaw, sizeof(yaw));
        packet_data = _read_int32_t(packet_data, &data);
        packet_data = _read_int16_t(packet_data, &velocity_x);
        packet_data = _read_int16_t(packet_data, &velocity_y);
        packet_data = _read_int16_t(packet_data, &velocity_z);

        bot->callbacks.clientbound_play_spawn_object_cb(
                bot,
                entity_id,
                uuid,
                type,
                x,
                y,
                z,
                pitch,
                yaw,
                data,
                velocity_x,
                velocity_y,
                velocity_z
                );
    }
}

void deserialize_clientbound_play_spawn_experience_orb(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_spawn_experience_orb_cb != NULL) {
        vint32_t entity_id;
        double x, y, z;
        int16_t count;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);
        packet_data = _read_int16_t(packet_data, &count);

        bot->callbacks.clientbound_play_spawn_experience_orb_cb(
                bot,
                entity_id,
                x,
                y,
                z,
                count
                );
    }
}

void deserialize_clientbound_play_spawn_global_entity(char *packet_data,
        struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_spawn_global_entity_cb != NULL) {
        vint32_t entity_id;
        int8_t type;
        double x, y, z;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, &type, sizeof(type));
        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);

        bot->callbacks.clientbound_play_spawn_global_entity_cb(
                bot,
                entity_id,
                type,
                x,
                y,
                z
                );
    }
}

char *_read_entity_metadata(char *packet_data, struct entity_metadata *metadata) {
    int32_t dummy_length;
    char *packet_ptr = packet_data;
    struct entity_metadata_entry dummy;
    dummy.next = NULL;
    struct entity_metadata_entry *current_entry = &dummy;
    uint8_t index;
    int8_t type;
    for (packet_ptr = _read(packet_ptr, &index, sizeof(index));
            index != 0xff; packet_ptr = _read(packet_ptr, &index, sizeof(index))) {
        metadata->length++;
        current_entry->next = malloc(sizeof(struct entity_metadata_entry));
        current_entry = current_entry->next;
        current_entry->index = index;
        packet_ptr = _read(packet_ptr, &type, sizeof(type));
        current_entry->type = type;
        switch (current_entry->type) {
            case ENTITY_METADATA_BYTE:
                packet_ptr = _read(packet_ptr, &current_entry->entity_byte, sizeof(current_entry->entity_byte));
                break;
            case ENTITY_METADATA_VARINT:
                packet_ptr = _read_vint32(packet_ptr, &current_entry->entity_varint);
                break;
            case ENTITY_METADATA_FLOAT:
                packet_ptr = _read_float(packet_ptr, &current_entry->entity_float);
                break;
            case ENTITY_METADATA_STRING:
                packet_ptr = _read_string(packet_ptr, &current_entry->entity_string, &dummy_length);
                break;
            case ENTITY_METADATA_CHAT:
                packet_ptr = _read_string(packet_ptr, &current_entry->entity_chat, &dummy_length);
                break;
            case ENTITY_METADATA_SLOT:
                packet_ptr = _read_slot(packet_ptr, &current_entry->entity_slot);
                break;
            case ENTITY_METADATA_BOOLEAN:
            {
                int8_t boolean;
                packet_ptr = _read(packet_ptr, &boolean, sizeof(boolean));
                current_entry->entity_boolean = boolean ? true : false;
                break;
            }
            case ENTITY_METADATA_ROTATION:
                packet_ptr = _read_float(packet_ptr, &current_entry->entity_rotation.x);
                packet_ptr = _read_float(packet_ptr, &current_entry->entity_rotation.y);
                packet_ptr = _read_float(packet_ptr, &current_entry->entity_rotation.z);
                break;
            case ENTITY_METADATA_POSITION:
                packet_ptr = _read_uint64_t(packet_ptr, &current_entry->entity_position);
                break;
            case ENTITY_METADATA_OPTPOSITION:
            {
                int8_t boolean;
                packet_ptr = _read(packet_ptr, &boolean, sizeof(boolean));
                current_entry->entity_opt_position.present = boolean ? true : false;
                packet_ptr = _read_uint64_t(packet_ptr, &current_entry->entity_opt_position.position);
                break;
            }
            case ENTITY_METADATA_DIRECTION:
                packet_ptr = _read_vint32(packet_ptr, &current_entry->entity_direction);
                break;
            case ENTITY_METADATA_OPTUUID:
            {
                int8_t boolean;
                packet_ptr = _read(packet_ptr, &boolean, sizeof(boolean));
                current_entry->entity_opt_uuid.present = boolean ? true : false;
                packet_ptr = _read(packet_ptr, current_entry->entity_opt_uuid.uuid, sizeof(current_entry->entity_opt_uuid.uuid));
                break;
            }
            case ENTITY_METADATA_BLOCKID:
                packet_ptr = _read_vint32(packet_ptr, &current_entry->entity_blockid);
                break;
        }
    }
    metadata->entries = dummy.next;
    return packet_ptr;
}

void deserialize_clientbound_play_spawn_mob(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_spawn_mob_cb != NULL) {
        vint32_t entity_id;
        char uuid[16];
        uint8_t type;
        double x, y, z;
        uint8_t yaw, pitch, head_pitch;
        int16_t v_x, v_y, v_z;
        struct entity_metadata metadata;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, uuid, sizeof(uuid));
        packet_data = _read(packet_data, &type, sizeof(type));
        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);
        packet_data = _read(packet_data, &yaw, sizeof(yaw));
        packet_data = _read(packet_data, &pitch, sizeof(pitch));
        packet_data = _read(packet_data, &head_pitch, sizeof(pitch));
        packet_data = _read_int16_t(packet_data, &v_x);
        packet_data = _read_int16_t(packet_data, &v_y);
        packet_data = _read_int16_t(packet_data, &v_z);
        packet_data = _read_entity_metadata(packet_data, &metadata);

        bot->callbacks.clientbound_play_spawn_mob_cb(
                bot,
                entity_id,
                uuid,
                type,
                x,
                y,
                z,
                yaw, 
                pitch,
                head_pitch,
                v_x,
                v_y,
                v_z,
                &metadata
                );
    }
}

void deserialize_clientbound_play_spawn_painting(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_spawn_painting_cb != NULL) {
        vint32_t entity_id;
        char uuid[16];
        int32_t title_length;
        char *title;
        position_t location;
        int8_t direction;
        
        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, uuid, sizeof(uuid));
        packet_data = _read_string(packet_data, &title, &title_length);
        packet_data = _read_uint64_t(packet_data, &location);
        packet_data = _read(packet_data, &direction, sizeof(direction));

        bot->callbacks.clientbound_play_spawn_painting_cb(
                bot,
                entity_id,
                uuid,
                title_length,
                title,
                location,
                direction
                );
    }
}

void deserialize_clientbound_play_spawn_player(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_spawn_player_cb != NULL) {
        vint32_t entity_id; 
        char uuid[16];
        double x, y, z;
        uint8_t yaw, pitch;
        struct entity_metadata metadata;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, uuid, sizeof(uuid));
        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);
        packet_data = _read(packet_data, &yaw, sizeof(yaw));
        packet_data = _read(packet_data, &pitch, sizeof(pitch));
        packet_data = _read_entity_metadata(packet_data, &metadata);

        bot->callbacks.clientbound_play_spawn_player_cb(
                bot,
                entity_id,
                uuid,
                x,
                y,
                z,
                yaw,
                pitch,
                &metadata
                );
    }
}

void deserialize_clientbound_play_animation(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_animation_cb != NULL) {
        vint32_t entity_id;
        uint8_t animation;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, &animation, sizeof(animation));
        
        bot->callbacks.clientbound_play_animation_cb(
                bot,
                entity_id,
                animation
                );
    }
}

char *_read_statistic(char *packet_data, int32_t count, struct statistic_type *statistics) {
    int32_t dummy_length;
    for (int i = 0; i < count; i++) {
        packet_data = _read_string(packet_data, &statistics[i].name, &dummy_length);
        packet_data = _read_vint32(packet_data, &statistics[i].value);
    }
    return packet_data;
}

void deserialize_clientbound_play_statistics(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_statistics_cb != NULL) {
        vint32_t count;
        struct statistic_type *statistic;

        packet_data = _read_vint32(packet_data, &count);
        statistic = malloc(count * sizeof(struct statistic_type)); 
        packet_data = _read_statistic(packet_data, count, statistic);

        bot->callbacks.clientbound_play_statistics_cb(
                bot,
                count,
                statistic
                );
    }
}

void deserialize_clientbound_play_block_break_animation(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_block_break_animation_cb != NULL) {
        vint32_t entity_id;
        position_t location;
        int8_t destroy_stage;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_uint64_t(packet_data, &location);
        packet_data = _read(packet_data, &destroy_stage, sizeof(destroy_stage));

        bot->callbacks.clientbound_play_block_break_animation_cb(
                bot,
                entity_id,
                location,
                destroy_stage
                );
    }
}

void deserialize_clientbound_play_update_block_entity(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_update_block_entity_cb != NULL) {
        position_t location;
        uint8_t action;
        struct nbt_tag *nbt;
        
        packet_data = _read_uint64_t(packet_data, &location);
        packet_data = _read(packet_data, &action, sizeof(action));
        uint32_t bytes_read;
        nbt = nbt_parse(packet_data, &bytes_read);
        packet_data += bytes_read;

        bot->callbacks.clientbound_play_update_block_entity_cb(
                bot,
                location,
                action,
                nbt
                );
    }
}

void deserialize_clientbound_play_block_action(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_block_action_cb != NULL) {
        position_t location;
        uint8_t byte1, byte2;
        vint32_t block_type;

        packet_data = _read_uint64_t(packet_data, &location);
        packet_data = _read(packet_data, &byte1, sizeof(byte1));
        packet_data = _read(packet_data, &byte2, sizeof(byte2));
        packet_data = _read_vint32(packet_data, &block_type);

        bot->callbacks.clientbound_play_block_action_cb(
                bot,
                location,
                byte1,
                byte2,
                block_type
                );
    }
}

void deserialize_clientbound_play_block_change(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_block_change_cb != NULL) {
        position_t location;
        vint32_t block_id;

        packet_data = _read_uint64_t(packet_data, &location);
        packet_data = _read_vint32(packet_data, &block_id);

        bot->callbacks.clientbound_play_block_change_cb(
                bot,
                location,
                block_id
                );
    }
}

char *_read_boss_bar_action(char *packet_data, struct boss_bar_action *action_data) {
    vint32_t action_type;
    packet_data = _read_vint32(packet_data, &action_type);
    action_data->action_type = action_type;
    switch (action_data->action_type) {
        case BOSS_BAR_ADD:
        {
            struct boss_bar_add *add = &action_data->add;
            packet_data = _read_string(packet_data, &add->title, NULL);
            packet_data = _read_float(packet_data, &add->health);
            packet_data = _read_vint32(packet_data, &add->color);
            packet_data = _read_vint32(packet_data, &add->division);
            packet_data = _read(packet_data, &add->flags, sizeof(add->flags));
            break;
        }
        case BOSS_BAR_REMOVE:
            /* no fields */
            break;
        case BOSS_BAR_UPDATE_HEALTH:
        {
            struct boss_bar_update_health *update_health = &action_data->update_health;
            packet_data = _read_float(packet_data, &update_health->health);
            break;
        }
        case BOSS_BAR_UPDATE_TITLE:
        {
            struct boss_bar_update_title *update_title = &action_data->update_title;
            packet_data = _read_string(packet_data, &update_title->title, NULL);
            break;
        }
        case BOSS_BAR_UPDATE_STYLE:
        {
            struct boss_bar_update_style *update_style = &action_data->update_style;
            packet_data = _read_vint32(packet_data, &update_style->color);
            packet_data = _read_vint32(packet_data, &update_style->dividers);
            break;
        }
        case BOSS_BAR_UPDATE_FLAGS:
        {
            struct boss_bar_update_flags *update_flags = &action_data->update_flags;
            packet_data = _read(packet_data, &update_flags->flags, sizeof(update_flags->flags));
            break;
        }
    }
    return packet_data;
}

void deserialize_clientbound_play_boss_bar(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_boss_bar_cb != NULL) {
        char uuid[16];
        struct boss_bar_action action_data;
        
        packet_data = _read(packet_data, uuid, sizeof(uuid));
        packet_data = _read_boss_bar_action(packet_data, &action_data);   

        bot->callbacks.clientbound_play_boss_bar_cb(
                bot,
                uuid,
                &action_data
                );

    }
}

void deserialize_clientbound_play_server_difficulty(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_server_difficulty_cb != NULL) {
        int8_t difficulty;

        packet_data = _read(packet_data, &difficulty, sizeof(difficulty));

        bot->callbacks.clientbound_play_server_difficulty_cb(
                bot,
                difficulty
                );
    }
}

void deserialize_clientbound_play_tab_complete(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_tab_complete_cb != NULL) {
        vint32_t count;
        char **matches;

        packet_data = _read_vint32(packet_data, &count);
        matches = malloc(count * sizeof(char *));
        for (int i = 0; i < count; i++) {
            packet_data = _read_string(packet_data, &matches[i], NULL);
        }

        bot->callbacks.clientbound_play_tab_complete_cb(
                bot,
                count,
                matches
                );
    }
}

void deserialize_clientbound_play_chat_message(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_chat_message_cb != NULL) {
        char *json_data;
        int8_t position;

        packet_data = _read_string(packet_data, &json_data, NULL);
        packet_data = _read(packet_data, &position, sizeof(position));

        bot->callbacks.clientbound_play_chat_message_cb(
                bot,
                json_data,
                position
                );
    }
}

void deserialize_clientbound_play_multi_block_change(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_multi_block_change_cb != NULL) {
        int32_t chunk_x, chunk_z;
        vint32_t record_count;
        struct chunk_record *record;

        packet_data = _read_int32_t(packet_data, &chunk_x);
        packet_data = _read_int32_t(packet_data, &chunk_z);
        packet_data = _read_vint32(packet_data, &record_count);
        record = malloc(record_count * sizeof(struct chunk_record));
        for (int i = 0; i < record_count; i++) {
            packet_data = _read(packet_data, &record[i].horizontal_position, sizeof(record[i].horizontal_position));
            packet_data = _read(packet_data, &record[i].y_coord, sizeof(record[i].y_coord));
            packet_data = _read_vint32(packet_data, &record[i].block_id);
        }

        bot->callbacks.clientbound_play_multi_block_change_cb(
                bot,
                chunk_x,
                chunk_z,
                record_count,
                record
                );
    }
}

void deserialize_clientbound_play_confirm_transaction(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_confirm_transaction_cb != NULL) {
        int8_t window_id;
        int16_t action_number;
        bool accepted;

        packet_data = _read(packet_data, &window_id, sizeof(window_id));
        packet_data = _read_int16_t(packet_data, &action_number);
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        accepted = b ? true : false;

        bot->callbacks.clientbound_play_confirm_transaction_cb(
                bot,
                window_id,
                action_number,
                accepted
                );
    }
}

void deserialize_clientbound_play_close_window(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_close_window_cb != NULL) {
        uint8_t window_id;

        packet_data = _read(packet_data, &window_id, sizeof(window_id));

        bot->callbacks.clientbound_play_close_window_cb(
                bot,
                window_id
                );
    }
}

void deserialize_clientbound_play_open_window(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_open_window_cb != NULL) {
        uint8_t window_id;
        char *window_type;
        char *window_title;
        uint8_t number_of_slots;
        int32_t entity_id;

        packet_data = _read(packet_data, &window_id, sizeof(window_id));
        uint32_t window_type_length;
        packet_data = _read_string(packet_data, &window_type, &window_type_length);
        packet_data = _read_string(packet_data, &window_title, NULL);
        packet_data = _read(packet_data, &number_of_slots, sizeof(number_of_slots));
        if (strncmp(window_type, "EntityHorse", window_type_length)) {
            packet_data = _read_int32_t(packet_data, &entity_id);
        }

        bot->callbacks.clientbound_play_open_window_cb(
                bot,
                window_id,
                window_type,
                window_title,
                number_of_slots,
                entity_id
                );
    }
}

void deserialize_clientbound_play_window_items(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_window_items_cb != NULL) {
        uint8_t window_id;
        int16_t count;
        struct slot_type *slot_data;
        
        packet_data = _read(packet_data, &window_id, sizeof(window_id));
        packet_data = _read_int16_t(packet_data, &count);
        slot_data = malloc(count * sizeof(struct slot_type));
        for (int i = 0; i < count; i++) {
            packet_data = _read_slot(packet_data, &slot_data[i]);
        }

        bot->callbacks.clientbound_play_window_items_cb(
                bot,
                window_id,
                count,
                slot_data
                );
    }
}

void deserialize_clientbound_play_window_property(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_window_property_cb != NULL) {
        uint8_t window_id;
        int16_t property;
        int16_t value;

        packet_data = _read(packet_data, &window_id, sizeof(window_id));
        packet_data = _read_int16_t(packet_data, &property);
        packet_data = _read_int16_t(packet_data, &value);

        bot->callbacks.clientbound_play_window_property_cb(
                bot,
                window_id,
                property,
                value
                );
    }
}

void deserialize_clientbound_play_set_slot(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_set_slot_cb != NULL) {
        int8_t window_id;
        int16_t slot;
        struct slot_type slot_data;

        packet_data = _read(packet_data, &window_id, sizeof(window_id));
        packet_data = _read_int16_t(packet_data, &slot);
        packet_data = _read_slot(packet_data, &slot_data);

        bot->callbacks.clientbound_play_set_slot_cb(
                bot,
                window_id,
                slot,
                &slot_data
                );
    }
}

void deserialize_clientbound_play_set_cooldown(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_set_cooldown_cb != NULL) {
        vint32_t item_id, cooldown_ticks;

        packet_data = _read_vint32(packet_data, &item_id);
        packet_data = _read_vint32(packet_data, &cooldown_ticks);

        bot->callbacks.clientbound_play_set_cooldown_cb(
                bot,
                item_id,
                cooldown_ticks
                );
    }
}

void deserialize_clientbound_play_plugin_message(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_plugin_message_cb != NULL) {
        char *channel;
        uint32_t data_length;
        char *data;

        packet_data = _read_string(packet_data, &channel, NULL);
        data_length = bot->packet_length - (packet_data - bot->packet_data);
        data = malloc(data_length);
        packet_data = _read(packet_data, data, data_length);

        bot->callbacks.clientbound_play_plugin_message_cb(
                bot,
                channel,
                data_length,
                data
                );
    }
}

void deserialize_clientbound_play_named_sound_effect(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_named_sound_effect_cb != NULL) {
        char *sound_name;
        vint32_t sound_category;
        int32_t effect_position_x, effect_position_y, effect_position_z;
        float volume;
        uint8_t pitch;

        packet_data = _read_string(packet_data, &sound_name, NULL);
        packet_data = _read_vint32(packet_data, &sound_category);
        packet_data = _read_int32_t(packet_data, &effect_position_x);
        packet_data = _read_int32_t(packet_data, &effect_position_y);
        packet_data = _read_int32_t(packet_data, &effect_position_z);
        packet_data = _read_float(packet_data, &volume);
        packet_data = _read(packet_data, &pitch, sizeof(pitch));

        bot->callbacks.clientbound_play_named_sound_effect_cb(
                bot,
                sound_name,
                sound_category,
                effect_position_x,
                effect_position_y,
                effect_position_z,
                volume,
                pitch
                );
    }
}

void deserialize_clientbound_play_disconnect(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_disconnect_cb != NULL) {
        char *reason;

        packet_data = _read_string(packet_data, &reason, NULL);

        bot->callbacks.clientbound_play_disconnect_cb(
                bot,
                reason
                );
    }
}

void deserialize_clientbound_play_entity_status(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_status_cb != NULL) {
        int32_t entity_id;
        int8_t entity_status;

        packet_data = _read_int32_t(packet_data, &entity_id);
        packet_data = _read(packet_data, &entity_status, sizeof(entity_status));

        bot->callbacks.clientbound_play_entity_status_cb(
                bot,
                entity_id,
                entity_status
                );
    }
}

void deserialize_clientbound_play_explosion(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_explosion_cb != NULL) {
        float x, y, z, radius;
        int32_t record_count;
        struct explosion_record *records;
        float player_motion_x, player_motion_y, player_motion_z;

        packet_data = _read_float(packet_data, &x);
        packet_data = _read_float(packet_data, &y);
        packet_data = _read_float(packet_data, &z);
        packet_data = _read_float(packet_data, &radius);
        packet_data = _read_int32_t(packet_data, &record_count);
        records = malloc(record_count * sizeof(struct explosion_record));
        for (int i = 0; i < record_count; i++) {
            packet_data = _read(packet_data, &records[i].x, sizeof(records[i].x));
            packet_data = _read(packet_data, &records[i].y, sizeof(records[i].y));
            packet_data = _read(packet_data, &records[i].z, sizeof(records[i].z));
        }
        packet_data = _read_float(packet_data, &player_motion_x);
        packet_data = _read_float(packet_data, &player_motion_y);
        packet_data = _read_float(packet_data, &player_motion_z);

        bot->callbacks.clientbound_play_explosion_cb(
                bot,
                x,
                y,
                z,
                radius,
                record_count,
                records,
                player_motion_x,
                player_motion_y,
                player_motion_z
                );
    }
}

void deserialize_clientbound_play_unload_chunk(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_unload_chunk_cb != NULL) {
        int32_t chunk_x, chunk_z;

        packet_data = _read_int32_t(packet_data, &chunk_x);
        packet_data = _read_int32_t(packet_data, &chunk_z);

        bot->callbacks.clientbound_play_unload_chunk_cb(
                bot,
                chunk_x,
                chunk_z
                );
    }
}

void deserialize_clientbound_play_change_game_state(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_change_game_state_cb != NULL) {
        uint8_t reason;
        float value;

        packet_data = _read(packet_data, &reason, sizeof(reason));
        packet_data = _read_float(packet_data, &value);

        bot->callbacks.clientbound_play_change_game_state_cb(
                bot,
                reason,
                value
                );
    }
}

void deserialize_clientbound_play_keep_alive(char *packet_data, struct bot_agent *bot) {
    /* client is required to respond to this, otherwise the server close the connection */
    vint32_t keep_alive_id;

    packet_data = _read_vint32(packet_data, &keep_alive_id);
    
    send_play_serverbound_keep_alive(
            bot,
            keep_alive_id
            );

    if (bot->callbacks.clientbound_play_keep_alive_cb != NULL) {
        bot->callbacks.clientbound_play_keep_alive_cb(
                bot,
                keep_alive_id
                );
    }
}

uint32_t palette_index(uint64_t *data, int bits_per_block, int index) {
    int long_index = index * bits_per_block / 64; 
    int bit_index = index * bits_per_block % 64;
    uint32_t result;
    if (64 - bit_index >= bits_per_block) { /* block fits in 1 long */
        uint64_t x = data[long_index];
        uint64_t mask = (1 << bits_per_block) - 1;
        result = (x >> bit_index) & mask;
    } else { /* block crosses long boundary */
        uint64_t first = data[long_index], second = data[long_index + 1];
        int b1 = 64 - bit_index;
        int b2 = bits_per_block - b1;
        uint64_t mask2 = (1 << b2) - 1;
        result = 0; 
        result |= (first >> bit_index);
        result |= (second & mask2) << b1;
    }
    return result;
}

#define TO_X(index) (index % CHUNK_DIM)

#define TO_Y(index) (index / CHUNK_DIM / CHUNK_DIM)
#define TO_Z(index) ((index / CHUNK_DIM) % CHUNK_DIM)

char *_read_chunk_sections(char *packet_data, int32_t number_of_sections, struct chunk_section *data, int overworld) {
    for (int i = 0; i < number_of_sections; i++) {
        uint8_t bits_per_block;
        packet_data = _read(packet_data, &bits_per_block, sizeof(bits_per_block));
        if (bits_per_block < 4 && bits_per_block > 0) {
            bits_per_block = 4;
        }
        
        vint32_t palette_length;
        packet_data = _read_vint32(packet_data, &palette_length);

        int use_palette = palette_length > 0 && bits_per_block > 0 && bits_per_block < 9;

        vint32_t *palette = malloc(palette_length * sizeof(vint32_t));
        for (int j = 0; j < palette_length; j++) {
            packet_data = _read_vint32(packet_data, &palette[j]);
        }

        vint32_t data_array_length;
        packet_data = _read_vint32(packet_data, &data_array_length);
        uint64_t *data_array = malloc(data_array_length * sizeof(uint64_t));
        for (int j = 0; j < data_array_length; j++) {
            packet_data = _read_uint64_t(packet_data, &data_array[j]);
        }
        
        for (int j = 0; j < CHUNK_DIM * CHUNK_DIM * CHUNK_DIM; j++) {
            int block_data = palette_index(data_array, bits_per_block, j); 
            if (!use_palette) {
                data[i].data_array[TO_X(j)][TO_Y(j)][TO_Z(j)] = block_data;
            } else if (block_data < palette_length) {
                data[i].data_array[TO_X(j)][TO_Y(j)][TO_Z(j)] =
                    palette[block_data];

            } else {
                data[i].data_array[TO_X(j)][TO_Y(j)][TO_Z(j)] = 0;
            }
        }


        for (int j = 0; j < CHUNK_DIM * CHUNK_DIM * CHUNK_DIM / 2; j++) {
            uint8_t even = packet_data[j] & 0x0f;
            uint8_t odd = packet_data[j] >> 4;
            int even_index = 2 * j, odd_index = 2 * j + 1;
            data[i].block_light[TO_X(even_index)][TO_Y(even_index)][TO_Z(even_index)] = even;
            data[i].block_light[TO_X(odd_index)][TO_Y(odd_index)][TO_Z(odd_index)] = odd;
        }
        packet_data += CHUNK_DIM * CHUNK_DIM * CHUNK_DIM / 2;

        if (overworld) {
            for (int j = 0; j < CHUNK_DIM * CHUNK_DIM * CHUNK_DIM / 2; j++) {
                uint8_t even = packet_data[j] & 0x0f;
                uint8_t odd = packet_data[j] >> 4;
                int even_index = 2 * j, odd_index = 2 * j + 1;
                data[i].sky_light[TO_X(even_index)][TO_Y(even_index)][TO_Z(even_index)] = even;
                data[i].sky_light[TO_X(odd_index)][TO_Y(odd_index)][TO_Z(odd_index)] = odd;
            }
            packet_data += CHUNK_DIM * CHUNK_DIM * CHUNK_DIM / 2;
        }

        free(palette);
        free(data_array);
    }
    return packet_data;
}

char *_read_biomes(char *packet_data, struct biome_record *biomes) {
    for (int i = 0; i < CHUNK_DIM * CHUNK_DIM; i++) {
        biomes->biome[i % CHUNK_DIM][i / CHUNK_DIM] = packet_data[i];
    }
    return packet_data + CHUNK_DIM * CHUNK_DIM;

}

void deserialize_clientbound_play_chunk_data(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_chunk_data_cb != NULL) {
        int32_t chunk_x, chunk_z;
        bool ground_up_continuous;
        vint32_t primary_bit_mask;
        vint32_t size;
        struct chunk_section *data;
        struct biome_record biomes;
        vint32_t number_of_block_entities;
        struct nbt_tag **block_entities;  

        packet_data = _read_int32_t(packet_data, &chunk_x);
        packet_data = _read_int32_t(packet_data, &chunk_z);
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        ground_up_continuous = b ? true : false;
        packet_data = _read_vint32(packet_data, &primary_bit_mask);
        int32_t number_of_sections = __builtin_popcount(primary_bit_mask);
        data = malloc(number_of_sections * sizeof(struct chunk_section));
        packet_data = _read_vint32(packet_data, &size);
        packet_data = _read_chunk_sections(packet_data, number_of_sections, data, bot->dimension == MINECRAFT_OVERWORLD);
        if (ground_up_continuous) {
            packet_data = _read_biomes(packet_data, &biomes);
        }
        packet_data = _read_vint32(packet_data, &number_of_block_entities);
        block_entities = malloc(number_of_block_entities * sizeof(struct nbt_tag *));
        for (int i = 0; i < number_of_block_entities; i++) {
            uint32_t bytes_read;
            block_entities[i] = nbt_parse(packet_data, &bytes_read);
            packet_data += bytes_read;
        }
        
        bot->callbacks.clientbound_play_chunk_data_cb(
                bot,
                chunk_x,
                chunk_z,
                ground_up_continuous,
                primary_bit_mask,
                number_of_sections,
                data,
                &biomes,
                number_of_block_entities,
                block_entities
                );
    }
}

void deserialize_clientbound_play_effect(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_effect_cb != NULL) {
        int32_t effect_id;
        position_t location;
        int32_t data;
        bool disable_relative_volume;

        packet_data = _read_int32_t(packet_data, &effect_id);
        packet_data = _read_uint64_t(packet_data, &location);
        packet_data = _read_int32_t(packet_data, &data);
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        disable_relative_volume = b ? true : false;

        bot->callbacks.clientbound_play_effect_cb(
                bot,
                effect_id,
                location,
                data,
                disable_relative_volume
                );
    }
}

void deserialize_clientbound_play_particle(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_particle_cb != NULL) {
        int32_t particle_id;
        bool long_distance;
        float x, y, z, offset_x, offset_y, offset_z, particle_data;
        int32_t particle_count;
        vint32_t *data;

        packet_data = _read_int32_t(packet_data, &particle_id);
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        long_distance = b ? true : false;
        packet_data = _read_float(packet_data, &x);
        packet_data = _read_float(packet_data, &y);
        packet_data = _read_float(packet_data, &z);
        packet_data = _read_float(packet_data, &offset_x);
        packet_data = _read_float(packet_data, &offset_y);
        packet_data = _read_float(packet_data, &offset_z);
        packet_data = _read_float(packet_data, &particle_data);
        packet_data = _read_int32_t(packet_data, &particle_count);
        uint32_t data_length;
        if (particle_id == 36) {
            data_length = 2;
        } else if (particle_id == 37 || particle_id == 38) {
            data_length = 1;
        } else {
            data_length = 0;
        }
        data = malloc(data_length * sizeof(vint32_t));
        for (int i = 0; i < data_length; i++) {
            packet_data = _read_vint32(packet_data, &data[i]);
        }

        bot->callbacks.clientbound_play_particle_cb(
                bot,
                particle_id,
                long_distance,
                x,
                y,
                z,
                offset_x,
                offset_y,
                offset_z,
                particle_data,
                particle_count,
                data_length,
                data
                );
    }
}

void deserialize_clientbound_play_join_game(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_join_game_cb != NULL) {
        int32_t entity_id;
        uint8_t gamemode;
        int32_t dimension;
        uint8_t difficulty;
        uint8_t max_players;
        char *level_type;
        bool reduced_debug_info;

        packet_data = _read_int32_t(packet_data, &entity_id);
        packet_data = _read(packet_data, &gamemode, sizeof(gamemode));
        packet_data = _read_int32_t(packet_data, &dimension);
        packet_data = _read(packet_data, &difficulty, sizeof(difficulty));
        packet_data = _read(packet_data, &max_players, sizeof(max_players));
        packet_data = _read_string(packet_data, &level_type, NULL);
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        reduced_debug_info = b ? true : false;

        bot->callbacks.clientbound_play_join_game_cb(
                bot,
                entity_id,
                gamemode,
                dimension,
                difficulty,
                max_players,
                level_type,
                reduced_debug_info
                );
    }
}

void deserialize_clientbound_play_map(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_map_cb != NULL) {
        vint32_t item_damage;
        int8_t scale;
        bool tracking_position;
        vint32_t icon_count;
        struct map_icon *icons;
        int8_t columns, rows, x, z;
        vint32_t length;
        uint8_t *data;

        packet_data = _read_vint32(packet_data, &item_damage);
        packet_data = _read(packet_data, &scale, sizeof(scale));
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        tracking_position = b ? true : false;
        packet_data = _read_vint32(packet_data, &icon_count);
        icons = malloc(icon_count * sizeof(struct map_icon));
        for (int i = 0; i < icon_count; i++) {
            packet_data = _read(packet_data, &icons[i].direction_and_type, sizeof(icons[i].direction_and_type));
            packet_data = _read(packet_data, &icons[i].x, sizeof(icons[i].x));
            packet_data = _read(packet_data, &icons[i].z, sizeof(icons[i].z));
        }
        packet_data = _read(packet_data, &columns, sizeof(columns));
        if (columns > 0) {
            packet_data = _read(packet_data, &rows, sizeof(rows));
            packet_data = _read(packet_data, &x, sizeof(x));
            packet_data = _read(packet_data, &z, sizeof(z));
            packet_data = _read_vint32(packet_data, &length);
            data = malloc(length);
            packet_data = _read(packet_data, data, length);
        }

        bot->callbacks.clientbound_play_map_cb(
                bot,
                item_damage,
                scale,
                tracking_position,
                icon_count,
                icons,
                columns,
                rows,
                x,
                z,
                length,
                data
                );
    }
}

void deserialize_clientbound_play_entity_relative_move(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_relative_move_cb != NULL) {
        vint32_t entity_id;
        int16_t delta_x, delta_y, delta_z;
        bool on_ground;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_int16_t(packet_data, &delta_x);
        packet_data = _read_int16_t(packet_data, &delta_y);
        packet_data = _read_int16_t(packet_data, &delta_z);
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        on_ground = b ? true : false;

        bot->callbacks.clientbound_play_entity_relative_move_cb(
                bot,
                entity_id,
                delta_x,
                delta_y,
                delta_z,
                on_ground
                );
    }
}

void deserialize_clientbound_play_entity_look_and_relative_move(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_look_and_relative_move_cb != NULL) {
        vint32_t entity_id;
        int16_t delta_x, delta_y, delta_z;
        uint8_t yaw, pitch;
        bool on_ground;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_int16_t(packet_data, &delta_x);
        packet_data = _read_int16_t(packet_data, &delta_y);
        packet_data = _read_int16_t(packet_data, &delta_z);
        packet_data = _read(packet_data, &yaw, sizeof(yaw));
        packet_data = _read(packet_data, &yaw, sizeof(pitch));
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        on_ground = b ? true : false;

        bot->callbacks.clientbound_play_entity_look_and_relative_move_cb(
                bot,
                entity_id,
                delta_x,
                delta_y,
                delta_z,
                yaw,
                pitch,
                on_ground
                );
    }
}

void deserialize_clientbound_play_entity_look(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_look_cb != NULL) {
        vint32_t entity_id;
        uint8_t yaw;
        uint8_t pitch;
        bool on_ground;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, &yaw, sizeof(yaw));
        packet_data = _read(packet_data, &pitch, sizeof(pitch));
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        on_ground = b ? true : false;

        bot->callbacks.clientbound_play_entity_look_cb(
                bot,
                entity_id,
                yaw,
                pitch,
                on_ground
                );
    }
}

void deserialize_clientbound_play_entity(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_cb != NULL) {
        vint32_t entity_id;

        packet_data = _read_vint32(packet_data, &entity_id);

        bot->callbacks.clientbound_play_entity_cb(
                bot,
                entity_id
                );
    }
}

void deserialize_clientbound_play_vehicle_move(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_vehicle_move_cb != NULL) {
        double x, y, z;
        uint8_t yaw, pitch;

        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);
        packet_data = _read(packet_data, &yaw, sizeof(yaw));
        packet_data = _read(packet_data, &pitch, sizeof(pitch));

        bot->callbacks.clientbound_play_vehicle_move_cb(
                bot,
                x,
                y,
                z,
                yaw,
                pitch
                );
    }
}

void deserialize_clientbound_play_open_sign_editor(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_open_sign_editor_cb != NULL) {
        position_t location;

        packet_data = _read_uint64_t(packet_data, &location);

        bot->callbacks.clientbound_play_open_sign_editor_cb(
                bot,
                location
                );
    }
}

void deserialize_clientbound_play_player_abilities(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_player_abilities_cb != NULL) {
        int8_t flags;
        float flying_speed, field_of_view_modifier;

        packet_data = _read(packet_data, &flags, sizeof(flags));
        packet_data = _read_float(packet_data, &flying_speed);
        packet_data = _read_float(packet_data, &field_of_view_modifier);

        bot->callbacks.clientbound_play_player_abilities_cb(
                bot,
                flags,
                flying_speed,
                field_of_view_modifier
                );
    }
}

char *_read_combat_event(char *packet_data, enum COMBAT_EVENT_TYPE type, struct combat_event *event) {
    event->type = type;
    switch(type) {
        case COMBAT_EVENT_ENTER_COMBAT:
            /* no fields */
            break;
        case COMBAT_EVENT_END_COMBAT:
            packet_data = _read_vint32(packet_data, &event->end_combat.duration);
            packet_data = _read_int32_t(packet_data, &event->end_combat.entity_id);
            break;
        case COMBAT_EVENT_ENTITY_DEAD:
            packet_data = _read_vint32(packet_data, &event->entity_dead.player_id);
            packet_data = _read_int32_t(packet_data, &event->entity_dead.entity_id);
            packet_data = _read_string(packet_data, &event->entity_dead.message, NULL);
            break;
    }
    return packet_data;
}

void deserialize_clientbound_play_combat_event(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_combat_event_cb != NULL) {
        vint32_t event_type;
        struct combat_event event;

        packet_data = _read_vint32(packet_data, &event_type);
        packet_data = _read_combat_event(packet_data, event_type, &event);

        bot->callbacks.clientbound_play_combat_event_cb(
                bot,
                &event
                );
    }
}

char *_read_player_list_actions(char *packet_data, enum PLAYER_LIST_ACTION_TYPE type, int32_t number_of_players, struct player_list_action *player_actions) {
    for (int i = 0; i < number_of_players; i++) {
        packet_data = _read(packet_data, player_actions[i].uuid, sizeof(player_actions[i].uuid));
        switch(type) {
            case PLAYER_LIST_ADD_PLAYER:
            {
                struct player_list_action_add_player *add_player = &player_actions[i].add_player;
                packet_data = _read_string(packet_data, &add_player->name, NULL);
                packet_data = _read_vint32(packet_data, &add_player->number_of_properties);
                add_player->properties = malloc(add_player->number_of_properties * sizeof(struct player_property));
                for (int j = 0; j < add_player->number_of_properties; j++) {
                    struct player_property *property = &add_player->properties[i];
                    packet_data = _read_string(packet_data, &property->name, NULL);
                    packet_data = _read_string(packet_data, &property->value, NULL);
                    int8_t b;
                    packet_data = _read(packet_data, &b, sizeof(b));
                    property->is_signed = b ? true : false;
                    if (property->is_signed) {
                        packet_data = _read_string(packet_data, &property->signature, NULL);
                    }
                }
                packet_data = _read_vint32(packet_data, &add_player->gamemode);
                packet_data = _read_vint32(packet_data, &add_player->ping);
                int8_t b;
                packet_data = _read(packet_data, &b, sizeof(b));
                add_player->has_display_name = b ? true : false;
                if (add_player->has_display_name) {
                    packet_data = _read_string(packet_data, &add_player->display_name, NULL);
                }
                break;
            }
            case PLAYER_LIST_UPDATE_GAMEMODE:
                packet_data = _read_vint32(packet_data, &player_actions[i].update_gamemode.gamemode);
                break;
            case PLAYER_LIST_UPDATE_LATENCY:
                packet_data = _read_vint32(packet_data, &player_actions[i].update_latency.ping);
                break;
            case PLAYER_LIST_UPDATE_DISPLAY_NAME:
            {
                int8_t b;
                packet_data = _read(packet_data, &b, sizeof(b));
                player_actions[i].update_display_name.has_display_name = b ? true : false;
                if (player_actions[i].update_display_name.has_display_name) {
                    packet_data = _read_string(packet_data, &player_actions[i].update_display_name.display_name, NULL);
                }
                break;
            }
            case PLAYER_LIST_REMOVE_PLAYER:
                /* no fields */
                break;
        }
    }
    return packet_data;
}

void deserialize_clientbound_play_player_list_item(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_player_list_item_cb != NULL) {
        vint32_t action_type;
        vint32_t number_of_players;
        struct player_list_action *player_actions;

        packet_data = _read_vint32(packet_data, &action_type);
        packet_data = _read_vint32(packet_data, &number_of_players);
        player_actions = malloc(number_of_players * sizeof(struct player_list_action));
        packet_data = _read_player_list_actions(packet_data, action_type, number_of_players, player_actions);
        
        bot->callbacks.clientbound_play_player_list_item_cb(
                bot,
                action_type,
                number_of_players,
                player_actions
                );
    }
}

void deserialize_clientbound_play_player_position_and_look(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_player_position_and_look_cb != NULL) {
        double x, y, z;
        float yaw, pitch;
        int8_t flags;
        vint32_t teleport_id;

        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);
        packet_data = _read_float(packet_data, &yaw);
        packet_data = _read_float(packet_data, &pitch);
        packet_data = _read(packet_data, &flags, sizeof(flags));
        packet_data = _read_vint32(packet_data, &teleport_id);

        bot->callbacks.clientbound_play_player_position_and_look_cb(
                bot,
                x,
                y,
                z,
                yaw,
                pitch,
                flags,
                teleport_id
                );
    }
}

void deserialize_clientbound_play_use_bed(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_use_bed_cb != NULL) {
        vint32_t entity_id;
        position_t location;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_uint64_t(packet_data, &location);

        bot->callbacks.clientbound_play_use_bed_cb(
                bot,
                entity_id,
                location
                );
    }
}

void deserialize_clientbound_play_destroy_entities(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_destroy_entities_cb != NULL) {
        vint32_t count;
        vint32_t *entity_ids;

        packet_data = _read_vint32(packet_data, &count);
        entity_ids = malloc(count * sizeof(vint32_t));
        for (int i = 0; i < count; i++) {
            packet_data = _read_vint32(packet_data, &entity_ids[i]);
        }

        bot->callbacks.clientbound_play_destroy_entities_cb(
                bot,
                count,
                entity_ids
                );
    }
}

void deserialize_clientbound_play_remove_entity_effect(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_remove_entity_effect_cb != NULL) {
        vint32_t entity_id;
        int8_t effect_id;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, &effect_id, sizeof(effect_id));

        bot->callbacks.clientbound_play_remove_entity_effect_cb(
                bot,
                entity_id,
                effect_id
                );
    }
}

void deserialize_clientbound_play_resource_pack_send(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_resource_pack_send_cb != NULL) {
        char *url;
        char *hash;

        packet_data = _read_string(packet_data, &url, NULL);
        packet_data = _read_string(packet_data, &hash, NULL);

        bot->callbacks.clientbound_play_resource_pack_send_cb(
                bot,
                url,
                hash
                );
    }
}

void deserialize_clientbound_play_respawn(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_respawn_cb != NULL) {
        int32_t dimension;
        uint8_t difficulty;
        uint8_t gamemode;
        char *level_type;

        packet_data = _read_int32_t(packet_data, &dimension);
        packet_data = _read(packet_data, &difficulty, sizeof(difficulty));
        packet_data = _read(packet_data, &gamemode, sizeof(difficulty));
        packet_data = _read_string(packet_data, &level_type, NULL);

        bot->callbacks.clientbound_play_respawn_cb(
                bot,
                dimension,
                difficulty,
                gamemode,
                level_type
                );
    }
}

void deserialize_clientbound_play_entity_head_look(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_head_look_cb != NULL) {
        vint32_t entity_id;
        uint8_t head_yaw;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, &head_yaw, sizeof(head_yaw));

        bot->callbacks.clientbound_play_entity_head_look_cb(
                bot,
                entity_id,
                head_yaw
                );
    }
}

char *_read_world_border_action(char *packet_data, enum WORLD_BORDER_ACTION_TYPE type, struct world_border_action *action) {
    action->type = type; 
    switch(type) {
        case WORLD_BORDER_SET_SIZE:
            packet_data = _read_double(packet_data, &action->set_size.diameter);
            break;
        case WORLD_BORDER_LERP_SIZE:
            packet_data = _read_double(packet_data, &action->lerp_size.old_diameter);
            packet_data = _read_double(packet_data, &action->lerp_size.new_diameter);
            packet_data = _read_vint64(packet_data, &action->lerp_size.speed);
            break;
        case WORLD_BORDER_SET_CENTER:
            packet_data = _read_double(packet_data, &action->set_center.x);
            packet_data = _read_double(packet_data, &action->set_center.z);
            break;
        case WORLD_BORDER_INITIALIZE:
            packet_data = _read_double(packet_data, &action->initialize.x);
            packet_data = _read_double(packet_data, &action->initialize.z);
            packet_data = _read_double(packet_data, &action->initialize.old_diameter);
            packet_data = _read_double(packet_data, &action->initialize.new_diameter);
            packet_data = _read_vint64(packet_data, &action->initialize.speed);
            packet_data = _read_vint32(packet_data, &action->initialize.portal_teleport_boundary);
            packet_data = _read_vint32(packet_data, &action->initialize.warning_time);
            packet_data = _read_vint32(packet_data, &action->initialize.warning_blocks);
            break;
        case WORLD_BORDER_SET_WARNING_TIME:
            packet_data = _read_vint32(packet_data, &action->set_warning_time.warning_time);
            break;
        case WORLD_BORDER_SET_WARNING_BLOCKS:
            packet_data = _read_vint32(packet_data, &action->set_warning_blocks.warning_blocks);
            break;
    }
    return packet_data;
}

void deserialize_clientbound_play_world_border(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_world_border_cb != NULL) {
        vint32_t action_type;
        struct world_border_action action;

        packet_data = _read_vint32(packet_data, &action_type);
        packet_data = _read_world_border_action(packet_data, action_type, &action); 

        bot->callbacks.clientbound_play_world_border_cb(
                bot,
                &action
                );
    }
}

void deserialize_clientbound_play_camera(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_camera_cb != NULL) {
        vint32_t camera_id;

        packet_data = _read_vint32(packet_data, &camera_id);

        bot->callbacks.clientbound_play_camera_cb(
                bot,
                camera_id
                );
    }
}

void deserialize_clientbound_play_held_item_change(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_held_item_change_cb != NULL) {
        int8_t slot;

        packet_data = _read(packet_data, &slot, sizeof(slot));

        bot->callbacks.clientbound_play_held_item_change_cb(
                bot,
                slot
                );
    }
}

void deserialize_clientbound_play_display_scoreboard(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_display_scoreboard_cb != NULL) {
        int8_t position;
        char *score_name;

        packet_data = _read(packet_data, &position, sizeof(position));
        packet_data = _read_string(packet_data, &score_name, NULL);
    }
}

void deserialize_clientbound_play_entity_metadata(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_metadata_cb != NULL) {
        vint32_t entity_id;
        struct entity_metadata metadata;
        
        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_entity_metadata(packet_data, &metadata);
        
        bot->callbacks.clientbound_play_entity_metadata_cb(
                bot,
                entity_id,
                &metadata
                );
    }
}

void deserialize_clientbound_play_attach_entity(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_attach_entity_cb != NULL) {
        int32_t attached_entity_id, holding_entity_id;

        packet_data = _read_int32_t(packet_data, &attached_entity_id);
        packet_data = _read_int32_t(packet_data, &holding_entity_id);

        bot->callbacks.clientbound_play_attach_entity_cb(
                bot,
                attached_entity_id,
                holding_entity_id
                );
    }
}

void deserialize_clientbound_play_entity_velocity(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_velocity_cb != NULL) {
        vint32_t entity_id;
        int16_t velocity_x, velocity_y, velocity_z;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_int16_t(packet_data, &velocity_x);
        packet_data = _read_int16_t(packet_data, &velocity_y);
        packet_data = _read_int16_t(packet_data, &velocity_z);

        bot->callbacks.clientbound_play_entity_velocity_cb(
                bot,
                entity_id,
                velocity_x,
                velocity_y,
                velocity_z
                );
    }
}

void deserialize_clientbound_play_entity_equipment(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_equipment_cb != NULL) {
        vint32_t entity_id, slot;
        struct slot_type item;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_vint32(packet_data, &slot);
        packet_data = _read_slot(packet_data, &item);

        bot->callbacks.clientbound_play_entity_equipment_cb(
                bot,
                entity_id,
                slot,
                &item
                );
    }
}

void deserialize_clientbound_play_set_experience(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_set_experience_cb != NULL) {
        float experience_bar;
        vint32_t level, total_experience;

        packet_data = _read_float(packet_data, &experience_bar);
        packet_data = _read_vint32(packet_data, &level);
        packet_data = _read_vint32(packet_data, &total_experience);

        bot->callbacks.clientbound_play_set_experience_cb(
                bot,
                experience_bar,
                level,
                total_experience
                );
    }
}

void deserialize_clientbound_play_update_health(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_update_health_cb != NULL) {
        float health;
        vint32_t food;
        float food_saturation;

        packet_data = _read_float(packet_data, &health);
        packet_data = _read_vint32(packet_data, &food);
        packet_data = _read_float(packet_data, &food_saturation);

        bot->callbacks.clientbound_play_update_health_cb(
                bot,
                health,
                food,
                food_saturation
                );
    }
}

void deserialize_clientbound_play_scoreboard_objective(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_scoreboard_objective_cb != NULL) {
        char *objective_name;
        int8_t mode;
        char *objective_value = NULL;
        char *type = NULL;
        
        packet_data = _read_string(packet_data, &objective_name, NULL);
        packet_data = _read(packet_data, &mode, sizeof(mode));
        if (mode == 0 || mode == 2) {
            packet_data = _read_string(packet_data, &objective_value, NULL);
            packet_data = _read_string(packet_data, &type, NULL);
        }
        
        bot->callbacks.clientbound_play_scoreboard_objective_cb(
                bot,
                objective_name,
                mode,
                objective_value,
                type
                );
    }
}

void deserialize_clientbound_play_set_passengers(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_set_passengers_cb != NULL) {
        vint32_t entity_id, passenger_count;
        vint32_t *passengers;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_vint32(packet_data, &passenger_count);
        passengers = malloc(passenger_count * sizeof(vint32_t));
        for (int i = 0; i < passenger_count; i++) {
            packet_data = _read_vint32(packet_data, &passengers[i]);
        }

        bot->callbacks.clientbound_play_set_passengers_cb(
                bot,
                entity_id,
                passenger_count,
                passengers
                );
    }
}

char *_read_team_action(char *packet_data, enum TEAM_ACTION_TYPE type, struct team_action *action) {
    action->type = type;
    switch(type) {
        case TEAM_ACTION_CREATE_TEAM:
        {
            struct team_action_create_team *create_team = &action->create_team;
            packet_data = _read_string(packet_data, &create_team->team_display_name, NULL);
            packet_data = _read_string(packet_data, &create_team->team_prefix, NULL);
            packet_data = _read_string(packet_data, &create_team->team_suffix, NULL);
            packet_data = _read(packet_data, &create_team->friendly_flags, sizeof(create_team->friendly_flags));
            packet_data = _read_string(packet_data, &create_team->name_tag_visibility, NULL);
            packet_data = _read_string(packet_data, &create_team->collision_rule, NULL);
            packet_data = _read(packet_data, &create_team->color, sizeof(create_team->color));
            packet_data = _read_vint32(packet_data, &create_team->player_count);
            create_team->players = malloc(create_team->player_count * sizeof(char *));
            for (int i = 0; i < create_team->player_count; i++) {
                packet_data = _read_string(packet_data, &create_team->players[i], NULL);
            }
            break;
        }
        case TEAM_ACTION_REMOVE_TEAM:
            /* no fields */
            break;
        case TEAM_ACTION_UPDATE_TEAM_INFO:
        {
            struct team_action_update_team_info *update_team_info = &action->update_team_info;
            packet_data = _read_string(packet_data, &update_team_info->team_display_name, NULL);
            packet_data = _read_string(packet_data, &update_team_info->team_prefix, NULL);
            packet_data = _read_string(packet_data, &update_team_info->team_suffix, NULL);
            packet_data = _read(packet_data, &update_team_info->friendly_flags, sizeof(update_team_info->friendly_flags));
            packet_data = _read_string(packet_data, &update_team_info->name_tag_visibility, NULL);
            packet_data = _read_string(packet_data, &update_team_info->collision_rule, NULL);
            packet_data = _read(packet_data, &update_team_info->color, sizeof(update_team_info->color));
            break;
        }
        case TEAM_ACTION_ADD_PLAYERS_TO_TEAM:
        {
            struct team_action_add_players_to_team *add_players_to_team = &action->add_players_to_team;
            packet_data = _read_vint32(packet_data, &add_players_to_team->player_count);
            add_players_to_team->players = malloc(add_players_to_team->player_count * sizeof(char *));
            for (int i = 0; i < add_players_to_team->player_count; i++) {
                packet_data = _read_string(packet_data, &add_players_to_team->players[i], NULL);
            }
            break;
        }
        case TEAM_ACTION_REMOVE_PLAYERS_FROM_TEAM:
        {
            struct team_action_remove_players_from_team *remove_players_from_team = &action->remove_players_from_team;
            packet_data = _read_vint32(packet_data, &remove_players_from_team->player_count);
            remove_players_from_team->players = malloc(remove_players_from_team->player_count * sizeof(char *));
            for (int i = 0; i < remove_players_from_team->player_count; i++) {
                packet_data = _read_string(packet_data, &remove_players_from_team->players[i], NULL);
            }

            break;
        }
    }
    return packet_data;
}

void deserialize_clientbound_play_teams(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_teams_cb != NULL) {
        char *team_name;
        int8_t mode;
        struct team_action action;

        packet_data = _read_string(packet_data, &team_name, NULL);
        packet_data = _read(packet_data, &mode, sizeof(mode));
        packet_data = _read_team_action(packet_data, mode, &action);

        bot->callbacks.clientbound_play_teams_cb(
                bot,
                team_name,
                &action
                );
    }
}

void deserialize_clientbound_play_update_score(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_update_score_cb != NULL) {
        char *score_name;
        int8_t action;
        char *objective_name;
        vint32_t value;

        packet_data = _read_string(packet_data, &score_name, NULL);
        packet_data = _read(packet_data, &action, sizeof(action));
        packet_data = _read_string(packet_data, &objective_name, NULL);
        if (action != 1) {
            packet_data = _read_vint32(packet_data, &value);
        }

        bot->callbacks.clientbound_play_update_score_cb(
                bot,
                score_name,
                action,
                objective_name,
                value
                );
    }
}

void deserialize_clientbound_play_spawn_position(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_spawn_position_cb != NULL) {
        position_t location;

        packet_data = _read_uint64_t(packet_data, &location);

        bot->callbacks.clientbound_play_spawn_position_cb(
                bot,
                location
                );
    }
}

void deserialize_clientbound_play_time_update(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_time_update_cb != NULL) {
        int64_t world_age, time_of_day;

        packet_data = _read_int64_t(packet_data, &world_age);
        packet_data = _read_int64_t(packet_data, &time_of_day);

        bot->callbacks.clientbound_play_time_update_cb(
                bot,
                world_age,
                time_of_day
                );
    }
}

char *_read_title_action(char *packet_data, enum TITLE_ACTION_TYPE type, struct title_action *action) {
    action->type = type;
    switch(type) {
        case TITLE_ACTION_SET_TITLE:
            packet_data = _read_string(packet_data, &action->set_title.title_text, NULL);
            break;
        case TITLE_ACTION_SET_SUBTITLE:
            packet_data = _read_string(packet_data, &action->set_subtitle.subtitle_text, NULL);
            break;
        case TITLE_ACTION_SET_TIMES_AND_DISPLAY:
            packet_data = _read_int32_t(packet_data, &action->set_times_and_display.fade_in);
            packet_data = _read_int32_t(packet_data, &action->set_times_and_display.stay);
            packet_data = _read_int32_t(packet_data, &action->set_times_and_display.fade_out);
            break;
        case TITLE_ACTION_HIDE:
            /* no fields */
            break;
        case TITLE_ACTION_RESET:
            /* no fields */
            break;
    }
    return packet_data;
}

void deserialize_clientbound_play_title(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_title_cb != NULL) {
        vint32_t action_type;
        struct title_action action;

        packet_data = _read_vint32(packet_data, &action_type);
        packet_data = _read_title_action(packet_data, action_type, &action);

        bot->callbacks.clientbound_play_title_cb(
                bot,
                &action
                );
    }
}

void deserialize_clientbound_play_sound_effect(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_sound_effect_cb != NULL) {
        vint32_t sound_id, sound_category;
        int32_t effect_position_x, effect_position_y, effect_position_z;
        float volume;
        uint8_t pitch;

        packet_data = _read_vint32(packet_data, &sound_id);
        packet_data = _read_vint32(packet_data, &sound_category);
        packet_data = _read_int32_t(packet_data, &effect_position_x);
        packet_data = _read_int32_t(packet_data, &effect_position_y);
        packet_data = _read_int32_t(packet_data, &effect_position_z);
        packet_data = _read_float(packet_data, &volume);
        packet_data = _read(packet_data, &pitch, sizeof(pitch));

        bot->callbacks.clientbound_play_sound_effect_cb(
                bot,
                sound_id,
                sound_category,
                effect_position_x,
                effect_position_y,
                effect_position_z,
                volume,
                pitch
                );
    }
}

void deserialize_clientbound_play_player_list_header_and_footer(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_player_list_header_and_footer_cb != NULL) {
        char *header, *footer;

        packet_data = _read_string(packet_data, &header, NULL);
        packet_data = _read_string(packet_data, &footer, NULL);

        bot->callbacks.clientbound_play_player_list_header_and_footer_cb(
                bot,
                header,
                footer
                );
    }
}

void deserialize_clientbound_play_collect_item(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_collect_item_cb != NULL) {
        vint32_t collected_entity_id;
        vint32_t collector_entity_id;

        packet_data = _read_vint32(packet_data, &collected_entity_id);
        packet_data = _read_vint32(packet_data, &collector_entity_id);

        bot->callbacks.clientbound_play_collect_item_cb(
                bot,
                collected_entity_id,
                collector_entity_id
                );
    }
}

void deserialize_clientbound_play_entity_teleport(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_teleport_cb != NULL) {
        vint32_t entity_id;
        double x, y, z;
        uint8_t yaw, pitch;
        bool on_ground;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_double(packet_data, &x);
        packet_data = _read_double(packet_data, &y);
        packet_data = _read_double(packet_data, &z);
        packet_data = _read(packet_data, &yaw, sizeof(yaw));
        packet_data = _read(packet_data, &pitch, sizeof(pitch));
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        on_ground = b ? true : false;

        bot->callbacks.clientbound_play_entity_teleport_cb(
                bot,
                entity_id,
                x,
                y,
                z,
                yaw,
                pitch,
                on_ground
                );
    }
}

char *_read_entity_property(char *packet_data, struct entity_property *property) {
    packet_data = _read_string(packet_data, &property->key, NULL);
    packet_data = _read_double(packet_data, &property->value);
    packet_data = _read_vint32(packet_data, &property->number_of_modifiers);
    property->modifiers = malloc(property->number_of_modifiers * sizeof(struct entity_modifier));
    for (int i = 0; i < property->number_of_modifiers; i++) {
        packet_data = _read(packet_data, &property->modifiers[i].uuid, sizeof(property->modifiers[i].uuid));
        packet_data = _read_double(packet_data, &property->modifiers[i].amount);
        packet_data = _read(packet_data, &property->modifiers[i].operation, sizeof(property->modifiers[i].operation));
    }
    return packet_data;
}

void deserialize_clientbound_play_entity_properties(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_properties_cb != NULL) {
        vint32_t entity_id;
        int32_t number_of_properties;
        struct entity_property *properties;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read_int32_t(packet_data, &number_of_properties);
        properties = malloc(number_of_properties * sizeof(struct entity_property));
        for (int i = 0; i < number_of_properties; i++) {
            packet_data = _read_entity_property(packet_data, &properties[i]);
        }
        
        bot->callbacks.clientbound_play_entity_properties_cb(
                bot,
                entity_id,
                number_of_properties,
                properties
                );
    }
}

void deserialize_clientbound_play_entity_effect(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_entity_effect_cb != NULL) {
        vint32_t entity_id;
        int8_t effect_id, amplifier;
        vint32_t duration;
        bool hide_particles;

        packet_data = _read_vint32(packet_data, &entity_id);
        packet_data = _read(packet_data, &effect_id, sizeof(effect_id));
        packet_data = _read(packet_data, &amplifier, sizeof(amplifier));
        packet_data = _read_vint32(packet_data, &duration);
        int8_t b;
        packet_data = _read(packet_data, &b, sizeof(b));
        hide_particles = b ? true : false;

        bot->callbacks.clientbound_play_entity_effect_cb(
                bot,
                entity_id,
                effect_id,
                amplifier,
                duration,
                hide_particles
                );
    }
}

void dispatch_packet_cb(struct bot_agent *bot) {
    char *packet_data = bot->packet_data;
    vint32_t packet_id;
    packet_data = _read_vint32(packet_data, &packet_id);
    switch (bot->current_state) {
        case LOGIN:
            switch (packet_id) {
                case 0x00: /* clientbound login disconnect */
                    deserialize_clientbound_login_disconnect(packet_data, bot);
                    break;
                case 0x01: /* clientbound login encryption request */
                    deserialize_clientbound_login_encryption_request(packet_data, bot);
                    break;
                case 0x02: /* clientbound login login success */
                    deserialize_clientbound_login_login_success(packet_data, bot);
                    break;
                case 0x03: /* clientbound login set compression */
                    deserialize_clientbound_login_set_compression(packet_data, bot);
                    break;
            }
            break;
        case STATUS:
            switch (packet_id) {
                case 0x00: /* clientbound status response */
                    deserialize_clientbound_status_response(packet_data, bot);
                    break;
                case 0x01:
                    deserialize_clientbound_status_pong(packet_data, bot);
                    break;
            }
            break;
        case PLAY:
            switch (packet_id) {
                case 0x00:
                    deserialize_clientbound_play_spawn_object(packet_data, bot);
                    break;
                case 0x01:
                    deserialize_clientbound_play_spawn_experience_orb(packet_data, bot);
                    break;
                case 0x02:
                    deserialize_clientbound_play_spawn_global_entity(packet_data, bot);
                    break;
                case 0x03:
                    deserialize_clientbound_play_spawn_mob(packet_data, bot);
                    break;
                case 0x04:
                    deserialize_clientbound_play_spawn_painting(packet_data, bot);
                    break;
                case 0x05:
                    deserialize_clientbound_play_spawn_player(packet_data, bot);
                    break;
                case 0x06:
                    deserialize_clientbound_play_animation(packet_data, bot);
                    break;
                case 0x07:
                    deserialize_clientbound_play_statistics(packet_data, bot);
                    break;
                case 0x08:
                    deserialize_clientbound_play_block_break_animation(packet_data, bot);
                    break;
                case 0x09:
                    deserialize_clientbound_play_update_block_entity(packet_data, bot);
                    break;
                case 0x0a:
                    deserialize_clientbound_play_block_action(packet_data, bot);
                    break;
                case 0x0b:
                    deserialize_clientbound_play_block_change(packet_data, bot);
                    break;
                case 0x0c:
                    deserialize_clientbound_play_boss_bar(packet_data, bot);
                    break;
                case 0x0d:
                    deserialize_clientbound_play_server_difficulty(packet_data, bot);
                    break;
                case 0x0e:
                    deserialize_clientbound_play_tab_complete(packet_data, bot);
                    break;
                case 0x0f:
                    deserialize_clientbound_play_chat_message(packet_data, bot);
                    break;
                case 0x10:
                    deserialize_clientbound_play_multi_block_change(packet_data, bot);
                    break;
                case 0x11:
                    deserialize_clientbound_play_confirm_transaction(packet_data, bot);
                    break;
                case 0x12:
                    deserialize_clientbound_play_close_window(packet_data, bot);
                    break;
                case 0x13:
                    deserialize_clientbound_play_open_window(packet_data, bot);
                    break;
                case 0x14:
                    deserialize_clientbound_play_window_items(packet_data, bot);
                    break;
                case 0x15:
                    deserialize_clientbound_play_window_property(packet_data, bot);
                    break;
                case 0x16:
                    deserialize_clientbound_play_set_slot(packet_data, bot);
                    break;
                case 0x17:
                    deserialize_clientbound_play_set_cooldown(packet_data, bot);
                    break;
                case 0x18:
                    deserialize_clientbound_play_plugin_message(packet_data, bot);
                    break;
                case 0x19:
                    deserialize_clientbound_play_named_sound_effect(packet_data, bot);
                    break;
                case 0x1a:
                    deserialize_clientbound_play_disconnect(packet_data, bot);
                    break;
                case 0x1b:
                    deserialize_clientbound_play_entity_status(packet_data, bot);
                    break;
                case 0x1c:
                    deserialize_clientbound_play_explosion(packet_data, bot);
                    break;
                case 0x1d:
                    deserialize_clientbound_play_unload_chunk(packet_data, bot);
                    break;
                case 0x1e:
                    deserialize_clientbound_play_change_game_state(packet_data, bot);
                    break;
                case 0x1f:
                    deserialize_clientbound_play_keep_alive(packet_data, bot);
                    break;
                case 0x20:
                    deserialize_clientbound_play_chunk_data(packet_data, bot);
                    break;
                case 0x21:
                    deserialize_clientbound_play_effect(packet_data, bot);
                    break;
                case 0x22:
                    deserialize_clientbound_play_particle(packet_data, bot);
                    break;
                case 0x23:
                    deserialize_clientbound_play_join_game(packet_data, bot);
                    break;
                case 0x24:
                    deserialize_clientbound_play_map(packet_data, bot);
                    break;
                case 0x25:
                    deserialize_clientbound_play_entity_relative_move(packet_data, bot);
                    break;
                case 0x26:
                    deserialize_clientbound_play_entity_look_and_relative_move(packet_data, bot);
                    break;
                case 0x27:
                    deserialize_clientbound_play_entity_look(packet_data, bot);
                    break;
                case 0x28:
                    deserialize_clientbound_play_entity(packet_data, bot);
                    break;
                case 0x29:
                    deserialize_clientbound_play_vehicle_move(packet_data, bot);
                    break;
                case 0x2a:
                    deserialize_clientbound_play_open_sign_editor(packet_data, bot);
                    break;
                case 0x2b:
                    deserialize_clientbound_play_player_abilities(packet_data, bot);
                    break;
                case 0x2c:
                    deserialize_clientbound_play_combat_event(packet_data, bot);
                    break;
                case 0x2d:
                    deserialize_clientbound_play_player_list_item(packet_data, bot);
                    break;
                case 0x2e:
                    deserialize_clientbound_play_player_position_and_look(packet_data, bot);
                    break;
                case 0x2f:
                    deserialize_clientbound_play_use_bed(packet_data, bot);
                    break;
                case 0x30:
                    deserialize_clientbound_play_destroy_entities(packet_data, bot);
                    break;
                case 0x31:
                    deserialize_clientbound_play_remove_entity_effect(packet_data, bot);
                    break;
                case 0x32:
                    deserialize_clientbound_play_resource_pack_send(packet_data, bot);
                    break;
                case 0x33:
                    deserialize_clientbound_play_respawn(packet_data, bot);
                    break;
                case 0x34:
                    deserialize_clientbound_play_entity_head_look(packet_data, bot);
                    break;
                case 0x35:
                    deserialize_clientbound_play_world_border(packet_data, bot);
                    break;
                case 0x36:
                    deserialize_clientbound_play_camera(packet_data, bot);
                    break;
                case 0x37:
                    deserialize_clientbound_play_held_item_change(packet_data, bot);
                    break;
                case 0x38:
                    deserialize_clientbound_play_display_scoreboard(packet_data, bot);
                    break;
                case 0x39:
                    deserialize_clientbound_play_entity_metadata(packet_data, bot);
                    break;
                case 0x3a:
                    deserialize_clientbound_play_attach_entity(packet_data, bot);
                    break;
                case 0x3b:
                    deserialize_clientbound_play_entity_velocity(packet_data, bot);
                    break;
                case 0x3c:
                    deserialize_clientbound_play_entity_equipment(packet_data, bot);
                    break;
                case 0x3d:
                    deserialize_clientbound_play_set_experience(packet_data, bot);
                    break;
                case 0x3e:
                    deserialize_clientbound_play_update_health(packet_data, bot);
                    break;
                case 0x3f:
                    deserialize_clientbound_play_scoreboard_objective(packet_data, bot);
                    break;
                case 0x40:
                    deserialize_clientbound_play_set_passengers(packet_data, bot);
                    break;
                case 0x41:
                    deserialize_clientbound_play_teams(packet_data, bot);
                    break;
                case 0x42:
                    deserialize_clientbound_play_update_score(packet_data, bot);
                    break;
                case 0x43:
                    deserialize_clientbound_play_spawn_position(packet_data, bot);
                    break;
                case 0x44:
                    deserialize_clientbound_play_time_update(packet_data, bot);
                    break;
                case 0x45:
                    deserialize_clientbound_play_title(packet_data, bot);
                    break;
                case 0x46:
                    deserialize_clientbound_play_sound_effect(packet_data, bot);
                    break;
                case 0x47:
                    deserialize_clientbound_play_player_list_header_and_footer(packet_data, bot);
                    break;
                case 0x48:
                    deserialize_clientbound_play_collect_item(packet_data, bot);
                    break;
                case 0x49:
                    deserialize_clientbound_play_entity_teleport(packet_data, bot);
                    break;
                case 0x4a:
                    deserialize_clientbound_play_entity_properties(packet_data, bot);
                    break;
                case 0x4b:
                    deserialize_clientbound_play_entity_effect(packet_data, bot);
                    break;
            }
            break;
    }
}

/* It looks like we cannot make any assumptions about the data read into buf
 * buf could either not have enough data for a full minecraft packet, or it
 * could contain data for many packets. Kill me. 
 * Precondition: buf->len > 5
 **/
void read_socket(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
}
