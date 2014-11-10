#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "marshal.h"
#include "bot.h"
#include "protocol.h"
#include "nbt.h"

// returns the number of bytes read from data
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


// a generalized form of the hton functions
// in the sockets library convert numbers to
// their Big-endian format. To avoid complications
// with types, the conversion will be done in place,
// rather than returning the converted value.
// In addition, this function extends to 64 and 128 bit
// words as well.
void reverse(void *number, int len)
{
    if(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) { // we need to fliparoo!
        switch(len) {
        case sizeof(int8_t):
            return;
        case sizeof(int16_t):
            *((uint16_t *)number) = __builtin_bswap16(*((uint16_t *)number));
            break;
        case sizeof(int32_t):
            *((uint32_t *)number) = __builtin_bswap32(*((uint32_t *)number));
            break;
        case sizeof(int64_t):
            *((uint64_t *)number) = __builtin_bswap64(*((uint64_t *)number));
            break;
        case sizeof(__int128_t):
            ;
            uint64_t *l = number;
            uint64_t *h = number + sizeof(int64_t);
            *l = __builtin_bswap64(*l);
            *h = __builtin_bswap64(*h);
            *l ^= *h;
            *h ^= *l;
            *l ^= *h;
            break;
        default:
            fprintf(stderr, "Bad word size\n");
            return;
        }
    }
}

size_t format_sizeof(char c)
{
    switch(c) {
    case 's':
        return sizeof(void *);
    case 'v':
        return sizeof(vint32_t);
    case '*':
        return sizeof(void *);
    case 'b':
        return sizeof(int8_t);
    case 'h':
        return sizeof(int16_t);
    case 'w':
        return sizeof(int32_t);
    case 'l':
        return sizeof(int64_t);
    case 'q':
        return sizeof(__int128_t);
    case 'S':
        return sizeof(slot_t *);
    default:
        fprintf(stderr, "Bad format specifier\n");
        return -1;
    }
}

// data writes in the same direction as the buffer is growing pushes the data,
// then returns the address where the next datum is to be pushed
void *push(void *buffer, void *data, size_t size)
{
    switch(size) {
    case sizeof(int8_t):
        *(int8_t *)buffer = *(int8_t *)data;
        buffer += size;
        break;
    case sizeof(int16_t):
        *(int16_t *)buffer = *(int16_t *)data;
        buffer += size;
        break;
    case sizeof(int32_t):
        *(int32_t *)buffer = *(int32_t *)data;
        buffer += size;
        break;
    case sizeof(int64_t):
        *(int64_t *)buffer = *(int64_t *)data;
        buffer += size;
        break;
    case sizeof(__int128_t):
        *(__int128_t *)buffer = *(__int128_t *)data;
        buffer += size;
        break;
    }
    return buffer;
}

// Read size bytes from buf as an integer and
// returns the 128 bit integer at the top of the buffer
__int128_t value_at(void *buf, size_t size)
{
    switch(size) {
    case sizeof(int8_t):
        return *(int8_t *)buf;
    case sizeof(int16_t):
        return *(int16_t *)buf;
    case sizeof(int32_t):
        return *(int32_t *)buf;
    case sizeof(int64_t):
        return *(int64_t *)buf;
    case sizeof(__int128_t):
        return *(__int128_t *)buf;
    }
    // if you're here, you're wrong
    assert(0);
}

void reentrant_memmove(void *dest, void *src, size_t len)
{
    // shift backwards, read forwards
    if(dest < src)
        while(len--)
            *(uint8_t *)dest++ = *(uint8_t *)src++;
    // shift forwards, read backwards
    else
        while(len--)
            *(uint8_t *)(dest + len) = *(uint8_t *)(src + len);

}

// puts the raw packet data from the struct packet_data into packet_raw
// returns the number of bytes written to packet_raw, or -1 if packet_raw
// is not a long enough array
int format_packet(bot_t *bot, void *packet_data, void *packet_raw)
{
    uint32_t len = bot->_data->packet_threshold;
    uint32_t value = 0;
    uint32_t varlen = 0;
    size_t arr_len = 0;
    char varint[5];
    size_t size;

    char *fmt = *((char **)packet_data);
    packet_data += sizeof(void *);
    void *save = packet_raw;
    while(*fmt) {
        size = format_sizeof(*fmt);
        packet_data = (void *)align(packet_data, size);
        switch(*fmt) {
        case 's': // string (null terminated)
            ;
            char *str = *((char **)packet_data);
            value = strlen(str);
            varlen = varint32_encode(value, varint, 5);
            if(packet_raw - save + varlen + value > len)
                return -1; // TODO: compression
            reentrant_memmove(packet_raw, varint, varlen);
            packet_raw += varlen;
            reentrant_memmove(packet_raw, str, value);
            packet_raw += value;
            break;
        case 'v': // varint32_t
            ;
            value = *((uint32_t *)packet_data);
            arr_len = value;
            varlen = varint32_encode(value, varint, 5);
            if(packet_raw - save + varlen > len)
                return -1; // TODO: compression
            reentrant_memmove(packet_raw, varint, varlen);
            packet_raw += varlen;
            break;
        case '*': // pointer/array
            ;
            fmt++;
            size_t size_elem = format_sizeof(*fmt);
            if(packet_raw - save + size_elem * arr_len > len)
                return -1; // TODO: compression
            void *arr = *((void **)packet_data);
            for(int i = 0; i < arr_len * size_elem; i += size_elem) {
                packet_raw = push(packet_raw, arr + i, size_elem);
                reverse(packet_raw - size_elem, size_elem);
            }
            break;
        case 'S': // slot_t NBT structure
            ;
            slot_t *item = *((slot_t **)packet_data);
            packet_raw = push(packet_raw, &item->item_id, sizeof(int16_t));
            if (item->item_id != -1) {
                packet_raw = push(packet_raw, &item->count, sizeof(uint8_t));
                packet_raw = push(packet_raw, &item->damage, sizeof(uint16_t));
                if (item->nbt) {
                    struct buffer buf = nbt_dump_binary(item->nbt);
                    reentrant_memmove(packet_raw, buf.data, buf.len);
                    packet_raw += buf.len;
                    free(buf.data);
                }
            }
            break;
        default:
            ;
            if(packet_raw - save + size > len)
                return -1; // TODO: compression
            packet_raw = push(packet_raw, packet_data, size);
            reverse(packet_raw - size, size);
            arr_len = value_at(packet_data, size);
            break;
        }
        packet_data += size;
        fmt++;
    }

    varlen = varint32_encode(packet_raw - save, varint, 5);
    if(packet_raw - save + varlen > len)
        return -1; // TODO: compression
    reentrant_memmove(save + varlen, save, packet_raw - save);
    reentrant_memmove(save, varint, varlen);
    return packet_raw - save + varlen;
}

