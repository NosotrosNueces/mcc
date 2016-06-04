#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>
#include "protocol.h"
#include "types.h"

#define PAD_LENGTH(x) (x = (char *)x + 5)
#define expect_more(x) (x & 0x80)

int varint64(char *data, int64_t *value)
{
    int64_t result = 0;
    int shifts = 0;
    do {
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    } while(expect_more(*data++));
    *value = result;
    return shifts;
}

// returns the number of bytes read from data
int varint32(char *data, int32_t *value)
{
    int32_t result = 0;
    int shifts = 0;
    do {
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    } while(expect_more(*data++));
    *value = result;
    return shifts;
}

int varint64_encode(int64_t value, char *data, int len)
{
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    do {
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value = (uint64_t)value >> 7;
        i++;
    } while(value);
    data[i - 1] &= mask;
    return i;
}

int varint32_encode(int32_t value, char *data, int len)
{
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    do {
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value = (uint32_t)value >> 7;
        i++;
    } while(value);
    data[i - 1] &= mask;
    return i;
}

uint64_t htonll(uint64_t number)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    number = __builtin_bswap64(number);
#endif
    return number;
}

uint64_t ntohll(uint64_t number)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    number = __builtin_bswap64(number);
#endif
    return number;
}


void *_read(void *buffer, void *storage, size_t size)
{
    memcpy(storage, buffer, size);
    return ((char *)buffer + size);
}

void *_read_int16_t(void *buffer, int16_t *val) {
    buffer = _read(buffer, val, sizeof(*val));
    *val = ntohs(*val);
    return buffer;
}

void *_read_uint16_t(void *buffer, uint16_t *val) {
    buffer = _read(buffer, val, sizeof(*val));
    *val = ntohs(*val);
    return buffer;
}

void *_read_int32_t(void *buffer, int32_t *val) {
    buffer = _read(buffer, val, sizeof(*val));
    *val = ntohl(*val);
    return buffer;
}

void *_read_uint32_t(void *buffer, uint32_t *val) {
    buffer = _read(buffer, val, sizeof(*val));
    *val = ntohl(*val);
    return buffer;
}

void *_read_int64_t(void *buffer, int64_t *val) {
    buffer = _read(buffer, val, sizeof(*val));
    *val = ntohll(*val);
    return buffer;
}

void *_read_uint64_t(void *buffer, uint64_t *val) {
    buffer = _read(buffer, val, sizeof(*val));
    *val = ntohll(*val);
    return buffer;
}

void *_read_float(void *buffer, float *val) {
    buffer = _read(buffer, val, sizeof(val));
    uint32_t *x = (uint32_t *)val; 
    *x = ntohl(*x);
    return buffer;
}

void *_read_double(void *buffer, double *val) {
    buffer = _read(buffer, val, sizeof(val));
    uint64_t *x = (uint64_t *)val;
    *x = ntohll(*x);
    return buffer;
}

void *_push(void *buffer, void *data, size_t size)
{
    memcpy(buffer, data, size);
    return (char *)buffer + size;
}

void *_push_int16_t(void *buffer, int16_t val) {
    val = htons(val);
    return _push(buffer, &val, sizeof(val));
}

void *_push_uint16_t(void *buffer, uint16_t val) {
    val = htons(val);
    return _push(buffer, &val, sizeof(val));
}

void *_push_int32_t(void *buffer, int32_t val) {
    val = htonl(val);
    return _push(buffer, &val, sizeof(val));
}

void *_push_uint32_t(void *buffer, uint32_t val) {
    val = htonl(val);
    return _push(buffer, &val, sizeof(val));
}

void *_push_int64_t(void *buffer, int64_t val) {
    val = htonll(val);
    return _push(buffer, &val, sizeof(val));
}

void *_push_uint64_t(void *buffer, uint64_t val) {
    val = htonll(val);
    return _push(buffer, &val, sizeof(val));
}

void *_push_float(void *buffer, float val) {
    uint32_t *x = (uint32_t *)&val;
    *x = htonl(*x);
    return _push(buffer, &val, sizeof(val));
}

void *_push_double(void *buffer, double val) {
    uint64_t *x = (uint64_t *)&val;
    *x = htonll(*x);
    return _push(buffer, &val, sizeof(val));
}

void *_read_vint32(void *buf, vint32_t *val) {
    uint32_t bytes_read = varint32(buf, val);
    return (char *)buf + bytes_read;
}

void *_push_vint32(void *buf, vint32_t val) {
    char varint[5];
    uint32_t len = varint32_encode(val, varint, sizeof(varint));
    buf = _push(buf, varint, len);
    return buf;
}

