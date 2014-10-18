#include "protocol_test.h"
#include "packet_test.h"
#include "bot.h"
#include <stdio.h>

#define LEN 6

int print_str(char *str) {
    printf("%s\n", str);
    return 1;
}

int main() {
    for (int j = 0; j < 10; j++) {
        bot_t *bot = init_bot("asdf", NULL);
        free_bot(bot);
    }
    char fmt[LEN + 1] = {0};
    // prints out every possible format string of length LEN
    init_random();
    int i;
    for (i = 1; i <= LEN; i++) {
        printf("Testing every format string of length %d\n", i);
        if(for_each(fmt, i, test_fmt_str, 0))
            printf("Passed every test\n");
        else
            printf("Format string %s failed\n", fmt);
    }
}
