#pragma once

#include "bot.h"

/*
 * On a successful login, set bot state to PLAY.
 */
void login_success_handler(bot_t *bot, char *uuid, char *username);

/*
 * Echo back keepalive_ids.
 */
void keepalive_handler(bot_t *bot, vint32_t keepalive_id);

/*
 * Set bot data:
 *      eid,
 *      gamemode,
 *      dimension,
 *      difficulty,
 *      max_players,
 *      level_type
 */
void join_game_handler(bot_t *bot, 
        int32_t entity_id, 
        uint8_t gamemode, 
        int8_t dimension, 
        uint8_t difficulty, 
        uint8_t max_players, 
        char* level_type);
/*
 * Parse the JSON data from users or the server.
 * If the message is in the form \<command> [<arg1> <arg2> ...],
 * excecute the command with the given args.
 */
void chat_handler(bot_t *bot, char *json, int8_t position);

/*
 * Update bot health, food, and saturation.
 */
void update_health_handler(bot_t *bot, 
        float health, 
        vint32_t food, 
        float saturation);

/*
 * Auto-respawn if dead.
 */
void respawn_handler(bot_t *bot,
        float health,
        vint32_t food,
        float saturation);

/*
 * Echo position back to the server (confirm location).
 */
void position_handler(bot_t *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch,
        int8_t flags);
