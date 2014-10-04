#include "marshal.h"
#include <stdio.h>
#include <assert.h>
#include "bot.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRUCT_SIZE 4096

char fmt_specifiers[6] = {'b', 'h', 'w', 'l', 'q', 'v'};

int random_fmt(char *, int);

FILE *f;
FILE *original_f;
FILE *decoded_f;
void test_no_pointers(uint64_t trials){ // test
    bot_t an_bot = {0, STRUCT_SIZE};
    void *test = malloc(STRUCT_SIZE);
    void *decoded = malloc(STRUCT_SIZE);
    uint64_t i;
    char fmt[STRUCT_SIZE + 1];
    for(i = 0; i < trials; i++){
        memset(decoded, 0, STRUCT_SIZE);
        fread(test, sizeof(int8_t), STRUCT_SIZE, f);
        random_fmt(fmt, STRUCT_SIZE + 1);
        //printf("format string: %s\n", str);

        *((char **)test) = fmt;
        *((char **)decoded) = fmt;

        // encode packet
        void *packet_raw;
        int len = format_packet(&an_bot, test, &packet_raw);
        //fwrite(test, 1, STRUCT_SIZE, original_f);

        // decode packet
        decode_packet(NULL, packet_raw, decoded);
        void *packet_raw_decode;

        // re-encode packet
        int len_decode = format_packet(&an_bot, decoded, &packet_raw_decode);
        //fwrite(decoded, 1, STRUCT_SIZE, decoded_f);

        // check to make sure they match
        assert(len_decode == len);
        assert(!memcmp(packet_raw, packet_raw_decode, len));
        free(packet_raw);
        free(packet_raw_decode);
    }
    free(test);
    free(decoded);
}



int random_fmt(char *str, int len){
    size_t size = 0; // size that the fmt specifier string represents
    int i = 0; // index within str
    int index = rand() % sizeof(fmt_specifiers);
    while(i < len && (size += format_sizeof(fmt_specifiers[index])) < STRUCT_SIZE) {
        str[i++] = fmt_specifiers[index];
        int index = rand() % sizeof(fmt_specifiers);
    }
    str[i] = 0;
    return size;
}

int main(int argc, char **argv){
    if(argc < 2) {
        fprintf(stderr, "Please specify how many tests\n");
        return 1;
    }
    f = fopen("/dev/urandom", "r");
    int seed;
    fread(&seed, 1, sizeof(int), f);
    srand(seed);
    test_no_pointers(~0lu);
    fclose(f);
    //char varint[100];
    //printf("%d bytes written\n", varint32_encode(-1, varint, 100));
    //printf("%hhX %hhX %hhX %hhX %hhX\n", varint[0], varint[1], varint[2], varint[3], varint[4]);
}
