#include <stdio.h>
#include <time.h>
#include "sample_defender.h"
#include "sample_slave.h"
#include "client.h"
#include "protocol.h"
#include "bot.h"
#include "api.h"

#define SERVER_NAME "localhost"
#define DEFAULT_SERVER_PORT 25565
#define NUM_BOTS 2

int main(int argc, char *argv[], char **envp)
{
    char *server_name;
    if (argc == 2) {
        server_name = argv[1];
    } else {
        server_name = SERVER_NAME;
    }

    bot_t *bots[NUM_BOTS];

    bots[0] = init_defender("plants", server_name, DEFAULT_SERVER_PORT);
    bots[1] = init_slave("batman", server_name, DEFAULT_SERVER_PORT);

    client_run(bots, NUM_BOTS);

    free(server_name);
    for (int i = 0; i < NUM_BOTS; i++) {
        free_bot(bots[i]);
    }
    return 0;
}
