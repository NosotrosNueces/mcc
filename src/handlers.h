#pragma once

#include "bot.h"

/*
 * On a successful login, set bot state to PLAY.
 */
void login_success_handler(bot_t *, void *);

/*
 * Echo back keepalive_ids.
 */
void keepalive_handler(bot_t *, void *);

/*
 * Set bot data:
 *      eid,
 *      gamemode,
 *      dimension,
 *      difficulty,
 *      max_players,
 *      level_type
 */
void join_game_handler(bot_t *, void *);

/*
 * Parse the JSON data from users or the server.
 * If the message is in the form \<command> [<arg1> <arg2> ...],
 * excecute the command with the given args.
 */
void chat_handler(bot_t *bot, void *vp);

/*
 * Update bot health, food, and saturation.
 */
void update_health_handler(bot_t *, void *);

/*
 * Echo position back to the server (confirm location).
 */
void position_handler(bot_t *, void *);
