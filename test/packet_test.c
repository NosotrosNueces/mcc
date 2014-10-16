#include "marshal.h"
#include <stdio.h>
#include <assert.h>
#include "bot.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRUCT_SIZE 4096
#define NUM_FMT_SPECS 8
#define MAX_ARR_SIZE 128

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
__int128_t value(void *buf, size_t size) {
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
    assert(0);
}

int packet_equals(void *p1, void *p2) {
    char *fmt = *(char **)p1;
    p1 += sizeof(void *);
    p2 += sizeof(void *);
    printf("packet equals: fmt: %s\n", fmt);
    size_t size;
    size_t arr_size = -1;
    while(*fmt) {
        size = format_sizeof(*fmt);
        p1 = (void *)align(p1, size);
        p2 = (void *)align(p2, size);
        switch(*fmt) {
            case 's':
                //hexDump("str1: ", *(char **)p1, strlen(*(char **)p1));
                //hexDump("str2: ", *(char **)p2, strlen(*(char **)p2));
                if(strcmp(*(char **)p1, *(char **)p2))
                    return 0;
                break;
            case '*':
                fmt++;
                size_t elem_size = format_sizeof(*fmt);
                if(memcmp(*(void **)p1, *(void **)p2, arr_size * elem_size))
                    return 0;
                break;
            default:
                if((arr_size = value(p1, size)) != value(p2, size))
                    return 0;
                break;
        }
        p1 += size;
        p2 += size;
        fmt++;
    }
    return 1;
}

int test_fmt_str(char *fmt) {
    char test[STRUCT_SIZE];
    char decoded[STRUCT_SIZE];
    
    bot_t bot;
    bot_internal bot_int = {0, STRUCT_SIZE};
    bot._data = &bot_int;

    memset(decoded, 0, STRUCT_SIZE);
    //printf("format string: %s\n", fmt);

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
        switch(*fmt) {
            case 's': // varint followed by string
                // push random array size onto tmp
                ;
                char *str = malloc(MAX_ARR_SIZE);
                //fread(str, sizeof(char), MAX_ARR_SIZE, urandom);
                str[MAX_ARR_SIZE - 1] = 0;
                tmp = push(tmp, (intptr_t)str, size);
                break;
            case '*': // array
                fmt++;
                size_t elem_size = format_sizeof(*fmt);
                printf("Array size: %lu\n", arr_size);
                void *arr = malloc(elem_size * arr_size);
                tmp = push(tmp, (intptr_t)arr, size);
                break;
            default:
                arr_size = rand() % MAX_ARR_SIZE;
                tmp = push(tmp, arr_size, size); 
                break;
        }
        fmt++;
    }

    // encode packet
    uint8_t packet_raw[STRUCT_SIZE];
    int len = format_packet(&bot, test, &packet_raw);
    //fwrite(test, 1, STRUCT_SIZE, original_f);

    // decode packet
    decode_packet(NULL, packet_raw, decoded);
    uint8_t packet_raw_decode[STRUCT_SIZE];

    // re-encode packet
    int len_decode = format_packet(&bot, decoded, &packet_raw_decode);
    printf("len: %d, len_decode: %d\n", len, len_decode);
    int8_t equals = ((len == len_decode) && 
            packet_equals(test, decoded));
    free_packet(test);
    free_packet(decoded);
    return equals;
}
