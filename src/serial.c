#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "nbt.h"

#define expect_more(x) (x & 0x80)

int varint64(char *data, int32_t bytes_left, int64_t *value)
{
    int64_t result = 0;
    int shifts = 0;
    do {
        if (bytes_left <= 0) {
            return -1;
        }
        bytes_left--;
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    } while (expect_more(*data++));
    *value = result;
    return shifts;
}

// returns the number of bytes read from data
int varint32(char *data, int32_t bytes_left, int32_t *value)
{
    int32_t result = 0;
    int shifts = 0;
    do {
        if (bytes_left <= 0) {
            return -1;
        }
        bytes_left--;
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    } while (expect_more(*data++));
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
    } while (value);
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
    } while (value);
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

/* When reading from an incoming packet, we already know the length. Therefore
 * we do not have to do any length checking except in the very beginning.
 *
 * However, when we send a packet to the server, we are building the packet as
 * we go along, so we do not know the length beforehand. Thus, we must do bounds
 * checking every time we push to the buffer
 **/

char *_read(char *buffer, void *storage, size_t size, struct bot_agent *bot)
{
    /* Bounds checking */
    if (bot->mcc_status == MCC_PARSE_ERROR || bot->packet_length - (buffer - bot->packet_data) < size) {
        bot->mcc_status = MCC_PARSE_ERROR;
        assert(0);
    }
    memcpy(storage, buffer, size);
    return ((char *)buffer + size);
}

char *_read_int16_t(char *buffer, int16_t *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    *val = ntohs(*val);
    return buffer;
}

char *_read_uint16_t(char *buffer, uint16_t *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    *val = ntohs(*val);
    return buffer;
}

char *_read_int32_t(char *buffer, int32_t *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    *val = ntohl(*val);
    return buffer;
}

char *_read_uint32_t(char *buffer, uint32_t *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    *val = ntohl(*val);
    return buffer;
}

char *_read_int64_t(char *buffer, int64_t *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    *val = ntohll(*val);
    return buffer;
}

char *_read_uint64_t(char *buffer, uint64_t *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    *val = ntohll(*val);
    return buffer;
}

char *_read_float(char *buffer, float *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    uint32_t *x = (uint32_t *)val; 
    *x = ntohl(*x);
    return buffer;
}

char *_read_double(char *buffer, double *val, struct bot_agent *bot) {
    buffer = _read(buffer, val, sizeof(*val), bot);
    uint64_t *x = (uint64_t *)val;
    *x = ntohll(*x);
    return buffer;
}

void _resize_packet_write_buffer(struct packet_write_buffer *buffer) {
    uint32_t offset = buffer->ptr - buffer->base;
    buffer->capacity *= 2;
    buffer->base = realloc(buffer->base, buffer->capacity);
    buffer->ptr = buffer->base + offset;
}

void pad_length(struct packet_write_buffer *buffer) {
    while (buffer->capacity < buffer->ptr + 5 - buffer->base) {
        _resize_packet_write_buffer(buffer);
    }
    buffer->ptr += 5;
}

void init_packet_write_buffer(struct packet_write_buffer *buffer, uint32_t capacity) {
    buffer->capacity = capacity;
    buffer->base = malloc(capacity);
    buffer->ptr = buffer->base;
}

void _push(struct packet_write_buffer *buffer, void *data, size_t size)
{
    while (buffer->capacity < buffer->ptr + size - buffer->base) {
        _resize_packet_write_buffer(buffer);
    }
    memcpy(buffer->ptr, data, size);    
    buffer->ptr += size;
}

void _push_int16_t(struct packet_write_buffer *buffer, int16_t val) {
    val = htons(val);
    _push(buffer, &val, sizeof(val));
}

void _push_uint16_t(struct packet_write_buffer *buffer, uint16_t val) {
    val = htons(val);
    _push(buffer, &val, sizeof(val));
}

void _push_int32_t(struct packet_write_buffer *buffer, int32_t val) {
    val = htonl(val);
    _push(buffer, &val, sizeof(val));
}

void _push_uint32_t(struct packet_write_buffer *buffer, uint32_t val) {
    val = htonl(val);
    _push(buffer, &val, sizeof(val));
}

void _push_int64_t(struct packet_write_buffer *buffer, int64_t val) {
    val = htonll(val);
    _push(buffer, &val, sizeof(val));
}

