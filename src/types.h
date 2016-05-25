#pragma once

#include "../cNBT/nbt.h"

typedef uint64_t position_t;
typedef int32_t vint32_t;
typedef char* chat_t;
typedef int32_t data_t;
typedef void* metadata_t;
typedef void* property_t;
typedef void* record_t;
typedef void* slot_t;

struct slot_data {
    int16_t block_id;
    int8_t count;
    int16_t damage;
    nbt_node *tree;
};

struct bot_agent {
    uint32_t eid;
    // 0: survival, 1: creative, 2: adventure. Bit 3 (0x8) is the hardcore flag
    uint8_t gamemode;
    // -1: nether, 0: overworld, 1: end
    int8_t dimension;
    // 0 thru 3 for Peaceful, Easy, Normal, Hard
    uint8_t difficulty;
    uint8_t max_players;
    // String  default, flat, largeBiomes, amplified, default_1_1
    char * level_type;
    bool reduced_debug_info;

    double x;
    double y;
    double z;
    float yaw;
    float pitch;
    int8_t flags;

    uint8_t animation;

    int32_t health;
    int32_t food;
    float saturation;

    size_t packet_threshold;
    uv_tcp_t *socket;
    /* callbacks */ 
}
