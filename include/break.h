#include "constants.h"
#include <stdint.h>

#define MINECRAFT_TOOL_AXE          ((uint32_t)0x01)
#define MINECRAFT_TOOL_PICKAXE      ((uint32_t)0x02)
#define MINECRAFT_TOOL_SHEARS       ((uint32_t)0x04)
#define MINECRAFT_TOOL_SHOVEL       ((uint32_t)0x08)
#define MINECRAFT_TOOL_SWORD        ((uint32_t)0x10)
#define MINECRAFT_TOOL_OTHER        ((uint32_t)0x20)

double block_hardness(int block_data);
double block_break_time_hand(int block_data);
double block_break_time_shears(int block_data);
double block_break_time_sword(int block_data);
const char * block_name(int block_data);

struct block_break_data {
    const char *name;
    double hardness;
    double break_time_diamond;
    double break_time_gold;
    double break_time_hand;
    double break_time_iron;
    double break_time_shears;
    double break_time_stone;
    double break_time_sword;
    double break_time_wood;
    uint32_t best_tool;
};

