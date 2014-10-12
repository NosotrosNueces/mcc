#include <stdint.h>

//void test_no_pointers(uint64_t);

// recursive function that generates all possible format strings. In the 
// initial call to this function, must pass in len(fmt) - 1 as the length
int for_each(char *, uint32_t, int (*)(char *), uint8_t);

// string must be null terminated
int test_fmt_str(char *);

void init_random();
