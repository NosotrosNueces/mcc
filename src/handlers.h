#include "bot.h"

/*
 * On a successful login, set bot state to PLAY.
 */
void login_success_handler(bot_t *bot, void *vp);

/*
 * Echo back keepalive_ids.
 */
void keepalive_handler(bot_t *bot, void *vp);

/*
 * Echo position back to the server (confirm location).
 */
void position_handler(bot_t *bot, void *vp);
