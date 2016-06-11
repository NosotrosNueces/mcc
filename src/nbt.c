#include <stdlib.h>
#include "nbt.h"
#include "serial.h"
#include <assert.h>

char *_nbt_parse_tag_byte(char *data, struct nbt_tag *node);
char *_nbt_parse_tag_short(char *data, struct nbt_tag *node);
char *_nbt_parse_tag_int(char *data, struct nbt_tag *node);
char *_nbt_parse_tag_long(char *data, struct nbt_tag *node);
char *_nbt_parse_tag_float(char *data, struct nbt_tag *node);
char *_nbt_parse_tag_double(char *data, struct nbt_tag *node);
char *_nbt_parse_tag_byte_array(char *data, struct nbt_byte_array *byte_array);
char *_nbt_parse_tag_string(char *data, struct nbt_tag *node);
char *_nbt_parse_tag_list(char *data, struct nbt_list *node);
char *_nbt_parse_tag_compound(char *data, struct nbt_compound **compound);
char *_nbt_parse_tag_int_array(char *data, struct nbt_int_array *array);

char *_nbt_read_string(char *data, struct nbt_string *str) {
    /* length */
    data = _read_int16_t(data, &str->length);
    /* allocate and read string */
    str->str = malloc(str->length + 1);
    data = _read(data, str->str, str->length);
    str->str[str->length] = '\0';
    return data;
}

char *_nbt_parse_tag(char *data, struct nbt_tag *node, enum NBT_TAG_TYPE type, int named) {
    node->type = type;
    /* read in the name */
    if (named && type != NBT_TAG_END_TYPE) {
        data = _nbt_read_string(data, &node->name);
    } else {
        node->name.length = 0;
        node->name.str = NULL;
    }
    switch (type) {
        case NBT_TAG_END_TYPE:
            break;
        case NBT_TAG_BYTE_TYPE:
            data = _read(data, &node->tag_byte, sizeof(node->tag_byte));
            break;
        case NBT_TAG_SHORT_TYPE:
            data = _read_int16_t(data, &node->tag_short);
            break;
        case NBT_TAG_INT_TYPE:
            data = _read_int32_t(data, &node->tag_int);
            break;
        case NBT_TAG_LONG_TYPE:
            data = _read_int64_t(data, &node->tag_long);
            break;
        case NBT_TAG_FLOAT_TYPE:
            data = _read_float(data, &node->tag_float);
            break;
        case NBT_TAG_DOUBLE_TYPE:
            data = _read_double(data, &node->tag_double);
            break;
        case NBT_TAG_BYTE_ARRAY_TYPE:
            data = _nbt_parse_tag_byte_array(data, &node->tag_byte_array);
            break;
        case NBT_TAG_STRING_TYPE:
            data = _nbt_read_string(data, &node->tag_string);
            break;
        case NBT_TAG_LIST_TYPE:
            data = _nbt_parse_tag_list(data, &node->tag_list);
            break;
        case NBT_TAG_COMPOUND_TYPE:
            data = _nbt_parse_tag_compound(data, &node->tag_compound);
            break;
        case NBT_TAG_INT_ARRAY:
            data = _nbt_parse_tag_int_array(data, &node->tag_int_array);
            break;
    }
    return data;
}



char *_nbt_parse_tag_byte_array(char *data, struct nbt_byte_array *byte_array) {
    data = _read_int32_t(data, &byte_array->length);
    byte_array->data = malloc(byte_array->length);
    data = _read(data, byte_array->data, byte_array->length);
    return data;
}

char *_nbt_parse_tag_list(char *data, struct nbt_list *list) {
    int8_t type;
    data = _read(data, &type, sizeof(type));
    list->type = type;
    data = _read_int32_t(data, &list->length);
    list->elements = malloc(sizeof(struct nbt_tag) * list->length);
    for (int32_t i = 0; i < list->length; i++) {
        struct nbt_tag *node = &list->elements[i];
        data = _nbt_parse_tag(data, node, type, 0);    
    }
    return data;
}

char *_nbt_parse_tag_compound(char *data, struct nbt_compound **compound) {
    int8_t type;
    data = _read(data, &type, sizeof(type));
    struct nbt_compound tmp;
    struct nbt_compound *current = &tmp;
    for (; type != NBT_TAG_END_TYPE; data = _read(data, &type, sizeof(type))) {
        current->next = malloc(sizeof(struct nbt_compound)); 
        current = current->next;
        data = _nbt_parse_tag(data, &current->payload, type, 1);
    }
    *compound = tmp.next;
    return data;
}

char *_nbt_parse_tag_int_array(char *data, struct nbt_int_array *array) {
    data = _read_int32_t(data, &array->length);
    array->data = malloc(array->length * sizeof(int32_t));
    for (int i = 0; i < array->length; i++) {
        data = _read_int32_t(data, &array->data[i]);
    }
    return data;
}

struct nbt_tag *nbt_parse(char *data, uint32_t *bytes_read) {
    struct nbt_tag *node = malloc(sizeof(struct nbt_tag));
    int8_t type;
    char *ptr = _read(data, &type, sizeof(type));
    ptr = _nbt_parse_tag(ptr, node, type, 1);
    *bytes_read = ptr - data;
    return node;
}
