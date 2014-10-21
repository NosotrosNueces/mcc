#include <stdio.h>
#include <time.h>
#include "sample_defender.h"
#include "sample_slave.h"
#include "client.h"
#include "protocol.h"
#include "bot.h"
#include "api.h"

#define SERVER_NAME "10.10.2.16"
#define DEFAULT_SERVER_PORT 25565
#define NUM_BOTS 2

int main(int argc, char *argv[])
{
    bot_t *bots[NUM_BOTS];

    bots[0] = init_defender("plants", SERVER_NAME, DEFAULT_SERVER_PORT);
    bots[1] = init_slave("batman", SERVER_NAME, DEFAULT_SERVER_PORT);

    client_run(bots, NUM_BOTS);
    for (int i = 0; i < NUM_BOTS; i++) {
        free_bot(bots[i]);
    }
    return 0;
}
