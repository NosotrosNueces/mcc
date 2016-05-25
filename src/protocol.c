#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "protocol.h"
#include "../cNBT/nbt.h"

#define PAD_LENGTH(x) (x = (char *)x + 5)

int varint64(char *data, int64_t *value)
{
    int64_t result = 0;
    int shifts = 0;
    do {
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    } while(expect_more(*data++));
    *value = result;
    return shifts;
}

// returns the number of bytes read from data
int varint32(char *data, int32_t *value)
{
    int32_t result = 0;
    int shifts = 0;
    do {
        result |= ((0x7F & *data) << (shifts * 7));
        shifts++;
    } while(expect_more(*data++));
    *value = result;
    return shifts;
}

int varint64_encode(int64_t value, char *data, int len)
{
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    do {
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value = (uint64_t)value >> 7;
        i++;
    } while(value);
    data[i - 1] &= mask;
    return i;
}

int varint32_encode(int32_t value, char *data, int len)
{
    memset(data, 0, len);
    char mask = 0x7F;
    int i = 0;
    do {
        if(i >= len)
            return -1;
        data[i] = (mask & value);
        data[i] |= 0X80;
        value = (uint32_t)value >> 7;
        i++;
    } while(value);
    data[i - 1] &= mask;
    return i;
}

void *_push(void *buffer, void *data, size_t size)
{
    memcpy(buffer, data, size);
    return (char *)buffer + size;
}

void *_push_vint32(void *buf, vint32_t val) {
	char varint[5];
	uint32_t len = varint32_encode(val, varint, sizeof(varint));
	buf = _push(buf, varint, len);
	return buf;
}

void *_push_string(void *buf, char *str) {
	uint32_t bytes_pushed, len;
	size_t s_len = strlen(str);
	char str_len[5];
	bytes_pushed = varint32_encode(s_len, str_len, sizeof(str_len));
	buf = _push(buf, str_len, bytes_pushed);
	buf = _push(buf, str, s_len);
	return buf; 
}

