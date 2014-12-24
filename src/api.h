#pragma once

#include "protocol.h"
#include "bot.h"

/* Briefly suspend thread execution for an interval measured in ms
 *
 *	Call nanosleep until it successfully sleeps for the required interval.
 */
void msleep(uint64_t ns);

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

/*
 * Encode a position from x,y,z.
 */
position_t encode_pos(int64_t x, int64_t y, int64_t z);

/*
 * Decode a location into x, y, and z integer pointers.
 */
void decode_pos(position_t pos, int64_t *x, int64_t *y, int64_t *z);

/*
 * Execute actions based on a command and arguments.
 * Both inputs are currently strings.
 */
void execute_command(bot_t *bot, char *command, char *strargs);

/*
 * This function parses user or server JSON for sender and message.
 *
 * User chat format:
 * data = {
 *   "with": [{"insertion": "JDongian",
 *             "text": "JDongian"},
 *            "hello"
 *           ]
 * }
 * Desired result (sender, message):
 * (data['with'][0]['text'], data['with'][1])
 *
 * Server chat format:
 * data = {
 *   "with": ["Server",
 *            {"extra": ["\dig", " ", "0", " ", "0", " ", "0"]}
 *           ]
 * }
 * Desired result (sender, message):
 * (data['with'][0], ''.join(data['with']['extra']))
 *
 * N.B.
 * This function allocates memory for *msg and *sender_name.
 * Please free it.
 */
void decode_chat_json(char *raw_json, char **msg, char **sender_name);
