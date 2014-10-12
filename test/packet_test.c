#include "marshal.h"
#include <stdio.h>
#include <assert.h>
#include "bot.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRUCT_SIZE 4096
#define NUM_FMT_SPECS 8

char fmt_specifiers[NUM_FMT_SPECS] = {'s', 'b', 'h', 'w', 'l', 'q', 'v', '*'};

int random_fmt(char *, int);

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
                for(j = 1; j < NUM_FMT_SPECS - 1; j++) {
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

//int test_fmt_str_noptr(char *fmt) {
//    char test[STRUCT_SIZE];
//    char decoded[STRUCT_SIZE];
//
//    bot_t bot;
//    bot_internal bot_int = {0, STRUCT_SIZE};
//    bot._data = &bot_int;
//
//    memset(decoded, 0, STRUCT_SIZE);
//    //printf("format string: %s\n", str);
//
//    *((char **)test) = fmt;
//    *((char **)decoded) = fmt;
//
//    // encode packet
//    uint8_t packet_raw[STRUCT_SIZE];
//    int len = format_packet(&bot, test, &packet_raw);
//    //fwrite(test, 1, STRUCT_SIZE, original_f);
//
//    // decode packet
//    decode_packet(NULL, packet_raw, decoded);
//    uint8_t packet_raw_decode[STRUCT_SIZE];
//
//    // re-encode packet
//    int len_decode = format_packet(&bot, decoded, &packet_raw_decode);
//    return (len == len_decode) && !memcmp(packet_raw, packet_raw_decode, len);
//}

int test_fmt_str_all(char *fmt) {

}
