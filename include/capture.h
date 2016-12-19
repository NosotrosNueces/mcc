#pragma once

#include "types.h"

enum packet_direction {
    PACKET_DIRECTION_CLIENTBOUND,
    PACKET_DIRECTION_SERVERBOUND
};

void init_capture(struct bot_agent *bot, const char *file);

void capture_packet(struct bot_agent *bot, enum packet_direction direction, void *payload, size_t payload_len);

void end_capture(struct bot_agent *bot);
