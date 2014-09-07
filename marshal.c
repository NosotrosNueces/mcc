#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "protocol.h"
#include "bot.h"

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



// puts the raw packet data from the struct packet_data into packet_raw
// returns the number of bytes written to packet_raw, or -1 if packet_raw
// is not a long enough array
int format_packet(bot_t *bot, void *packet_data, void **packet_raw_ptr){
    int len = bot -> packet_threshold;
    void *packet_raw = calloc(bot -> packet_threshold, sizeof(int8_t));
    int index = 0;
    char *fmt = *((char **)packet_data);
    packet_data += sizeof(void *);
    // each value in the packet may or may not be the array length of
    // an array following it, so it must be saved off.
    int arr_len;
    int32_t value;
    char varint[5];
    int varlen;
    while(*fmt){
        switch(*fmt){
            case 's':{ // string (null terminated)
                packet_data = align(packet_data, sizeof(void *));
                char *str = *((char **)packet_data);
                value = strlen(str);
                varlen = varint32_encode(value, varint, 5);
                if(index + varlen > len)
                    return -1; // TODO: compression
                memcpy(packet_raw + index, varint, varlen);
                index += varlen;
                if(index + value > len)
                    return -1; // TODO: compression
                memcpy(packet_raw + index, str, value);
                index += value;
                packet_data += sizeof(void *);
                break;
            }
            case 'v':{ // varint32_t
                packet_data = align(packet_data, sizeof(vint32_t));
                value = *((int32_t *)packet_data);
                arr_len = value;
                varlen = varint32_encode(value, varint, 5);
                if(index + varlen > len)
                    return -1; // TODO: compression
                memcpy(packet_raw + index, varint, varlen);
                index += varlen;
                packet_data += sizeof(vint32_t);
                break;
            }
            case '*':{ // pointer/array
                fmt++;
                int size = 0;
                switch(*fmt){
                    case 'b':
                        size = sizeof(int8_t);
                        break;
                    case 'h':
                        size = sizeof(int16_t);
                        break;
                    case 'w':
                        size = sizeof(int32_t);
                        break;
                    case 'l':
                        size = sizeof(int64_t);
                        break;
                    case 'q':
                        size = sizeof(__int128_t);
                        break;
                    default:
                        fprintf(stderr, "Bad format string\n");
                        return -1;
                }
                if(index + size * arr_len > len)
                    return -1; // TODO: compression
                packet_data = align(packet_data, sizeof(void *));
                void *arr = *((void **)packet_data);
                int i = 0;
                for(;i < arr_len * size; i += size){
                    memcpy(packet_raw + index + i, arr + i, size);
                    reverse(packet_raw + index + i, size);
                }
                //memcpy(packet_raw + index, arr, arr_len * size);
                index += arr_len * size;
                packet_data += sizeof(void *);
                break;
            }
            default:{
                int size = 0;
                switch(*fmt){
                    case 'b':
                        size = sizeof(int8_t);
                        break;
                    case 'h':
                        size = sizeof(int16_t);
                        break;
                    case 'w':
                        size = sizeof(int32_t);
                        break;
                    case 'l':
                        size = sizeof(int64_t);
                        break;
                    case 'q':
                        size = sizeof(__int128_t);
                        break;
                    default:
                        fprintf(stderr, "Bad format string\n");
                        return -1;
                }
                if(index + size > len)
                    return -1; // TODO: compression
                packet_data = align(packet_data, size);
                memcpy(packet_raw + index, packet_data, size);
                reverse(packet_raw + index, size);
                arr_len = *((int *)packet_data);
                index += size;
                packet_data += size;
                break;
            }
        }
        fmt++;
    }
    // =)
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
    char *fmt = *((char **)packet_data);
    packet_data += sizeof(void *);
    int32_t packet_size;
    int packet_size_len = varint32(packet_raw, &packet_size);
    packet_raw += packet_size_len;
    int len;
    vint32_t value;
    int arr_len;
    while(*fmt){
        switch(*fmt){
            case 's': // varint followed by string
                packet_data = align(packet_data, sizeof(void *));
                len = varint32(packet_raw, &value);
                packet_raw += len;
                char *str = calloc(value + 1, sizeof(char)); // null terminated
                memcpy(str, packet_raw, value);
                *((char **)packet_data) = str;
                packet_raw += value;
                packet_data += sizeof(void *);
                break;
            case 'v': // varint
                packet_data = align(packet_data, sizeof(vint32_t));
                len = varint32(packet_raw, &value);
                arr_len = value;
                memcpy(packet_data, &value, sizeof(vint32_t));
                packet_raw += len;
                packet_data += sizeof(vint32_t);
                break;
            case '*':
                fmt++;
                int size = 8;
                switch(*fmt){
                    case 'b':
                        size = sizeof(int8_t);
                        break;
                    case 'h':
                        size = sizeof(int16_t);
                        break;
                    case 'w':
                        size = sizeof(int32_t);
                        break;
                    case 'l':
                        size = sizeof(int64_t);
                        break;
                    case 'q':
                        size = sizeof(__int128_t);
                        break;
                    default:
                        fprintf(stderr, "Bad format string\n");
                        return -1;
                }
                packet_data = align(packet_data, sizeof(void *));
                void *arr = calloc(arr_len, size);
                int i = 0;
                for(; i < arr_len * size; i += size){
                    memcpy(arr + i, packet_raw + i, size);
                    reverse(arr + i, size);
                }
                *((void **)packet_data) = arr;
                packet_raw += arr_len * size;
                packet_data += sizeof(void *);
                break;
            default:{
                int size = 8;
                switch(*fmt){
                    case 'b':
                        size = sizeof(int8_t);
                        break;
                    case 'h':
                        size = sizeof(int16_t);
                        break;
                    case 'w':
                        size = sizeof(int32_t);
                        break;
                    case 'l':
                        size = sizeof(int64_t);
                        break;
                    case 'q':
                        size = sizeof(__int128_t);
                        break;
                    default:
                        fprintf(stderr, "Bad format string\n");
                        return -1;
                }
                packet_data = align(packet_data, size);
                memcpy(packet_data, packet_raw, size);
                reverse(packet_data, size);
                arr_len = *((int *)packet_data);
                packet_raw += size;
                packet_data += size;
                break;
            }
        }
        fmt++;
    }
    return packet_size_len + packet_size;
}