void *_read_string(void *buf, char **strptr, int32_t *str_len) {
    uint32_t bytes_read = varint32(buf, str_len);
    buf = (char *)buf + bytes_read;
    *strptr = malloc(*str_len + 1);
    memcpy(*strptr, buf, *str_len);
    (*strptr)[*str_len] = '\0';
    return (char *)buf + *str_len;
}

void *_push_string(void *buf, char *str) {
    uint32_t bytes_pushed;
    size_t s_len = strlen(str);
    char str_len[5];
    bytes_pushed = varint32_encode(s_len, str_len, sizeof(str_len));
    buf = _push(buf, str_len, bytes_pushed);
    buf = _push(buf, str, s_len);
    return buf; 
}

void *_push_slot(void *_packet_raw, struct slot_type *slot_data)
{
    int16_t block_id = slot_data->block_id;

    char *packet_raw = _packet_raw;
    if (-1 == block_id) { // Empty slot is 0xffff
        return _push(packet_raw, &block_id, sizeof(block_id));
    } else {
        int8_t count = slot_data->count;
        int16_t damage = slot_data->damage;

        /* copy block_id (2 bytes) */
        packet_raw = _push_int16_t(packet_raw, block_id);

        /* copy count (1 byte) */
        packet_raw = _push(packet_raw, &count, sizeof(count));

        /* copy damage (2 bytes) */
        packet_raw = _push_int16_t(packet_raw, damage);

        if (slot_data->nbt_data) {
            /* convert nbt tree to binary nbt data */
            packet_raw = _push(packet_raw, slot_data->nbt_data, slot_data->nbt_length);
        } else {
            memset(packet_raw, 0, sizeof(int8_t));
            packet_raw += sizeof(int8_t);
        }
    }
    return packet_raw;
}



