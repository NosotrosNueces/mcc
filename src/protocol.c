#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "protocol.h"
#include "marshal.h"
#include "bot.h"
#include "../cNBT/nbt.h"

#define PAD_LENGTH(x) (x = (char *)x + 5)

// function to send packets to server
uint32_t _render_send(bot_t *bot, void *p)
{
	int8_t packet[bot->_data->packet_threshold]; /* <--- this right here... no me gusta */
	uint32_t length = format_packet(bot, p, (void *) &packet);
	send_raw(bot, packet, length);
	return length;
}

void *_push_vint32(void *buf, vint32_t val) {
	char varint[5];
	uint32_t len = varint32_encode(val, varint, sizeof(varint));
	buf = push(buf, varint, len);
	return buf;
}

void *_push_string(void *buf, char *str) {
	uint32_t bytes_pushed, len;
	size_t s_len = strlen(str);
	char str_len[5];
	bytes_pushed = varint32_encode(s_len, str_len, sizeof(str_len));
	buf = push(buf, str_len, bytes_pushed);
	buf = push(buf, str, s_len);
	return buf; 
}

void *_push_slot(void *_packet_raw, slot_t *slot_data)
{
	int16_t block_id = slot_data->block_id;

	char *packet_raw = _packet_raw;
	if (-1 == block_id) { // Empty slot is 0xffff
		return push(packet_raw, &block_id, sizeof(block_id));
	} else {
		int8_t count = slot_data->count;
		int16_t damage = slot_data->damage;
		nbt_node *tree = slot_data->tree;

		/* Convert little to big endian; flip the bytes. */
		block_id = htons(block_id);
		damage = htons(damage);

		/* copy block_id (2 bytes) */
        packet_raw = push(packet_raw, &block_id, sizeof(block_id));

		/* copy count (1 byte) */
        packet_raw = push(packet_raw, &count, sizeof(count));

		/* copy damage (2 bytes) */
        packet_raw = push(packet_raw, &damage, sizeof(damage));

		if (tree) {
			/* convert nbt tree to binary nbt data */
			struct buffer nbt_data = nbt_dump_binary(tree);
            packet_raw = push(packet_raw, nbt_data.data, nbt_data.len);
			free(nbt_data.data);
		} else {
			memset(packet_raw, 0, sizeof(int8_t));
			packet_raw += sizeof(int8_t);
		}
	}
	return packet_raw;
}

uint64_t htonll(uint64_t number)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    number = __builtin_bswap64(number);
#endif
    return number;
}

/* appends length to the buffer as a varint, returns the start of the buffer */
/* writes the new length of packet in len */
void *_append_len(void *buf, uint32_t *len) {
	char buf_len[5];
	int offset = varint32_encode(*len, buf_len, sizeof(buf_len));
	*len += offset;
	void *start = (char *)buf + 5 - offset;
	push(start, buf_len, offset);
	return start;
}

/*
 * Handshaking serverbound functions
 */

int32_t send_handshaking_serverbound_handshake(
		bot_t*        bot,
		vint32_t      protocol_version,
		char*         server_addr,
		uint16_t      server_port,
		vint32_t      next_state
		) {
    server_port = htons(server_port);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x00);
	buf = _push_vint32(buf, protocol_version);
	buf = _push_string(buf, server_addr);
	buf = push(buf, &server_port, sizeof(server_port));
	buf = _push_vint32(buf, next_state);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

/*
 * Login serverbound functions
 */

int32_t send_login_serverbound_login_start(
		bot_t*        bot,
		char*         username
		)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x00);
	buf = _push_string(buf, username);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

/*
 * Status serverbound functions
 */

int32_t send_status_serverbound_request(
		bot_t*        bot
		)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x00);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_status_serverbound_ping(
		bot_t*        bot,
		int64_t       time
		)
{
    time = htonll(time);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x01);
	buf = push(buf, &time, sizeof(time));
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

/*
 * Play serverbound functions
 */

