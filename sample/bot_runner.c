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
    char *server_name = SERVER_NAME;
    int server_port = DEFAULT_SERVER_PORT;
    if (argc == 3) {
        server_name = argv[1];
        server_port = strtol(argv[2], NULL, 10);
        if (!server_port) {
            printf("Expected arguments: ./mcc [<SERVER> [<PORT>]]\n");
            return 0;
        }
    } else if (argc == 2) {
        server_name = argv[1];
    } else if (argc > 3) {
        printf("Expected arguments: ./mcc [<SERVER> [<PORT>]]\n");
        return 0;
    }

    bot_t *bots[NUM_BOTS];

    bots[0] = init_defender("plants", server_name, server_port);
    //bots[0] = init_slave("batman", server_name, server_port);

    client_run(bots, NUM_BOTS);

    free(server_name);
    for (int i = 0; i < NUM_BOTS; i++) {
        free_bot(bots[i]);
    }
    return 0;
}