/* appends length to the buffer as a varint, returns the start of the buffer */
/* writes the new length of packet in len */
void *_append_len(void *buf, uint32_t *len) {
    char buf_len[5];
    int offset = varint32_encode(*len, buf_len, sizeof(buf_len));
    *len += offset;
    void *start = (char *)buf + 5 - offset;
    _push(start, buf_len, offset);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x00);
    buf = _push_vint32(buf, protocol_version);
    buf = _push_string(buf, server_addr);
    buf = _push_uint16_t(buf, server_port);
    buf = _push_vint32(buf, next_state);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x00);
    buf = _push_string(buf, username);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
        struct bot_agent*        bot
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x00);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_status_serverbound_ping(
        struct bot_agent*        bot,
        int64_t       time
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x01);
    buf = _push_int64_t(buf, time);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x00);
    buf = _push_vint32(buf, teleport_id);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x01);
    buf = _push_string(buf, text);
    int8_t b = assume_command ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));
    b = has_position ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));
    if (has_position) {
        buf = _push_uint64_t(buf, looked_at_block);
    }
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_chat_message(
        struct bot_agent *bot,
        char *message
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x02);
    buf = _push_string(buf, message);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_client_status(
        struct bot_agent *bot,
        vint32_t action_id
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x03);
    buf = _push_vint32(buf, action_id);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;
    buf = _push_vint32(buf, 0x04);
    buf = _push_string(buf, locale);
    buf = _push(buf, &view_distance, sizeof(view_distance));
    buf = _push_vint32(buf, chat_mode);
    int8_t b = chat_colors ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));
    buf = _push(buf, &displayed_skin_parts, sizeof(displayed_skin_parts));
    buf = _push_vint32(buf, main_hand);
    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_confirm_transaction(
        struct bot_agent *bot,
        int8_t window_id,
        int16_t action_number,
        bool accepted
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x05);
    buf = _push(buf, &window_id, sizeof(window_id));
    buf = _push_int16_t(buf, action_number);
    int8_t b = accepted ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_enchant_item(
        struct bot_agent *bot,
        int8_t window_id,
        int8_t enchantment
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x06);
    buf = _push(buf, &window_id, sizeof(window_id));
    buf = _push(buf, &enchantment, sizeof(enchantment));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x07);
    buf = _push(buf, &window_id, sizeof(window_id));
    buf = _push_int16_t(buf, slot);
    buf = _push(buf, &button, sizeof(button));
    buf = _push_int16_t(buf, action_number);
    buf = _push_vint32(buf, mode);
    buf = _push_slot(buf, clicked_item);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_close_window(
        struct bot_agent *bot,
        uint8_t window_id
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x08);
    buf = _push(buf, &window_id, sizeof(window_id));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_plugin_message(
        struct bot_agent *bot,
        char *channel,
        uint32_t data_length,
        void *data
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x09);
    buf = _push_string(buf, channel);
    buf = _push(buf, data, data_length); 

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x0A);
    buf = _push_vint32(buf, target);
    buf = _push_vint32(buf, type);
    switch(type) {
        case 2: /* interact at */
            buf = _push_float(buf, target_x);
            buf = _push_float(buf, target_y);
            buf = _push_float(buf, target_z);
        case 0: /* interact */
            buf = _push_vint32(buf, hand);
    }

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_keepalive(
        struct bot_agent*        bot,
        vint32_t      keepalive_id
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x0B);
    buf = _push_vint32(buf, keepalive_id);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x0C);
    buf = _push_double(buf, x);
    buf = _push_double(buf, y);
    buf = _push_double(buf, z);
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x0D);
    buf = _push_double(buf, x);
    buf = _push_double(buf, y);
    buf = _push_double(buf, z);
    buf = _push_float(buf, yaw);
    buf = _push_float(buf, pitch);
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_player_look(
        struct bot_agent *bot,
        float yaw,
        float pitch,
        bool on_ground
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x0E);
    buf = _push_float(buf, yaw);
    buf = _push_float(buf, pitch);
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_player(
        struct bot_agent *bot,
        bool on_ground
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x0F);
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x10);
    buf = _push_double(buf, x);
    buf = _push_double(buf, y);
    buf = _push_double(buf, z);
    buf = _push_float(buf, yaw);
    buf = _push_float(buf, pitch);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_steer_boat(
        struct bot_agent *bot,
        bool right_paddle,
        bool left_paddle
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x11);
    int8_t b = right_paddle ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));
    b = left_paddle ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_player_abilities(
        struct bot_agent *bot,
        int8_t flags,
        float flying_speed,
        float walking_speed
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x12);
    buf = _push(buf, &flags, sizeof(flags));
    buf = _push_float(buf, flying_speed);
    buf = _push_float(buf, walking_speed);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_player_digging(
        struct bot_agent *bot,
        vint32_t status,
        position_t location,
        int8_t face
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x13);
    buf = _push_vint32(buf, status);
    buf = _push_uint64_t(buf, location);
    buf = _push(buf, &face, sizeof(face));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_entity_action(
        struct bot_agent *bot,
        vint32_t entity_id,
        vint32_t action_id,
        vint32_t jump_boost
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x14);
    buf = _push_vint32(buf, entity_id);
    buf = _push_vint32(buf, action_id);
    buf = _push_vint32(buf, jump_boost);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_steer_vehicle(
        struct bot_agent *bot,
        float sideways,
        float forward,
        uint8_t flags
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x15);
    buf = _push_float(buf, sideways);
    buf = _push_float(buf, forward);
    buf = _push(buf, &flags, sizeof(flags));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_resource_pack_status(
        struct bot_agent *bot,
        char *hash,
        vint32_t result
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x16);
    buf = _push_string(buf, hash);
    buf = _push_vint32(buf, result);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_held_item_change(
        struct bot_agent *bot,
        int16_t slot
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x17);
    buf = _push_int16_t(buf, slot);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_creative_inventory_action(
        struct bot_agent *bot,
        int16_t slot,
        struct slot_type *clicked_item
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x18);
    buf = _push_int16_t(buf, slot);
    buf = _push_slot(buf, clicked_item);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x19);
    buf = _push_uint64_t(buf, location);
    buf = _push_string(buf, line1);
    buf = _push_string(buf, line2);
    buf = _push_string(buf, line3);
    buf = _push_string(buf, line4);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_animation(
        struct bot_agent *bot,
        vint32_t hand
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x1A);
    buf = _push_vint32(buf, hand);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_spectate(
        struct bot_agent *bot,
        char *uuid
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x1B);
    buf = _push(buf, uuid, 16);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x1C);
    buf = _push_uint64_t(buf, location);
    buf = _push_vint32(buf, face);
    buf = _push_vint32(buf, hand);
    buf = _push(buf, &x, sizeof(x));
    buf = _push(buf, &y, sizeof(y));
    buf = _push(buf, &z, sizeof(z));

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
    return length;
}

int32_t send_play_serverbound_use_item(
        struct bot_agent *bot,
        vint32_t hand
        )
{
    void *tmp = malloc(bot->packet_threshold);
    void *buf = tmp, *packet;
    PAD_LENGTH(buf);
    packet = buf;

    buf = _push_vint32(buf, 0x1D);
    buf = _push_vint32(buf, hand);

    uint32_t length = (int32_t)(buf - packet);
    packet = _append_len(tmp, &length);
    send_raw(bot, packet, length);
    free(tmp);
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

void *_read_entity_metadata(void *buf, struct entity_metadata *metadata) {
    uint32_t num_entries = 0;

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
