#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "bot.h"
#include "serial.h"

#define DEFAULT_THRESHOLD (512)


int send_string(struct bot_agent *bot, char *str);

int send_uncompressed(struct bot_agent *bot, struct packet_write_buffer *data, size_t len);
int send_compressed(struct bot_agent *bot, struct packet_write_buffer *data, size_t len);


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
		struct mc_position looked_at_block
		);

int32_t send_play_serverbound_chat_message(
		struct bot_agent *bot,
		const char *message
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

int32_t send_play_serverbound_keep_alive(
		struct bot_agent*        bot,
		vint32_t      keep_alive_id
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
        struct mc_position location,
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
        struct mc_position location,
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
        struct mc_position location,
        vint32_t face,
        vint32_t hand,
        float x,
        float y,
        float z
        );

int32_t send_play_serverbound_use_item(
        struct bot_agent *bot,
        vint32_t hand
        );

void read_socket(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
