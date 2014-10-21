#include "bot.h"

/* Briefly suspend thread execution for an interval measured in ms
 *
 *	Call nanosleep until it successfully sleeps for the required interval.
 */
void hsleep(uint64_t ns);

/*
 * Registers
 *      keepalive_handler,
 *      login_success_handler,
 *      position_handler.
 */
void register_defaults(bot_t *bot);

/*
 * Connect to the server, initiate the handshake, and login.
 */
void login(bot_t *bot, char *server_address, int port);
