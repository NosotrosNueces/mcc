#include "marshal.h"
#include <stdio.h>
#include <assert.h>
#include "bot.h"
#include <stdlib.h>
#include <string.h>

#define STRUCT_SIZE 4096

char fmt_specifiers[6] = {'b', 'h', 'w', 'l', 'q', 'v'};

char *random_fmt();

FILE *f;
FILE *original_f;
FILE *decoded_f;
void test_no_pointers(uint64_t trials){ // test
    bot_t an_bot = {0, STRUCT_SIZE};
    void *test = malloc(STRUCT_SIZE);
    void *decoded = malloc(STRUCT_SIZE);
    uint64_t i;
    for(i = 0; i < trials; i++){
        memset(decoded, 0, STRUCT_SIZE);
        fread(test, sizeof(int8_t), STRUCT_SIZE, f);
        char *str = random_fmt();
        //printf("format string: %s\n", str);

        *((char **)test) = str;
        *((char **)decoded) = str;

        // encode packet
        void *packet_raw;
        int len = format_packet(&an_bot, test, &packet_raw);
        //fwrite(test, 1, STRUCT_SIZE, original_f);

        // decode packet
        decode_packet(packet_raw, decoded);
        void *packet_raw_decode;

        // re-encode packet
        int len_decode = format_packet(&an_bot, decoded, &packet_raw_decode);
        //fwrite(decoded, 1, STRUCT_SIZE, decoded_f);

        // check to make sure they match
        assert(len_decode == len);
        assert(!memcmp(packet_raw, packet_raw_decode, len));
        free(str);
        free(packet_raw);
        free(packet_raw_decode);
    }
    free(test);
    free(decoded);
}

char *random_fmt(){
    char *str = calloc(STRUCT_SIZE / 32, sizeof(char));
    int i = 0;
    for(; i < STRUCT_SIZE / 32 - 1; i++){
        int index = rand() % sizeof(fmt_specifiers);
        str[i] = fmt_specifiers[index];
    }
    return str;
}

int main(){
    f = fopen("/dev/urandom", "r");
    int seed;
    fread(&seed, 1, sizeof(int), f);
    srand(seed);
    test_no_pointers(~0lu);
    fclose(f);
}