int decode_packet(bot_t *bot, void *packet_raw, void *packet_data)
{
    // packet_raw = raw packet data
    // packet_data = struct containing packet data
    uint32_t len;
    vint32_t value;
    uint32_t arr_len = -1;
    size_t size;

    char *fmt = *((char **)packet_data);
    assert(fmt != 0);
    packet_data += sizeof(void *);

    int32_t packet_size;
    void *save = packet_raw;
    int packet_size_len = varint32(packet_raw, &packet_size);
    packet_raw += packet_size_len;

    while(*fmt) {
        size = format_sizeof(*fmt);
        packet_data = (void *)align(packet_data, size);
        switch(*fmt) {
        case 's': // varint followed by string
            len = varint32(packet_raw, &value);
            packet_raw += len;
            char *str = calloc(value + 1, sizeof(char)); // null terminated
            reentrant_memmove(str, packet_raw, value);
            *((char **)packet_data) = str;
            packet_raw += value;
            break;
        case 'v': // varint
            len = varint32(packet_raw, &value);
            arr_len = value;
            reentrant_memmove(packet_data, &value, sizeof(vint32_t));
            packet_raw += len;
            break;
        case '*':
            fmt++;
            size_t size_elem = format_sizeof(*fmt);
            assert(arr_len != -1);
            void *arr = calloc(arr_len, size_elem);
            for(int i = 0; i < arr_len * size_elem; i += size_elem) {
                reentrant_memmove(arr + i, packet_raw + i, size_elem);
                reverse(arr + i, size_elem);
            }
            *((void **)packet_data) = arr;
            packet_raw += arr_len * size_elem;
            break;
        case 'S': // slot_t NBT structure
            ;
            slot_t* item = calloc(1, sizeof(slot_t));
            item->item_id = value_at(packet_raw, sizeof(int16_t));
            reverse(&item->item_id, sizeof(int16_t));
            packet_raw += sizeof(int16_t);

            if (item->item_id != -1) {
                item->count = value_at(packet_raw, sizeof(uint8_t));
                reverse(&item->item_id, sizeof(uint8_t));
                packet_raw += sizeof(uint8_t);

                item->damage = value_at(packet_raw, sizeof(uint16_t));
                reverse(&item->item_id, sizeof(uint16_t));
                packet_raw += sizeof(uint16_t);

                if (value_at(packet_raw, sizeof(uint8_t)) != 0) {
                    size_t nbt_len = bot->_data->packet_threshold - (save - packet_raw);
                    nbt_node* tree = nbt_parse(packet_raw, &nbt_len);
                    item->nbt = tree;
                    packet_raw += nbt_len;
                }
            }
            *((slot_t **)packet_data) = item;
            break;
        default:
            ;
            reentrant_memmove(packet_data, packet_raw, size);
            reverse(packet_data, size);
            arr_len = value_at(packet_data, size);
            packet_raw += size;
            break;
        }
        packet_data += size;
        fmt++;
    }
    return packet_size_len + packet_size;
}

vint32_t peek_packet(bot_t *bot, void *packet_raw)
{
    int32_t packet_size;
    int32_t value;
    uint32_t len;
    int packet_size_len = varint32(packet_raw, &packet_size);
    packet_raw += packet_size_len;

    len = varint32(packet_raw, &value);
    return value;
}

void free_packet(void *packet_data)
{
    char *fmt = *((char **)packet_data);
    void *save = packet_data;
    packet_data += sizeof(void *);
    while (*fmt) {
        size_t size = format_sizeof(*fmt);
        packet_data = (void *)align(packet_data, size);
        if (*fmt == 's' || *fmt == '*') {
            free(*((void **)packet_data));
            if(*fmt == '*')
                fmt++;
        }
        packet_data += size;
        fmt++;
    }
    free(save);
}
