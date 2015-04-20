#include <stdio.h>
#include "packet_test.h"
#include "api.h"

#define POS_TEST_LEN 8
#define NEG_TEST_LEN 5

int64_t pos_tests[POS_TEST_LEN*4] = {0x0000000000000000, 0, 0, 0,
                                     0x0000000000000011, 0, 0, 0x11,
                                     0x0000000000000000, 0, 0, 0,
                                     0x0000000000000000, 0, 0, 0,
                                     0x0000000000000000, 0, 0, 0,
                                     0x0000000000000000, 0, 0, 0,
                                     0x0000000000000000, 0, 0, 0
                                    };

int64_t neg_tests[NEG_TEST_LEN*4] = {0x0000000000000000, 0, 0, 1,
                                     0x0000000000000000, 0, 1, 0,
                                     0x0000000000000000, 1, 0, 0,
                                     0xffffffffffffffff, 0, 0, 0,
                                     0x000000000000000f, 0xf, 0, 0
                                    };

void unload_test(int64_t *test,
                 position_t *pos, int64_t *x, int64_t *y, int64_t *z)
{
    *pos = *test++;
    *x = *test++;
    *y = *test++;
    *z = *test;
}

bool position_equals(position_t pos, int64_t x, int64_t y, int64_t z)
{
    int64_t x_0, y_0, z_0;
    decode_pos(pos, &x_0, &y_0, &z_0);
    bool decode_works = (x == x_0) && (y == y_0) && (z == z_0);
    bool encode_works = encode_pos(x, y, z) == pos;
    return decode_works && encode_works;
}

void position_test(void)
{
    position_t pos;
    int64_t x,y,z;
    int i;
    // Positive tests
    for (i = 0; i < POS_TEST_LEN; ++i) {
        unload_test(&pos_tests[i*4], &pos, &x, &y, &z);
        if (!position_equals(pos, x, y, z)) {
            printf("Position encoding or decoding failed on "
                   "pos=%0lx, x=%ld, y=%ld, z=%ld.", pos, x, y, z);
            return;
        }
    }
    // Negative tests
    for (i = 0; i < NEG_TEST_LEN; ++i) {
        unload_test(&neg_tests[i*4], &pos, &x, &y, &z);
        if (position_equals(pos, x, y, z)) {
            printf("Position encoding or decoding failed on"
                   "pos=%0lx, x=%ld, y=%ld, z=%ld.", pos, x, y, z);
            return;
        }
    }
}
