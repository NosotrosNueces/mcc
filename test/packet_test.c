#include "marshal.h"
#include <stdio.h>
#include <assert.h>
#include "bot.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRUCT_SIZE 4096
#define NUM_FMT_SPECS 8
#define MAX_ARR_SIZE 8

char fmt_specifiers[NUM_FMT_SPECS] = {'s', 'b', 'h', 'w', 'v', 'l', 'q', '*'};

int random_fmt(char *, int);

FILE *urandom;

// performs function for each possible format string of length len
int for_each(char *fmt, uint32_t len, int (*func)(const char *),
        uint8_t arr_allowed) {

    int i;
    int j;
    int max = NUM_FMT_SPECS;
    // if we're at the first char, don't pick '*' or 's'
    if(!len) 
        return func(fmt);
    if(len == 1) {
        for(i = 0; i < NUM_FMT_SPECS - 1; i++) {
            fmt[len - 1] = fmt_specifiers[i];
            if(!func(fmt))
                return 0;
        }
    } else { 
        if(!arr_allowed)
            max--; 
        for(i = 0; i < max; i++) {
            fmt[len - 1] = fmt_specifiers[i];
            if(fmt[len - 1] == '*') {
                // if array, must be preceded by int/varint
                for(j = 1; j < NUM_FMT_SPECS - 2; j++) {
                    fmt[len - 2] = fmt_specifiers[j];
                    if(!for_each(fmt, len - 2, func, 0))
                        return 0;
                }
            } else if(fmt[len - 1] == 's') {
                // if str, must be preceded by varint
                if(!for_each(fmt, len - 1, func, 0))
                    return 0;
            } else {
                // otherwise, do whatever you want
                if(!for_each(fmt, len - 1, func, 1))
                    return 0;
            }
        }
    }
    return 1;
}

void init_random() {
    urandom = fopen("/dev/urandom", "r");
    int seed;
    fread(&seed, sizeof(int), 1, urandom);
    srand(seed);
}


int packet_equals(void *p1, void *p2) {
    char *fmt = *(char **)p1;
    p1 += sizeof(void *);
    p2 += sizeof(void *);
    size_t size;
    __int128_t arr_size = -1;
    while(*fmt) {
        size = format_sizeof(*fmt);
        p1 = (void *)align(p1, size);
        p2 = (void *)align(p2, size);
        switch(*fmt) {
            case 's':
                if(strcmp(*(char **)p1, *(char **)p2)) {
                    printf("String mismatch\n");
                    return 0;
                }
                break;
            case '*':
                fmt++;
                size_t elem_size = format_sizeof(*fmt);
                if(memcmp(*(void **)p1, *(void **)p2, arr_size * elem_size)){
                    printf("Array mismatch\n");
                    return 0;
                }
                break;
            default:
                if((arr_size = value_at(p1, size)) != value_at(p2, size)) {
                    printf("Integer/Varint mismatch\n");
                    return 0;
                }
                break;
        }
        p1 += size;
        p2 += size;
        fmt++;
    }
    return 1;
}

int test_fmt_str(char *fmt) {
    // structs
    char *test = malloc(STRUCT_SIZE);
    char *decoded = malloc(STRUCT_SIZE);

    bot_t bot;
    bot_internal bot_int = {0, STRUCT_SIZE};
    bot._data = &bot_int;

    memset(decoded, 0, STRUCT_SIZE);

    *((char **)test) = fmt;
    *((char **)decoded) = fmt;

    // search through fmt for '*' and 's', then initialize
    // array lengths and allocate for arrays
    size_t size;
    void *tmp = test + sizeof(void *);
    size_t arr_size;
    while(*fmt) {
        size = format_sizeof(*fmt);
        tmp = (void *)align(tmp, size);
        char var[5];
        switch(*fmt) {
            case 's': // varint followed by string
                // push random array size onto tmp
                ;
                char *str = malloc(MAX_ARR_SIZE);
                str[MAX_ARR_SIZE - 1] = 0;
                tmp = push(tmp, &str, size);
                break;
            case '*': // array
                fmt++;
                size_t elem_size = format_sizeof(*fmt);
                void *arr = malloc(elem_size * arr_size);
                tmp = push(tmp, &arr, size);
                break;
            default:
                arr_size = rand() % MAX_ARR_SIZE;
                tmp = push(tmp, &arr_size, size); 
                break;
        }
        fmt++;
    }

    // encode packet
    uint8_t packet_raw[STRUCT_SIZE];
    int len = format_packet(&bot, test, packet_raw);

    // decode packet
    decode_packet(NULL, packet_raw, decoded);
    uint8_t packet_raw_decode[STRUCT_SIZE];



    // re-encode packet
    int len_decode = format_packet(&bot, decoded, packet_raw_decode);
    int8_t equals = ((len == len_decode) && packet_equals(test, decoded) &&
            !memcmp(packet_raw, packet_raw_decode, len));
    free_packet(test);
    free_packet(decoded);
    return equals;
}
