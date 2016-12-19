#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "types.h"

/** \brief Initialize a bot with a name and main function
 *
 *  This function creates a bot object. The bot_main argument is used for
 *  threading purposes. If bot_main exits, the bot will cease to exist.
 *  bot_main will likely contain a large switch on some sort of global state.
 */
void init_bot(struct bot_agent *bot, char *name);

/** \brief Open a socket to the specified server
 *
 *  Open a socket connection to a specific server for a particular bot.
 */
void join_server_ipaddr(struct bot_agent *bot, char* server_host, int server_port);
void join_server_hostname(struct bot_agent *bot, char *server_hostname, char *service);

void disconnect(struct bot_agent *bot);

/** \brief Sends a string across the network using a bot's socket
 *
 *  Sends a raw string across the network using a bot's socket. This function
 *  should not be used unless you know what you're doing.
 */
/** \brief Recieve bytes from the network using a bot's socket
 *
 *  Recieve raw bytes from the network using a bot's socket. This function
 *  should not be used unless you know what you're doing.
 */