void *_push_slot(void *_packet_raw, slot_data *slot_data)
{
	int16_t block_id = slot_data->block_id;

	char *packet_raw = _packet_raw;
	if (-1 == block_id) { // Empty slot is 0xffff
		return _push(packet_raw, &block_id, sizeof(block_id));
	} else {
		int8_t count = slot_data->count;
		int16_t damage = slot_data->damage;
		nbt_node *tree = slot_data->tree;

		/* Convert little to big endian; flip the bytes. */
		block_id = htons(block_id);
		damage = htons(damage);

		/* copy block_id (2 bytes) */
        packet_raw = _push(packet_raw, &block_id, sizeof(block_id));

		/* copy count (1 byte) */
        packet_raw = _push(packet_raw, &count, sizeof(count));

		/* copy damage (2 bytes) */
        packet_raw = _push(packet_raw, &damage, sizeof(damage));

		if (tree) {
			/* convert nbt tree to binary nbt data */
			struct buffer nbt_data = nbt_dump_binary(tree);
            packet_raw = _push(packet_raw, nbt_data.data, nbt_data.len);
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
	_push(start, buf_len, offset);
	return start;
}

/*
 * Handshaking serverbound functions
 */

int32_t send_handshaking_serverbound_handshake(
		bot_agent*        bot,
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
	buf = _push(buf, &server_port, sizeof(server_port));
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
		bot_agent*        bot,
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
		bot_agent*        bot
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
		bot_agent*        bot,
		int64_t       time
		)
{
    time = htonll(time);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
	buf = _push_vint32(buf, 0x01);
	buf = _push(buf, &time, sizeof(time));
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
		bot_agent *bot,
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
		bot_agent *bot,
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
	buf = _push(buf, &b, sizeof(b));
	b = has_position ? 1 : 0;
	buf = _push(buf, &b, sizeof(b));
	if (has_position) {
		buf = _push(buf, &looked_at_block, sizeof(looked_at_block));
	}
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_chat_message(
		bot_agent *bot,
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
		bot_agent *bot,
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
		bot_agent *bot,
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
	buf = _push(buf, &view_distance, sizeof(view_distance));
	buf = _push_vint32(buf, chat_mode);
	int8_t b = chat_colors ? 1 : 0;
	buf = _push(buf, &b, sizeof(b));
	buf = _push(buf, &displayed_skin_parts, sizeof(displayed_skin_parts));
	buf = _push_vint32(buf, main_hand);
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_confirm_transaction(
		bot_agent *bot,
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
	buf = _push(buf, &window_id, sizeof(window_id));
	buf = _push(buf, &action_number, sizeof(action_number));
	int8_t b = accepted ? 1 : 0;
	buf = _push(buf, &b, sizeof(b));

	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_enchant_item(
		bot_agent *bot,
		int8_t window_id,
		int8_t enchantment
		)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x06);
	buf = _push(buf, &window_id, sizeof(window_id));
	buf = _push(buf, &enchantment, sizeof(enchantment));

	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_click_window(
		bot_agent *bot,
		uint8_t window_id,
		int16_t slot,
		int8_t button,
		int16_t action_number,
		vint32_t mode,
        slot_data *clicked_item
		)
{
    slot = htons(slot);
    action_number = htons(action_number);
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

    buf = _push_vint32(buf, 0x07);
    buf = _push(buf, &window_id, sizeof(window_id));
    buf = _push(buf, &slot, sizeof(slot));
    buf = _push(buf, &button, sizeof(button));
    buf = _push(buf, &action_number, sizeof(action_number));
    buf = _push_vint32(buf, mode);
    buf = _push_slot(buf, clicked_item);
    
	int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_close_window(
        bot_agent *bot,
        uint8_t window_id
)
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x08);
    buf = _push(buf, &window_id, sizeof(window_id));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_plugin_message(
        bot_agent *bot,
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
    buf = _push(buf, data, data_length); 

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_use_entity(
        bot_agent *bot,
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
            buf = _push(buf, &target_x, sizeof(target_x));
            buf = _push(buf, &target_y, sizeof(target_y));
            buf = _push(buf, &target_z, sizeof(target_z));
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
		bot_agent*        bot,
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
        bot_agent *bot,
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
    buf = _push(buf, &x, sizeof(x));
    buf = _push(buf, &y, sizeof(y));
    buf = _push(buf, &z, sizeof(z));
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_position_look(
        bot_agent *bot,
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
    buf = _push(buf, &x, sizeof(x));
    buf = _push(buf, &y, sizeof(y));
    buf = _push(buf, &z, sizeof(z));
    buf = _push(buf, &yaw, sizeof(yaw));
    buf = _push(buf, &pitch, sizeof(pitch));
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_look(
        bot_agent *bot,
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
    buf = _push(buf, &yaw, sizeof(yaw));
    buf = _push(buf, &pitch, sizeof(pitch));
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player(
        bot_agent *bot,
        bool on_ground
)
{
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x0F);
    int8_t b = on_ground ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_vehicle_move(
        bot_agent *bot,
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
    buf = _push(buf, &x, sizeof(x));
    buf = _push(buf, &y, sizeof(y));
    buf = _push(buf, &z, sizeof(z));
    buf = _push(buf, &yaw, sizeof(yaw));
    buf = _push(buf, &pitch, sizeof(pitch));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_steer_boat(
        bot_agent *bot,
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
    buf = _push(buf, &b, sizeof(b));
    b = left_paddle ? 1 : 0;
    buf = _push(buf, &b, sizeof(b));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_abilities(
        bot_agent *bot,
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
    buf = _push(buf, &flags, sizeof(flags));
    buf = _push(buf, &flying_speed, sizeof(flying_speed));
    buf = _push(buf, &walking_speed, sizeof(walking_speed));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_digging(
        bot_agent *bot,
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
    buf = _push(buf, &location, sizeof(location));
    buf = _push(buf, &face, sizeof(face));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_entity_action(
        bot_agent *bot,
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
        bot_agent *bot,
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
    buf = _push(buf, &sideways, sizeof(sideways));
    buf = _push(buf, &forward, sizeof(forward));
    buf = _push(buf, &flags, sizeof(flags));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_resource_pack_status(
        bot_agent *bot,
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
        bot_agent *bot,
        int16_t slot
)
{
    slot = htons(slot);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x17);
    buf = _push(buf, &slot, sizeof(slot));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_creative_inventory_action(
        bot_agent *bot,
        int16_t slot,
        slot_data *clicked_item
)
{
    slot = htons(slot);
	void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;

	buf = _push_vint32(buf, 0x18);
    buf = _push(buf, &slot, sizeof(slot));
    buf = _push_slot(buf, clicked_item);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_update_sign(
        bot_agent *bot,
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
    buf = _push(buf, &location, sizeof(location));
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
        bot_agent *bot,
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
        bot_agent *bot,
        char *uuid
        )
{
    void *tmp = malloc(bot->_data->packet_threshold);
	void *buf = tmp, *packet;
	PAD_LENGTH(buf);
	packet = buf;
    
    buf = _push_vint32(buf, 0x1B);
    buf = _push(buf, uuid, 16);

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_player_block_placement(
        bot_agent *bot,
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
    buf = _push(buf, &location, sizeof(location));
    buf = _push_vint32(buf, face);
    buf = _push_vint32(buf, hand);
    buf = _push(buf, &x, sizeof(x));
    buf = _push(buf, &y, sizeof(y));
    buf = _push(buf, &z, sizeof(z));

    int32_t length = (int32_t)(buf - packet);
	packet = _append_len(tmp, &length);
	send_raw(bot, packet, length);
	free(tmp);
	return length;
}

int32_t send_play_serverbound_use_item(
        bot_agent *bot,
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

