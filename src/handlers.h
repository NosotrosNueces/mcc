#pragma once

#include "protocol.h"
#include "bot.h"

/*
 * On a successful login, set bot state to PLAY.
 */
void mcc_login_success_handler(bot_t *bot, char *uuid, char *username);

/*
 * Echo back keepalive_ids.
 */
void mcc_keepalive_handler(bot_t *bot, vint32_t keepalive_id);

/*
 * Set bot data:
 *      eid,
 *      gamemode,
 *      dimension,
 *      difficulty,
 *      max_players,
 *      level_type
 */
void mcc_join_game_handler(bot_t *bot, int32_t entity_id,
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
void mcc_chat_handler(bot_t *bot, char *json, int8_t position);

/*
 * Update bot health, food, and saturation.
 */
void mcc_update_health_handler(bot_t *bot,
                               float health,
                               vint32_t food,
                               float saturation);

/*
 * Immediately respawn whenever dead.
 */
void mcc_autorespawn_handler(bot_t *bot,
                             float health,
                             vint32_t food,
                             float saturation);

/*
 * Echo position back to the server (confirm location).
 */
void mcc_position_handler(bot_t *bot,
                          double x,
                          double y,
                          double z,
                          float yaw,
                          float pitch,
                          int8_t flags);
