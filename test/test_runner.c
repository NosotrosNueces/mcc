#include "protocol_test.h"
#include "packet_test.h"
#include <stdio.h>

#define LEN 6

int print_str(char *str) {
    printf("%s\n", str);
    return 1;
}

int main() {
    char fmt[LEN + 1] = {0};
    // prints out every possible format string of length LEN
    for_each(fmt, LEN, print_str, 0);
}
