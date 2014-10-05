#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include "bot.h"
#include "client.h"

void sample_main(void* dummy) {
    struct timespec req = {0, 50000000};
    struct timespec rem;
    while(1) {
        nanosleep(&req, &rem);
    }
}

int main() {
    /* bot_t *batman_bot = init_bot("batman", *sample_main); */
    /* client_run(batman_bot, 1); */
}
