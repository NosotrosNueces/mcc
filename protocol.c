#include "protocol.h"
#include <string.h>
#include <stdio.h>

#define expect_more(x) (x & 0x80)
// align pointer x to y
// y is assumed to be a power of 2
#define align(x, y) ((void *)((size_t)(x + y) & (~y + 1)))

// returns the number of bytes read from data
int uvarint64(char *data, uint64_t *value){
    uint64_t result = 0;
    int shifts = 0;
    do{
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    }while(expect_more(*data++));
    *value = result;
    return shifts;
}

// returns the number of bytes read from data
int uvarint32(char *data, uint32_t *value){
    uint32_t result = 0;
    int shifts = 0;
    do{
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    }while(expect_more(*data++));
    *value = result;
    return shifts;
}

int uvarint64_encode(uint64_t value, char *data, int len){
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    while(value){
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value >>= 7;
        i++;
    }
    data[i - 1] &= mask;
    return i;
}

int uvarint32_encode(uint32_t value, char *data, int len){
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    while(value){
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value >>= 7;
        i++;
    }
    data[i - 1] &= mask;
    return i;
}

// puts the raw packet data from the struct packet_data into packet_raw
// returns the number of bytes written to packet_raw, or -1 if packet_raw
// is not a long enough array
int format_packet(void *packet_data, void *packet_raw, int len){
    memset(packet_raw, 0, len);
    // length of packet calculated at the end
    //__int128_t i = 0;
    int index = 0;
    char *fmt = packet_data;
    while(*fmt){
        switch(*fmt){
            case '*': // pointer/array
                break;
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
                    return -1;
                packet_data = align(packet_data, size);
                memcpy(packet_raw + index, packet_data, size);
                index += size;
                break;
            }
        }
    }
    return index;
}


//int main(){
//    void *i = 0;
//    for(; i < 64; i++){
//        printf("%p\n", i);
//        printf("%p\n", align(i, 1));
//        printf("%p\n", align(i, 2));
//        printf("%p\n", align(i, 4));
//        printf("%p\n", align(i, 8));
//        printf("%p\n", align(i, 16));
//        printf("\n\n");
//    }
//}
