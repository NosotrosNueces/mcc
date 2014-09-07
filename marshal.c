#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "marshal.h"
#include "bot.h"
#include "protocol.h"

#define expect_more(x) (x & 0x80)
// align pointer x to y
// y is assumed to be a power of 2
#define align(x, y) ((void *)((size_t)(x + y - 1) & (~y + 1)))

// returns the number of bytes read from data
int varint64(char *data, int64_t *value){
    int64_t result = 0;
    int shifts = 0;
    do{
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    }while(expect_more(*data++));
    *value = result;
    return shifts;
}

// returns the number of bytes read from data
int varint32(char *data, int32_t *value){
    int32_t result = 0;
    int shifts = 0;
    do{
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    }while(expect_more(*data++));
    *value = result;
    return shifts;
}

int varint64_encode(uint64_t value, char *data, int len){
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    do{
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value = value >> 7;
        i++;
    }while(value);
    data[i - 1] &= mask;
    return i;
}

int varint32_encode(uint32_t value, char *data, int len){
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    do{
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value = value >> 7;
        i++;
    }while(value);
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
void reverse(void *number, int len){
    if(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__){ // we need to fliparoo!
        switch(len){
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
            case sizeof(__int128_t):{
                uint64_t *l = number;
                uint64_t *h = number + sizeof(int64_t);
                *l = __builtin_bswap64(*l);
                *h = __builtin_bswap64(*h);
                *l ^= *h;
                *h ^= *l;
                *l ^= *h;
                break;
            }
            default:
                fprintf(stderr, "Bad word size\n");
                return;
        }
    }
}

size_t format_sizeof(char c) {
    switch(c){
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
        default:
            fprintf(stderr, "Bad format specifier\n");
            return -1;
    }
}


// puts the raw packet data from the struct packet_data into packet_raw
// returns the number of bytes written to packet_raw, or -1 if packet_raw
// is not a long enough array
int format_packet(bot_t *bot, void *packet_data, void **packet_raw_ptr){
    uint32_t len = bot -> packet_threshold;
    uint32_t index = 0;
    uint32_t value = 0;
    uint32_t varlen = 0;
    uint32_t arr_len = 0;
    char varint[5];
    size_t size;

    void *packet_raw = calloc(bot -> packet_threshold, sizeof(int8_t));
    char *fmt = *((char **)packet_data);
    packet_data += sizeof(void *);

    while(*fmt){
        size = format_sizeof(*fmt);
        packet_data = align(packet_data, size);
        switch(*fmt){
            case 's':{ // string (null terminated)
                char *str = *((char **)packet_data);
                value = strlen(str);
                varlen = varint32_encode(value, varint, 5);
                if(index + varlen + value > len)
                    return -1; // TODO: compression
                memcpy(packet_raw + index, varint, varlen);
                memcpy(packet_raw + index + varlen, str, value);
                index += value + varlen;
                break;
            }
            case 'v':{ // varint32_t
                value = *((uint32_t *)packet_data);
                arr_len = value;
                varlen = varint32_encode(value, varint, 5);
                if(index + varlen > len)
                    return -1; // TODO: compression
                memcpy(packet_raw + index, varint, varlen);
                index += varlen;
                break;
            }
            case '*':{ // pointer/array
                fmt++;
                size_t size_elem = format_sizeof(*fmt);
                if(index + size_elem * arr_len > len)
                    return -1; // TODO: compression
                void *arr = *((void **)packet_data);
                for(int i = 0;i < arr_len * size_elem; i += size_elem){
                    memcpy(packet_raw + index + i, arr + i, size_elem);
                    reverse(packet_raw + index + i, size_elem);
                }
                index += arr_len * size_elem;
                break;
            }
            default:{
                size_t size = format_sizeof(*fmt);
                if(index + size > len)
                    return -1; // TODO: compression
                memcpy(packet_raw + index, packet_data, size);
                reverse(packet_raw + index, size);
                arr_len = *((int *)packet_data);
                index += size;
                break;
            }
        }
        packet_data += size;
        fmt++;
    }
    varlen = varint32_encode(index, varint, 5);
    if(index + varlen > len)
        return -1; // TODO: compression
    memmove(packet_raw + varlen, packet_raw, index);
    memcpy(packet_raw, varint, varlen);
    *packet_raw_ptr = packet_raw;
    return index + varlen;
}

int decode_packet(void *packet_raw, void *packet_data){
    // packet_raw = raw packet data
    // packet_data = struct containing packet data
    uint32_t len;
    vint32_t value;
    uint32_t arr_len;
    size_t size;

    char *fmt = *((char **)packet_data);
    packet_data += sizeof(void *);
    
    int32_t packet_size;
    int packet_size_len = varint32(packet_raw, &packet_size);
    packet_raw += packet_size_len;
    
    while(*fmt){
        size = format_sizeof(*fmt);
        packet_data = align(packet_data, size);
        switch(*fmt){
            case 's': // varint followed by string
                len = varint32(packet_raw, &value);
                packet_raw += len;
                char *str = calloc(value + 1, sizeof(char)); // null terminated
                memcpy(str, packet_raw, value);
                *((char **)packet_data) = str;
                packet_raw += value;
                break;
            case 'v': // varint
                len = varint32(packet_raw, &value);
                arr_len = value;
                memcpy(packet_data, &value, sizeof(vint32_t));
                packet_raw += len;
                break;
            case '*':
                fmt++;
                size_t size_elem = format_sizeof(*fmt);
                void *arr = calloc(arr_len, size_elem);
                for(int i = 0; i < arr_len * size_elem; i += size_elem){
                    memcpy(arr + i, packet_raw + i, size_elem);
                    reverse(arr + i, size_elem);
                }
                *((void **)packet_data) = arr;
                packet_raw += arr_len * size_elem;
                break;
            default:{
                memcpy(packet_data, packet_raw, size);
                reverse(packet_data, size);
                arr_len = *((int *)packet_data);
                packet_raw += size;
                break;
            }
        }
        packet_data += size;
        fmt++;
    }
    return packet_size_len + packet_size;
}


void free_packet(void *packet_data){
    char *fmt = *((char **)packet_data);
    packet_data += sizeof(void *);
    switch(*fmt){
        case 'b':
            packet_data = align(packet_data, sizeof(int8_t));
            packet_data += sizeof(int8_t);
            break;
        case 'h':
            packet_data = align(packet_data, sizeof(int16_t));
            packet_data += sizeof(int16_t);
            break;
        case 'w': case 'v':
            packet_data = align(packet_data, sizeof(int32_t));
            packet_data += sizeof(int32_t);
            break;
        case 'l':
            packet_data = align(packet_data, sizeof(int64_t));
            packet_data += sizeof(int64_t);
            break;
        case 'q':
            packet_data = align(packet_data, sizeof(__int128_t));
            packet_data += sizeof(__int128_t);
            break;
        case 's':
            packet_data = align(packet_data, sizeof(void *));
            free(*((void **)packet_data));
            packet_data += sizeof(void *);
            break;
        case '*':
            packet_data = align(packet_data, sizeof(void *));
            free(*((void **)packet_data));
            packet_data += sizeof(void *);
            fmt++;
            break;
        default:
            break;
    }
    fmt++;
}
