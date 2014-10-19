#include <stdio.h>
#include <time.h>
#include "sample_defender.h"
#include "client.h"
#include "protocol.h"
#include "bot.h"
#include "api.h"

#define SERVER_NAME "10.10.2.16"
#define DEFAULT_SERVER_PORT 25565
#define NUM_BOTS 1

int main(int argc, char *argv[]) {
    bot_t *bots[NUM_BOTS];
    
    bots[0] = defender_init("plants");

    client_run(bots, NUM_BOTS);
    for (int i = 0; i < NUM_BOTS; i++) {
        free_bot(bots[i]);
    }
    return 0;
}