int32_t send_play_serverbound_teleport_confirm(
		bot_t *bot,
		vint32_t teleport_id
		) 
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x00);
	buf = _push_vint32(buf, teleport_id);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;

}

int32_t send_play_serverbound_tab_complete(
		bot_t *bot,
		char *text,
		bool assume_command,
		bool has_position,
		position_t looked_at_block
		)
{
    looked_at_block = htonll(looked_at_block);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x01);
	buf = _push_string(buf, text);
	int8_t b = assume_command ? 1 : 0;
	buf = push(buf, &b, sizeof(b));
	b = has_position ? 1 : 0;
	buf = push(buf, &b, sizeof(b));
	if (has_position) {
		buf = push(buf, &looked_at_block, sizeof(looked_at_block));
	}
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_chat_message(
		bot_t *bot,
		char *message
		)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x02);
	buf = _push_string(buf, message);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_client_status(
		bot_t *bot,
		vint32_t action_id
		)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x03);
	buf = _push_vint32(buf, action_id);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_client_settings(
		bot_t *bot,
		char *locale,
		int8_t view_distance,
		vint32_t chat_mode,
		bool chat_colors,
		uint8_t displayed_skin_parts,
		vint32_t main_hand
		)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x04);
	buf = _push_string(buf, locale);
	buf = push(buf, &view_distance, sizeof(view_distance));
	buf = _push_vint32(buf, chat_mode);
	int8_t b = chat_colors ? 1 : 0;
	buf = push(buf, &b, sizeof(b));
	buf = push(buf, &displayed_skin_parts, sizeof(displayed_skin_parts));
	buf = _push_vint32(buf, main_hand);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_confirm_transaction(
		bot_t *bot,
		int8_t window_id,
		int16_t action_number,
		bool accepted
		)
{
    action_number = htons(action_number);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x05);
	buf = push(buf, &window_id, sizeof(window_id));
	buf = push(buf, &action_number, sizeof(action_number));
	int8_t b = accepted ? 1 : 0;
	buf = push(buf, &b, sizeof(b));

	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_enchant_item(
		bot_t *bot,
		int8_t window_id,
		int8_t enchantment
		)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x06);
	buf = push(buf, &window_id, sizeof(window_id));
	buf = push(buf, &enchantment, sizeof(enchantment));

	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_click_window(
		bot_t *bot,
		uint8_t window_id,
		int16_t slot,
		int8_t button,
		int16_t action_number,
		vint32_t mode,
        slot_t *clicked_item
		)
{
    slot = htons(slot);
    action_number = htons(action_number);
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

    buf = _push_vint32(buf, 0x07);
    buf = push(buf, &window_id, sizeof(window_id));
    buf = push(buf, &slot, sizeof(slot));
    buf = push(buf, &button, sizeof(button));
    buf = push(buf, &action_number, sizeof(action_number));
    buf = _push_vint32(buf, mode);
    buf = _push_slot(buf, clicked_item);
    
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_close_window(
        bot_t *bot,
        uint8_t window_id
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x08);
    buf = push(buf, &window_id, sizeof(window_id));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_plugin_message(
        bot_t *bot,
        char *channel,
        uint32_t data_length,
        void *data
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x09);
    buf = _push_string(buf, channel);
    buf = push(buf, data, data_length); 

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_use_entity(
        bot_t *bot,
        vint32_t target,
        vint32_t type,
        float target_x,
        float target_y,
        float target_z,
        vint32_t hand
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x0A);
    buf = _push_vint32(buf, target);
    buf = _push_vint32(buf, type);
    switch(type) {
        case 2: /* interact at */
            buf = push(buf, &target_x, sizeof(target_x));
            buf = push(buf, &target_y, sizeof(target_y));
            buf = push(buf, &target_z, sizeof(target_z));
        case 0: /* interact */
            buf = _push_vint32(buf, hand);
    }

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_keepalive(
		bot_t*        bot,
		vint32_t      keepalive_id
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x0B);
    buf = _push_vint32(buf, keepalive_id);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_position(
        bot_t *bot,
        double x,
        double y,
        double z,
        bool on_ground
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x0C);
    buf = push(buf, &x, sizeof(x));
    buf = push(buf, &y, sizeof(y));
    buf = push(buf, &z, sizeof(z));
    int8_t b = on_ground ? 1 : 0;
    buf = push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_position_look(
        bot_t *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch,
        bool on_ground
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x0D);
    buf = push(buf, &x, sizeof(x));
    buf = push(buf, &y, sizeof(y));
    buf = push(buf, &z, sizeof(z));
    buf = push(buf, &yaw, sizeof(yaw));
    buf = push(buf, &pitch, sizeof(pitch));
    int8_t b = on_ground ? 1 : 0;
    buf = push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_look(
        bot_t *bot,
        float yaw,
        float pitch,
        bool on_ground
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x0E);
    buf = push(buf, &yaw, sizeof(yaw));
    buf = push(buf, &pitch, sizeof(pitch));
    int8_t b = on_ground ? 1 : 0;
    buf = push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player(
        bot_t *bot,
        bool on_ground
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x0F);
    int8_t b = on_ground ? 1 : 0;
    buf = push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_vehicle_move(
        bot_t *bot,
        double x,
        double y,
        double z,
        float yaw,
        float pitch
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x10);
    buf = push(buf, &x, sizeof(x));
    buf = push(buf, &y, sizeof(y));
    buf = push(buf, &z, sizeof(z));
    buf = push(buf, &yaw, sizeof(yaw));
    buf = push(buf, &pitch, sizeof(pitch));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_steer_boat(
        bot_t *bot,
        bool right_paddle,
        bool left_paddle
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x11);
    int8_t b = right_paddle ? 1 : 0;
    buf = push(buf, &b, sizeof(b));
    b = left_paddle ? 1 : 0;
    buf = push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_abilities(
        bot_t *bot,
        int8_t flags,
        float flying_speed,
        float walking_speed
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x12);
    buf = push(buf, &flags, sizeof(flags));
    buf = push(buf, &flying_speed, sizeof(flying_speed));
    buf = push(buf, &walking_speed, sizeof(walking_speed));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_digging(
        bot_t *bot,
        vint32_t status,
        position_t location,
        int8_t face
)
{
    location = htonll(location);
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x13);
    buf = _push_vint32(buf, status);
    buf = push(buf, &location, sizeof(location));
    buf = push(buf, &face, sizeof(face));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_entity_action(
        bot_t *bot,
        vint32_t entity_id,
        vint32_t action_id,
        vint32_t jump_boost
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x14);
    buf = _push_vint32(buf, entity_id);
    buf = _push_vint32(buf, action_id);
    buf = _push_vint32(buf, jump_boost);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_steer_vehicle(
        bot_t *bot,
        float sideways,
        float forward,
        uint8_t flags
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x15);
    buf = push(buf, &sideways, sizeof(sideways));
    buf = push(buf, &forward, sizeof(forward));
    buf = push(buf, &flags, sizeof(flags));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_resource_pack_status(
        bot_t *bot,
        char *hash,
        vint32_t result
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x16);
    buf = _push_string(buf, hash);
    buf = _push_vint32(buf, result);
    
    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_held_item_change(
        bot_t *bot,
        int16_t slot
)
{
    slot = htons(slot);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x17);
    buf = push(buf, &slot, sizeof(slot));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_creative_inventory_action(
        bot_t *bot,
        int16_t slot,
        slot_t *clicked_item
)
{
    slot = htons(slot);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x18);
    buf = push(buf, &slot, sizeof(slot));
    buf = _push_slot(buf, clicked_item);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_update_sign(
        bot_t *bot,
        position_t location,
        char *line1,
        char *line2,
        char *line3,
        char *line4
)
{
    location = htonll(location);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x19);
    buf = push(buf, &location, sizeof(location));
    buf = _push_string(buf, line1);
    buf = _push_string(buf, line2);
    buf = _push_string(buf, line3);
    buf = _push_string(buf, line4);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_animation(
        bot_t *bot,
        vint32_t hand
        )
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x1A);
    buf = _push_vint32(buf, hand);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_spectate(
        bot_t *bot,
        char *uuid
        )
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x1B);
    buf = push(buf, uuid, 16);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_block_placement(
        bot_t *bot,
        position_t location,
        vint32_t face,
        vint32_t hand,
        uint8_t x,
        uint8_t y,
        uint8_t z
        )
{
    location = htonll(location);
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
 
    buf = _push_vint32(buf, 0x1C);
    buf = push(buf, &location, sizeof(location));
    buf = _push_vint32(buf, face);
    buf = _push_vint32(buf, hand);
    buf = push(buf, &x, sizeof(x));
    buf = push(buf, &y, sizeof(y));
    buf = push(buf, &z, sizeof(z));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_use_item(
        bot_t *bot,
        vint32_t hand
        )
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
 
    buf = _push_vint32(buf, 0x1D);
    buf = _push_vint32(buf, hand);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}


