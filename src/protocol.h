#pragma once

#include <stdint.h>
#include "bot.h"
#include <stdbool.h>
#include "types.h"

#define DEFAULT_THRESHOLD (512)

int32_t send_handshaking_serverbound_handshake(
		struct bot_agent*        bot,
		vint32_t      protocol_version,
		char*         server_addr,
		uint16_t      server_port,
		vint32_t      next_state
		);

/*
 * Login serverbound functions
 */

int32_t send_login_serverbound_login_start(
		struct bot_agent*        bot,
		char*         username
        );

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
		struct bot_agent*        bot
		);

int32_t send_status_serverbound_ping(
		struct bot_agent*        bot,
		int64_t       time
		);

/*
 * Play serverbound functions
 */

int32_t send_play_serverbound_teleport_confirm(
		struct bot_agent *bot,
		vint32_t teleport_id
		);

int32_t send_play_serverbound_tab_complete(
		struct bot_agent *bot,
		char *text,
		bool assume_command,
		bool has_position,
		position_t looked_at_block
		);

int32_t send_play_serverbound_chat_message(
		struct bot_agent *bot,
		char *message
		);

int32_t send_play_serverbound_client_status(
		struct bot_agent *bot,
		vint32_t action_id
		);

int32_t send_play_serverbound_client_settings(
		struct bot_agent *bot,
		char *locale,
		int8_t view_distance,
		vint32_t chat_mode,
		bool chat_colors,
		uint8_t displayed_skin_parts,
		vint32_t main_hand
		);

int32_t send_play_serverbound_confirm_transaction(
		struct bot_agent *bot,
		int8_t window_id,
		int16_t action_number,
		bool accepted
		);

int32_t send_play_serverbound_enchant_item(
		struct bot_agent *bot,
		int8_t window_id,
		int8_t enchantment
		);

int32_t send_play_serverbound_click_window(
		struct bot_agent *bot,
		uint8_t window_id,
		int16_t slot,
		int8_t button,
		int16_t action_number,
		vint32_t mode,
        struct slot_type *clicked_item
		);

int32_t send_play_serverbound_close_window(
        struct bot_agent *bot,
        uint8_t window_id
        );

int32_t send_play_serverbound_plugin_message(
        struct bot_agent *bot,
        char *channel,
        uint32_t data_length,
        char *data
        );

int32_t send_play_serverbound_use_entity(
        struct bot_agent *bot,
        vint32_t target,
        vint32_t type,
        float target_x,
        float target_y,
        float target_z,
        vint32_t hand
);

int32_t send_play_serverbound_keepalive(
		struct bot_agent*        bot,
		vint32_t      keepalive_id
);

int32_t send_play_serverbound_player_position(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        bool on_ground
);

int32_t send_play_serverbound_player_position_look(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch,
        bool on_ground
);

int32_t send_play_serverbound_player_look(
        struct bot_agent *bot,
        float yaw,
        float pitch,
        bool on_ground
);

int32_t send_play_serverbound_player(
        struct bot_agent *bot,
        bool on_ground
);

int32_t send_play_serverbound_vehicle_move(
        struct bot_agent *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch
);

int32_t send_play_serverbound_steer_boat(
        struct bot_agent *bot,
        bool right_paddle,
        bool left_paddle
);

int32_t send_play_serverbound_player_abilities(
        struct bot_agent *bot,
        int8_t flags,
        float flying_speed,
        float walking_speed
);

int32_t send_play_serverbound_player_digging(
        struct bot_agent *bot,
        vint32_t status,
        position_t location,
        int8_t face
);

int32_t send_play_serverbound_entity_action(
        struct bot_agent *bot,
        vint32_t entity_id,
        vint32_t action_id,
        vint32_t jump_boost
);

int32_t send_play_serverbound_steer_vehicle(
        struct bot_agent *bot,
        float sideways,
        float forward,
        uint8_t flags
);

int32_t send_play_serverbound_resource_pack_status(
        struct bot_agent *bot,
        char *hash,
        vint32_t result
);

int32_t send_play_serverbound_held_item_change(
        struct bot_agent *bot,
        int16_t slot
);

int32_t send_play_serverbound_creative_inventory_action(
        struct bot_agent *bot,
        int16_t slot,
        struct slot_type *clicked_item
);

int32_t send_play_serverbound_update_sign(
        struct bot_agent *bot,
        position_t location,
        char *line1,
        char *line2,
        char *line3,
        char *line4
);

int32_t send_play_serverbound_animation(
        struct bot_agent *bot,
        vint32_t hand
        );

int32_t send_play_serverbound_spectate(
        struct bot_agent *bot,
        char *uuid
        );

int32_t send_play_serverbound_player_block_placement(
        struct bot_agent *bot,
        position_t location,
        vint32_t face,
        vint32_t hand,
        uint8_t x,
        uint8_t y,
        uint8_t z
        );

int32_t send_play_serverbound_use_item(
        struct bot_agent *bot,
        vint32_t hand
        );
