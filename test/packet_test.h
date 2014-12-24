#include <stdint.h>

// length of format strings to brute force
#define FORMAT_LENGTH 5

// recursive function that generates all possible format strings. In the
// initial call to this function, must pass in len(fmt) - 1 as the length
int for_each(char *, uint32_t, int (*)(char *), uint8_t);

// string must be null terminated
int test_fmt_str(char *);

// initialize RNG
void init_random();

// run the packet test
void packet_test(void);
