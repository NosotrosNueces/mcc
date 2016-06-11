#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>
#include "protocol.h"
#include "types.h"
#include "serial.h"

#define PAD_LENGTH(x) (x = (char *)x + 5)
#define DEFAULT_PACKET_LENGTH (1 << 10)


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

int32_t send_play_serverbound_keepalive(
        struct bot_agent*        bot,
        vint32_t      keepalive_id
        )
{
    struct packet_write_buffer buf;
    init_packet_write_buffer(&buf, DEFAULT_PACKET_LENGTH);
    pad_length(&buf);
    char *data_start = buf.ptr;

    _push_vint32(&buf, 0x0B);
    _push_vint32(&buf, keepalive_id);

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
                packet_ptr = _read_int64_t(packet_ptr, &current_entry->entity_position);
                break;
            case ENTITY_METADATA_OPTPOSITION:
            {
                int8_t boolean;
                packet_ptr = _read(packet_ptr, &boolean, sizeof(boolean));
                current_entry->entity_opt_position.present = boolean ? true : false;
                packet_ptr = _read_int64_t(packet_ptr, &current_entry->entity_opt_position.position);
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
        packet_data = _read_int64_t(packet_data, &location);
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
        packet_data = _read_int64_t(packet_data, &location);
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
        
        packet_data = _read_int64_t(packet_data, &location);
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

        packet_data = _read_int64_t(packet_data, &location);
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

        packet_data = _read_int64_t(packet_data, &location);
        packet_data = _read_vint32(packet_data, &block_id);

        bot->callbacks.clientbound_play_block_change_cb(
                bot,
                location,
                block_id
                );
    }
}

void _read_boss_bar_action(char *packet_data, struct boss_bar_action *action_data) {
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
            struct boss_bar_update_health *update_health;
            packet_data = _read_float(packet_data, &update_health->health);
            break;
        }
        case BOSS_BAR_UPDATE_TITLE:
            break;
        case BOSS_BAR_UPDATE_STYLE:
            break;
        case BOSS_BAR_UPDATE_FLAGS:
            break;
    }
}

void deserialize_clientbound_play_boss_bar(char *packet_data, struct bot_agent *bot) {
    if (bot->callbacks.clientbound_play_boss_bar_cb != NULL) {
        char uuid[16];
        vint32_t action;
        struct boss_bar_action action_data;
        
        packet_data = _read(packet_data, uuid, sizeof(uuid));
        

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
                    break;
                case 0x01:
                    break;
                case 0x02:
                    break;
                case 0x03:
                    break;
                case 0x04:
                    break;
                case 0x05:
                    break;
                case 0x06:
                    break;
                case 0x07:
                    break;
                case 0x08:
                    break;
                case 0x09:
                    break;
                case 0x0a:
                    break;
                case 0x0b:
                    break;
                case 0x0c:
                    break;
                case 0x0d:
                    break;
                case 0x0e:
                    break;
                case 0x0f:
                    break;
                case 0x10:
                    break;
                case 0x11:
                    break;
                case 0x12:
                    break;
                case 0x13:
                    break;
                case 0x14:
                    break;
                case 0x15:
                    break;
                case 0x16:
                    break;
                case 0x17:
                    break;
                case 0x18:
                    break;
                case 0x19:
                    break;
                case 0x1a:
                    break;
                case 0x1b:
                    break;
                case 0x1c:
                    break;
                case 0x1d:
                    break;
                case 0x1e:
                    break;
                case 0x1f:
                    break;
                case 0x20:
                    break;
                case 0x21:
                    break;
                case 0x22:
                    break;
                case 0x23:
                    break;
                case 0x24:
                    break;
                case 0x25:
                    break;
                case 0x26:
                    break;
                case 0x27:
                    break;
                case 0x28:
                    break;
                case 0x29:
                    break;
                case 0x2a:
                    break;
                case 0x2b:
                    break;
                case 0x2c:
                    break;
                case 0x2d:
                    break;
                case 0x2e:
                    break;
                case 0x2f:
                    break;
                case 0x30:
                    break;
                case 0x31:
                    break;
                case 0x32:
                    break;
                case 0x33:
                    break;
                case 0x34:
                    break;
                case 0x35:
                    break;
                case 0x36:
                    break;
                case 0x37:
                    break;
                case 0x38:
                    break;
                case 0x39:
                    break;
                case 0x3a:
                    break;
                case 0x3b:
                    break;
                case 0x3c:
                    break;
                case 0x3d:
                    break;
                case 0x3e:
                    break;
                case 0x3f:
                    break;
                case 0x40:
                    break;
                case 0x41:
                    break;
                case 0x42:
                    break;
                case 0x43:
                    break;
                case 0x44:
                    break;
                case 0x45:
                    break;
                case 0x46:
                    break;
                case 0x47:
                    break;
                case 0x48:
                    break;
                case 0x49:
                    break;
                case 0x4a:
                    break;
                case 0x4b:
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
    struct bot_agent *bot = stream->data;
    char *packet_data = buf->base;
    size_t length = buf->len;
    size_t bytes_read = 0;
    while (bytes_read < length) {
        switch (bot->socket_read_state) {
            case SOCKET_READ_PACKET_START: /* reading from start of packet */
                {
                    /* read in the packet length */
                    int varint_length = varint32(packet_data, &bot->packet_length);
                    bytes_read += varint_length;
                    /* Check if bot buffer is long enough */
                    if (bot->packet_length > bot->packet_capacity) {
                        free(bot->packet_data);
                        bot->packet_data = malloc(bot->packet_length);
                        bot->packet_capacity = bot->packet_length;
                    }
                    /* Check if the full packet is available in buf */
                    if (bot->packet_length <= length - bytes_read) {
                        memcpy(bot->packet_data, &packet_data[bytes_read], bot->packet_length);
                        /* Parse the full packet, invoke callbacks */
                        bytes_read += bot->packet_length;
                    } else {
                        memcpy(bot->packet_data, &packet_data[bytes_read], length - bytes_read);
                        bytes_read = length;
                        bot->packet_bytes_read = length - bytes_read;
                        bot->socket_read_state = SOCKET_READ_PACKET_MIDDLE;
                    }
                    break;
                }
            case SOCKET_READ_PACKET_MIDDLE: /* reading from middle of packet */
                {
                    /* Check if rest of packet is available in buf */
                    if (bot->packet_length - bot->packet_bytes_read <= length - bytes_read) {
                        memcpy(&bot->packet_data[bot->packet_bytes_read], &packet_data[bytes_read], bot->packet_length - bot->packet_bytes_read);
                        /* Parse the full packet, invoke callbacks */
                        bytes_read += bot->packet_length;
                        bot->socket_read_state = SOCKET_READ_PACKET_START;
                    } else {
                        memcpy(&bot->packet_data[bot->packet_bytes_read], &packet_data[bytes_read], length - bytes_read);
                        bytes_read = length;
                        bot->packet_bytes_read += length - bytes_read;
                    }
                }
        }
    }
}
