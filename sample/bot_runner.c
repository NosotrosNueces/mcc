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
#define NUM_BOTS 1

int main(int argc, char *argv[], char **envp)
{
    char *server_name;
    int port;
    if (argc == 2) {
        server_name = argv[1];
        port = DEFAULT_SERVER_PORT;
    } else if (argc == 3) {
        server_name = argv[1];
        port = strtol(argv[2], NULL, 0);
        printf("port: %d\n", port);
    } else {
        server_name = SERVER_NAME;
        port = DEFAULT_SERVER_PORT;
    }

    bot_t *bots[NUM_BOTS];

    //bots[0] = init_defender("plants", server_name, DEFAULT_SERVER_PORT);
    bots[0] = init_slave("batman", server_name, port);

    client_run(bots, NUM_BOTS);

    free(server_name);
    for (int i = 0; i < NUM_BOTS; i++) {
        free_bot(bots[i]);
    }
    return 0;
}
