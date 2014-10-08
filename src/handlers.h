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
 * Update bot health, food, and saturation.
 */
void update_health_handler(bot_t *, void *);

/*
 * Echo position back to the server (confirm location).
 */
void position_handler(bot_t *, void *);