void _push_uint64_t(struct packet_write_buffer *buffer, uint64_t val) {
    val = htonll(val);
    _push(buffer, &val, sizeof(val));
}

void _push_float(struct packet_write_buffer *buffer, float val) {
    uint32_t *x = (uint32_t *)&val;
    *x = htonl(*x);
    _push(buffer, &val, sizeof(val));
}

void _push_double(struct packet_write_buffer *buffer, double val) {
    uint64_t *x = (uint64_t *)&val;
    *x = htonll(*x);
    _push(buffer, &val, sizeof(val));
}

char *_read_vint32(char *buf, vint32_t *val, struct bot_agent *bot) {
    if (bot->mcc_status == MCC_PARSE_ERROR) {
        assert(0);
    }
    uint32_t bytes_left = bot->packet_length - (buf - bot->packet_data);
    uint32_t bytes_read = varint32(buf, bytes_left, val);
    if (bytes_read == -1) {
        bot->mcc_status = MCC_PARSE_ERROR;
        assert(0);
    }
    return (char *)buf + bytes_read;
}

void _push_vint32(struct packet_write_buffer *buf, vint32_t val) {
    char varint[5];
    uint32_t len = varint32_encode(val, varint, sizeof(varint));
    _push(buf, varint, len);
}

char *_read_vint64(char *buf, vint64_t *val, struct bot_agent *bot) {
    if (bot->mcc_status == MCC_PARSE_ERROR) {
        assert(0);
    }
    uint32_t bytes_left = bot->packet_length - (buf - bot->packet_data);
    uint32_t bytes_read = varint64(buf, bytes_left, val);
    if (bytes_read == -1) {
        bot->mcc_status = MCC_PARSE_ERROR;
        assert(0);
    }
    return buf;
}

void _push_vint64(struct packet_write_buffer *buf, vint64_t val) {
    char varint[10];
    uint32_t len = varint64_encode(val, varint, sizeof(varint));
    _push(buf, varint, len);
}

char *_read_string(char *buf, char **strptr, int32_t *str_len_opt, struct bot_agent *bot) {
    int32_t str_len;
    
    buf = _read_vint32(buf, &str_len, bot);

    *strptr = malloc(str_len + 1);
    buf = _read(buf, *strptr, str_len, bot);
    (*strptr)[str_len] = '\0';

    if (str_len_opt != NULL) {
        *str_len_opt = str_len;
    }
    return buf;
}

void _push_string(struct packet_write_buffer *buf, char *str) {
    uint32_t bytes_pushed;
    size_t s_len = strlen(str);
    char str_len[5];
    bytes_pushed = varint32_encode(s_len, str_len, sizeof(str_len));
    _push(buf, str_len, bytes_pushed);
    _push(buf, str, s_len);
}

char *_read_slot(char *packet_raw, struct slot_type *slot_data, struct bot_agent *bot) {
    packet_raw = _read_int16_t(packet_raw, &slot_data->block_id, bot);
    if (slot_data->block_id == -1) {
        return packet_raw;
    }
    packet_raw = _read(packet_raw, &slot_data->count, sizeof(slot_data->count), bot);
    packet_raw = _read_int16_t(packet_raw, &slot_data->damage, bot);

    uint32_t bytes_read;
    slot_data->tree = nbt_parse(packet_raw, &bytes_read, bot);
    packet_raw += bytes_read;
    return packet_raw;
}

void _push_slot(struct packet_write_buffer *packet_raw, struct slot_type *slot_data)
{
    int16_t block_id = slot_data->block_id;

    if (-1 == block_id) { // Empty slot is 0xffff
        return _push(packet_raw, &block_id, sizeof(block_id));
    }

    int8_t count = slot_data->count;
    int16_t damage = slot_data->damage;

    /* copy block_id (2 bytes) */
    _push_int16_t(packet_raw, block_id);

    /* copy count (1 byte) */
    _push(packet_raw, &count, sizeof(count));

    /* copy damage (2 bytes) */
    _push_int16_t(packet_raw, damage);

    switch (slot_data->type) {
        case NBT_TREE:
            break;
        case NBT_BINARY:
            if (slot_data->nbt_binary.data != NULL) {
                /* convert nbt tree to binary nbt data */
                _push(packet_raw, slot_data->nbt_binary.data, slot_data->nbt_binary.length);
            } else {
            }
            break;
    }
}
