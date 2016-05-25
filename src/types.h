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

typedef struct _slot_t {
    int16_t block_id;
    int8_t count;
    int16_t damage;
    nbt_node *tree;
} slot_t;


