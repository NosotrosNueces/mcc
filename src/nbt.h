#pragma once

#include <stdint.h>
#include "types.h"

enum NBT_TAG_TYPE {
    NBT_TAG_END_TYPE,
    NBT_TAG_BYTE_TYPE,
    NBT_TAG_SHORT_TYPE,
    NBT_TAG_INT_TYPE,
    NBT_TAG_LONG_TYPE,
    NBT_TAG_FLOAT_TYPE,
    NBT_TAG_DOUBLE_TYPE,
    NBT_TAG_BYTE_ARRAY_TYPE,
    NBT_TAG_STRING_TYPE,
    NBT_TAG_LIST_TYPE,
    NBT_TAG_COMPOUND_TYPE,
    NBT_TAG_INT_ARRAY
};

struct nbt_byte_array {
    int32_t length;
    int8_t *data;
};

struct nbt_string {
    int16_t length;
    char *str;
};

struct nbt_list {
    enum NBT_TAG_TYPE type;
    int32_t length;
    struct nbt_tag *elements;
};

struct nbt_int_array {
    int32_t length;
    int32_t *data;
};

struct nbt_tag {
    enum NBT_TAG_TYPE type;
    struct nbt_string name;
    union {
        int8_t tag_byte;
        int16_t tag_short;
        int32_t tag_int;
        int64_t tag_long;
        float tag_float;
        double tag_double;
        struct nbt_byte_array tag_byte_array;
        struct nbt_string tag_string;
        struct nbt_list tag_list;
        struct nbt_compound *tag_compound;
        struct nbt_int_array tag_int_array;
    };
};

struct nbt_compound {
    struct nbt_tag payload;
    struct nbt_compound *next;
};

void nbt_print(struct nbt_tag *n);
struct nbt_tag *nbt_parse(char *data, uint32_t *bytes_read, struct bot_agent *bot);
