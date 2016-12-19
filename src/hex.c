#include <stdio.h>
#include "hex.h"



int hexdump(char *buf, int len) {
    return 0;
}

inline int hexline_len(int len) {
    return len * 2 + 1;
}

int hexline(const unsigned char *buf, int len, char *str) {
    int i = 0;
    for (; i + 16 <= len; i += 16) {
        sprintf(&str[2 * i],
                "%02x%02x%02x%02x%02x%02x%02x%02x"
                "%02x%02x%02x%02x%02x%02x%02x%02x",
                buf[i], buf[i + 1], buf[i + 2], buf[i + 3],
                buf[i + 4], buf[i + 5], buf[i + 6], buf[i + 7],
                buf[i + 8], buf[i + 9], buf[i + 10], buf[i + 11],
                buf[i + 12], buf[i + 13], buf[i + 14], buf[i + 15] 
                );
    }
    for (; i < len; i++) {
        sprintf(&str[2 * i], "%02x", buf[i]);
    }
    return len;
}

/*

int main() {
    unsigned char buf[] = "Hello World!";
    char str[27];
    hexline(buf, sizeof(buf), str);
    printf("%lu, %s\n", sizeof(buf), str);
}

*/