/*
 * Login clientbound structs
 */

login_clientbound_disconnect_t* recv_login_clientbound_disconnect(bot_t *bot)
{
	login_clientbound_disconnect_t *p;
	p = calloc(1, sizeof(login_clientbound_disconnect_t));
	p->format = "vs";
	p->packet_id = 0;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

login_clientbound_success_t* recv_login_clientbound_success(bot_t *bot)
{
	login_clientbound_success_t *p;
	p = calloc(1, sizeof(login_clientbound_success_t));
	p->format = "vss";
	p->packet_id = 2;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

login_clientbound_set_compression_t* recv_login_clientbound_set_compression(bot_t *bot)
{
	login_clientbound_set_compression_t *p;
	p = calloc(1, sizeof(login_clientbound_set_compression_t));
	p->format = "vv";
	p->packet_id = 3;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

/*
 * Status clientbound structs
 */

status_clientbound_response_t* recv_status_clientbound_response(bot_t *bot)
{
	status_clientbound_response_t *p;
	p = calloc(1, sizeof(status_clientbound_response_t));
	p->format = "vs";
	p->packet_id = 0;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

status_clientbound_ping_t* recv_status_clientbound_ping(bot_t *bot)
{
	status_clientbound_ping_t *p;
	p = calloc(1, sizeof(status_clientbound_ping_t));
	p->format = "vl";
	p->packet_id = 1;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

/*
 * Play clientbound structs
 */

play_clientbound_keepalive_t* recv_play_clientbound_keepalive(bot_t *bot)
{
	play_clientbound_keepalive_t *p;
	p = calloc(1, sizeof(play_clientbound_keepalive_t));
	p->format = "vv";
	p->packet_id = 0;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_join_game_t* recv_play_clientbound_join_game(bot_t *bot)
{
	play_clientbound_join_game_t *p;
	p = calloc(1, sizeof(play_clientbound_join_game_t));
	p->format = "vwbbbbs";
	p->packet_id = 1;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_chat_t* recv_play_clientbound_chat(bot_t *bot)
{
	play_clientbound_chat_t *p;
	p = calloc(1, sizeof(play_clientbound_chat_t));
	p->format = "vsb";
	p->packet_id = 2;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_time_update_t* recv_play_clientbound_time_update(bot_t *bot)
{
	play_clientbound_time_update_t *p;
	p = calloc(1, sizeof(play_clientbound_time_update_t));
	p->format = "vll";
	p->packet_id = 3;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_equipment_t* recv_play_clientbound_entity_equipment(bot_t *bot)
{
	play_clientbound_entity_equipment_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_equipment_t));
	p->format = "vvhw";
	p->packet_id = 4;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_spawn_position_t* recv_play_clientbound_spawn_position(bot_t *bot)
{
	play_clientbound_spawn_position_t *p;
	p = calloc(1, sizeof(play_clientbound_spawn_position_t));
	p->format = "vl";
	p->packet_id = 5;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_update_health_t* recv_play_clientbound_update_health(bot_t *bot)
{
	play_clientbound_update_health_t *p;
	p = calloc(1, sizeof(play_clientbound_update_health_t));
	p->format = "vwvw";
	p->packet_id = 6;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_respawn_t* recv_play_clientbound_respawn(bot_t *bot)
{
	play_clientbound_respawn_t *p;
	p = calloc(1, sizeof(play_clientbound_respawn_t));
	p->format = "vwbbs";
	p->packet_id = 7;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_position_t* recv_play_clientbound_position(bot_t *bot)
{
	play_clientbound_position_t *p;
	p = calloc(1, sizeof(play_clientbound_position_t));
	p->format = "vlllwwb";
	p->packet_id = 8;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_item_change_t* recv_play_clientbound_item_change(bot_t *bot)
{
	play_clientbound_item_change_t *p;
	p = calloc(1, sizeof(play_clientbound_item_change_t));
	p->format = "vb";
	p->packet_id = 9;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_use_bed_t* recv_play_clientbound_use_bed(bot_t *bot)
{
	play_clientbound_use_bed_t *p;
	p = calloc(1, sizeof(play_clientbound_use_bed_t));
	p->format = "vvl";
	p->packet_id = 10;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_animation_t* recv_play_clientbound_animation(bot_t *bot)
{
	play_clientbound_animation_t *p;
	p = calloc(1, sizeof(play_clientbound_animation_t));
	p->format = "vvb";
	p->packet_id = 11;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_spawn_player_t* recv_play_clientbound_spawn_player(bot_t *bot)
{
	play_clientbound_spawn_player_t *p;
	p = calloc(1, sizeof(play_clientbound_spawn_player_t));
	p->format = "vvqwwwbbhw";
	p->packet_id = 12;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_collect_t* recv_play_clientbound_collect(bot_t *bot)
{
	play_clientbound_collect_t *p;
	p = calloc(1, sizeof(play_clientbound_collect_t));
	p->format = "vvv";
	p->packet_id = 13;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_spawn_object_t* recv_play_clientbound_spawn_object(bot_t *bot)
{
	play_clientbound_spawn_object_t *p;
	p = calloc(1, sizeof(play_clientbound_spawn_object_t));
	p->format = "vvbwwwbbw";
	p->packet_id = 14;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_spawn_mob_t* recv_play_clientbound_spawn_mob(bot_t *bot)
{
	play_clientbound_spawn_mob_t *p;
	p = calloc(1, sizeof(play_clientbound_spawn_mob_t));
	p->format = "vvbwwwbbbhhhw";
	p->packet_id = 15;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_spawn_painting_t* recv_play_clientbound_spawn_painting(bot_t *bot)
{
	play_clientbound_spawn_painting_t *p;
	p = calloc(1, sizeof(play_clientbound_spawn_painting_t));
	p->format = "vvslb";
	p->packet_id = 16;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_spawn_xp_t* recv_play_clientbound_spawn_xp(bot_t *bot)
{
	play_clientbound_spawn_xp_t *p;
	p = calloc(1, sizeof(play_clientbound_spawn_xp_t));
	p->format = "vvwwwh";
	p->packet_id = 17;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_velocity_t* recv_play_clientbound_entity_velocity(bot_t *bot)
{
	play_clientbound_entity_velocity_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_velocity_t));
	p->format = "vvhhh";
	p->packet_id = 18;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_destroy_entities_t* recv_play_clientbound_entity_destroy_entities(bot_t *bot)
{
	play_clientbound_entity_destroy_entities_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_destroy_entities_t));
	p->format = "vv*v";
	p->packet_id = 19;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_t* recv_play_clientbound_entity(bot_t *bot)
{
	play_clientbound_entity_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_t));
	p->format = "vv";
	p->packet_id = 20;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_move_t* recv_play_clientbound_entity_move(bot_t *bot)
{
	play_clientbound_entity_move_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_move_t));
	p->format = "vvbbbb";
	p->packet_id = 21;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_look_t* recv_play_clientbound_entity_look(bot_t *bot)
{
	play_clientbound_entity_look_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_look_t));
	p->format = "vvbbbb";
	p->packet_id = 22;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_look_move_t* recv_play_clientbound_entity_look_move(bot_t *bot)
{
	play_clientbound_entity_look_move_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_look_move_t));
	p->format = "vvbbbbbb";
	p->packet_id = 23;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_teleport_t* recv_play_clientbound_entity_teleport(bot_t *bot)
{
	play_clientbound_entity_teleport_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_teleport_t));
	p->format = "vvwwwbbb";
	p->packet_id = 24;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_head_look_t* recv_play_clientbound_entity_head_look(bot_t *bot)
{
	play_clientbound_entity_head_look_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_head_look_t));
	p->format = "vvb";
	p->packet_id = 25;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_status_t* recv_play_clientbound_entity_status(bot_t *bot)
{
	play_clientbound_entity_status_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_status_t));
	p->format = "vwb";
	p->packet_id = 26;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_attach_t* recv_play_clientbound_entity_attach(bot_t *bot)
{
	play_clientbound_entity_attach_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_attach_t));
	p->format = "vwwb";
	p->packet_id = 27;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_effect_t* recv_play_clientbound_entity_effect(bot_t *bot)
{
	play_clientbound_entity_effect_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_effect_t));
	p->format = "vvbbvb";
	p->packet_id = 29;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_clear_effect_t* recv_play_clientbound_entity_clear_effect(bot_t *bot)
{
	play_clientbound_entity_clear_effect_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_clear_effect_t));
	p->format = "vvb";
	p->packet_id = 30;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_properties_t* recv_play_clientbound_entity_properties(bot_t *bot)
{
	play_clientbound_entity_properties_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_properties_t));
	p->format = "vvw*w";
	p->packet_id = 32;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_set_xp_t* recv_play_clientbound_set_xp(bot_t *bot)
{
	play_clientbound_set_xp_t *p;
	p = calloc(1, sizeof(play_clientbound_set_xp_t));
	p->format = "vwww";
	p->packet_id = 31;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_chunk_data_t* recv_play_clientbound_chunk_data(bot_t *bot)
{
	play_clientbound_chunk_data_t *p;
	p = calloc(1, sizeof(play_clientbound_chunk_data_t));
	p->format = "vwwbhv*b";
	p->packet_id = 33;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_multi_block_change_t* recv_play_clientbound_multi_block_change(bot_t *bot)
{
	play_clientbound_multi_block_change_t *p;
	p = calloc(1, sizeof(play_clientbound_multi_block_change_t));
	p->format = "vwwv*w";
	p->packet_id = 34;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_block_change_t* recv_play_clientbound_block_change(bot_t *bot)
{
	play_clientbound_block_change_t *p;
	p = calloc(1, sizeof(play_clientbound_block_change_t));
	p->format = "vlv";
	p->packet_id = 35;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_block_action_t* recv_play_clientbound_block_action(bot_t *bot)
{
	play_clientbound_block_action_t *p;
	p = calloc(1, sizeof(play_clientbound_block_action_t));
	p->format = "vlbbv";
	p->packet_id = 36;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_block_break_animation_t* recv_play_clientbound_block_break_animation(bot_t *bot)
{
	play_clientbound_block_break_animation_t *p;
	p = calloc(1, sizeof(play_clientbound_block_break_animation_t));
	p->format = "vvlb";
	p->packet_id = 37;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_chunk_bulk_t* recv_play_clientbound_chunk_bulk(bot_t *bot)
{
	play_clientbound_chunk_bulk_t *p;
	p = calloc(1, sizeof(play_clientbound_chunk_bulk_t));
	p->format = "vbvwwh*b";
	p->packet_id = 38;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_explosion_t* recv_play_clientbound_explosion(bot_t *bot)
{
	play_clientbound_explosion_t *p;
	p = calloc(1, sizeof(play_clientbound_explosion_t));
	p->format = "vwwwww*wwww";
	p->packet_id = 39;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_effect_t* recv_play_clientbound_effect(bot_t *bot)
{
	play_clientbound_effect_t *p;
	p = calloc(1, sizeof(play_clientbound_effect_t));
	p->format = "vwlwb";
	p->packet_id = 40;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_sound_effect_t* recv_play_clientbound_sound_effect(bot_t *bot)
{
	play_clientbound_sound_effect_t *p;
	p = calloc(1, sizeof(play_clientbound_sound_effect_t));
	p->format = "vswwwwb";
	p->packet_id = 41;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_entity_spawn_global_t* recv_play_clientbound_entity_spawn_global(bot_t *bot)
{
	play_clientbound_entity_spawn_global_t *p;
	p = calloc(1, sizeof(play_clientbound_entity_spawn_global_t));
	p->format = "vvbwww";
	p->packet_id = 44;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_update_sign_t* recv_play_clientbound_update_sign(bot_t *bot)
{
	play_clientbound_update_sign_t *p;
	p = calloc(1, sizeof(play_clientbound_update_sign_t));
	p->format = "vlssss";
	p->packet_id = 51;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_plugin_message_t* recv_play_clientbound_plugin_message(bot_t *bot)
{
	play_clientbound_plugin_message_t *p;
	p = calloc(1, sizeof(play_clientbound_plugin_message_t));
	p->format = "vs*b";
	p->packet_id = 63;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_plugin_disconnect_t* recv_play_clientbound_plugin_disconnect(bot_t *bot)
{
	play_clientbound_plugin_disconnect_t *p;
	p = calloc(1, sizeof(play_clientbound_plugin_disconnect_t));
	p->format = "vs";
	p->packet_id = 64;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_plugin_difficulty_t* recv_play_clientbound_plugin_difficulty(bot_t *bot)
{
	play_clientbound_plugin_difficulty_t *p;
	p = calloc(1, sizeof(play_clientbound_plugin_difficulty_t));
	p->format = "vb";
	p->packet_id = 65;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}

play_clientbound_set_compression_t* recv_play_clientbound_set_compression(bot_t *bot)
{
	play_clientbound_set_compression_t *p;
	p = calloc(1, sizeof(play_clientbound_set_compression_t));
	p->format = "vv";
	p->packet_id = 70;
	decode_packet(bot, bot->_data->buf, p);
	return p;
}


void *protocol_decode(bot_t *bot)
{
	int32_t pid = receive_packet(bot);
	if (pid < 0) {
		if (pid == -1) exit(123);
		return NULL;
	}
	void *recv_struct = NULL;
	switch (bot->_data->current_state) {
		case HANDSHAKE:
			switch (pid) {
			}
			break;
		case LOGIN:
			switch (pid) {
				case 0x00:
					recv_struct = recv_login_clientbound_disconnect(bot);
					break;
				case 0x02:
					recv_struct = recv_login_clientbound_success(bot);
					break;
				case 0x03:
					recv_struct = recv_login_clientbound_set_compression(bot);
					break;
			}
			break;
		case STATUS:
			switch (pid) {
				case 0x00:
					recv_struct = recv_status_clientbound_response(bot);
					break;
				case 0x01:
					recv_struct = recv_status_clientbound_ping(bot);
					break;
			}
			break;
		case PLAY:
			switch (pid) {
				case 0x00:
					recv_struct = recv_play_clientbound_keepalive(bot);
					break;
				case 0x01:
					recv_struct = recv_play_clientbound_join_game(bot);
					break;
				case 0x02:
					recv_struct = recv_play_clientbound_chat(bot);
					break;
				case 0x03:
					recv_struct = recv_play_clientbound_time_update(bot);
					break;
				case 0x04:
					recv_struct = recv_play_clientbound_entity_equipment(bot);
					break;
				case 0x05:
					recv_struct = recv_play_clientbound_spawn_position(bot);
					break;
				case 0x06:
					recv_struct = recv_play_clientbound_update_health(bot);
					break;
				case 0x07:
					recv_struct = recv_play_clientbound_respawn(bot);
					break;
				case 0x08:
					recv_struct = recv_play_clientbound_position(bot);
					break;
				case 0x09:
					recv_struct = recv_play_clientbound_item_change(bot);
					break;
				case 0x0A:
					recv_struct = recv_play_clientbound_use_bed(bot);
					break;
				case 0x0B:
					recv_struct = recv_play_clientbound_animation(bot);
					break;
				case 0x0C:
					recv_struct = recv_play_clientbound_spawn_player(bot);
					break;
				case 0x0D:
					recv_struct = recv_play_clientbound_collect(bot);
					break;
				case 0x0E:
					recv_struct = recv_play_clientbound_spawn_object(bot);
					break;
				case 0x0F:
					recv_struct = recv_play_clientbound_spawn_mob(bot);
					break;
				case 0x10:
					recv_struct = recv_play_clientbound_spawn_painting(bot);
					break;
				case 0x11:
					recv_struct = recv_play_clientbound_spawn_xp(bot);
					break;
				case 0x12:
					recv_struct = recv_play_clientbound_entity_velocity(bot);
					break;
				case 0x13:
					recv_struct = recv_play_clientbound_entity_destroy_entities(bot);
					break;
				case 0x14:
					recv_struct = recv_play_clientbound_entity(bot);
					break;
				case 0x15:
					recv_struct = recv_play_clientbound_entity_move(bot);
					break;
				case 0x16:
					recv_struct = recv_play_clientbound_entity_look(bot);
					break;
				case 0x17:
					recv_struct = recv_play_clientbound_entity_look_move(bot);
					break;
				case 0x18:
					recv_struct = recv_play_clientbound_entity_teleport(bot);
					break;
				case 0x19:
					recv_struct = recv_play_clientbound_entity_head_look(bot);
					break;
				case 0x1A:
					recv_struct = recv_play_clientbound_entity_status(bot);
					break;
				case 0x1B:
					recv_struct = recv_play_clientbound_entity_attach(bot);
					break;
				case 0x1D:
					recv_struct = recv_play_clientbound_entity_effect(bot);
					break;
				case 0x1E:
					recv_struct = recv_play_clientbound_entity_clear_effect(bot);
					break;
				case 0x20:
					recv_struct = recv_play_clientbound_entity_properties(bot);
					break;
				case 0x1F:
					recv_struct = recv_play_clientbound_set_xp(bot);
					break;
				case 0x21:
					recv_struct = recv_play_clientbound_chunk_data(bot);
					break;
				case 0x22:
					recv_struct = recv_play_clientbound_multi_block_change(bot);
					break;
				case 0x23:
					recv_struct = recv_play_clientbound_block_change(bot);
					break;
				case 0x24:
					recv_struct = recv_play_clientbound_block_action(bot);
					break;
				case 0x25:
					recv_struct = recv_play_clientbound_block_break_animation(bot);
					break;
				case 0x26:
					recv_struct = recv_play_clientbound_chunk_bulk(bot);
					break;
				case 0x27:
					recv_struct = recv_play_clientbound_explosion(bot);
					break;
				case 0x28:
					recv_struct = recv_play_clientbound_effect(bot);
					break;
				case 0x29:
					recv_struct = recv_play_clientbound_sound_effect(bot);
					break;
				case 0x2C:
					recv_struct = recv_play_clientbound_entity_spawn_global(bot);
					break;
				case 0x33:
					recv_struct = recv_play_clientbound_update_sign(bot);
					break;
				case 0x3F:
					recv_struct = recv_play_clientbound_plugin_message(bot);
					break;
				case 0x40:
					recv_struct = recv_play_clientbound_plugin_disconnect(bot);
					break;
				case 0x41:
					recv_struct = recv_play_clientbound_plugin_difficulty(bot);
					break;
				case 0x46:
					recv_struct = recv_play_clientbound_set_compression(bot);
					break;
			}
			break;
		default:
			break;
	}
	return recv_struct;
}
