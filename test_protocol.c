
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "protocol.h"
#include "bot.h"


void hexDump (char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        printf ("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}


typedef struct test_protocol {
    char*         format;
    vint32_t      packet_id;

    uint64_t data[16];
} test_protocol_t;

#define TEST_CASES_FORMAT (6)

char* test_formats[TEST_CASES_FORMAT] = {
	"vqqqqqqqq",
	"vllllllllllllllll",
	"vlwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
	"vlvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv",
	"vlhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh",
	"vlbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
};

void test_random_protocol() {
	test_protocol_t test;
	test_protocol_t test_decode;
	
	bot_t test_bot;
	test_bot.packet_threshold = 8192;

	void* test_output;

	// fill our test structure with pseudo-random data
	memset(&test, 0, sizeof(test_protocol_t));
	for (int i = 0; i < 16; i++) {
		test.data[i] = (i + 0xcafebabe) * 0xdeadbeefcafeface;
	}

	// encode random data with some fixed format strings
	// make sure the decoded version is equivalent
	int length;
	for (int i = 0; i < TEST_CASES_FORMAT; i++) {
		test.format = test_formats[i];
		test.packet_id = i;

		memset(&test_decode, 0, sizeof(test_protocol_t));
		test_decode.format = test.format;
		test_decode.packet_id = test.packet_id;

		length = format_packet(&test_bot, &test, &test_output);

		decode_packet(test_output, &test_decode);

		void* original = (void*) test.data;
		void* decoded = (void*) test_decode.data;

		assert(memcmp(original, decoded, 128) == 0);
	}

	printf("test_random_protocol passed!\n");
}

int main() {
	test_random_protocol();
}