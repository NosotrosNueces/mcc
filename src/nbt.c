#include <assert.h>
#include <stdlib.h>
#include "nbt.h"
#include "serial.h"

char *_nbt_parse_tag_byte(char *data, struct nbt_tag *node, struct bot_agent *bot);
char *_nbt_parse_tag_short(char *data, struct nbt_tag *node, struct bot_agent *bot);
char *_nbt_parse_tag_int(char *data, struct nbt_tag *node, struct bot_agent *bot);
char *_nbt_parse_tag_long(char *data, struct nbt_tag *node, struct bot_agent *bot);
char *_nbt_parse_tag_float(char *data, struct nbt_tag *node, struct bot_agent *bot);
char *_nbt_parse_tag_double(char *data, struct nbt_tag *node, struct bot_agent *bot);
char *_nbt_parse_tag_byte_array(char *data, struct nbt_byte_array *byte_array, struct bot_agent *bot);
char *_nbt_parse_tag_string(char *data, struct nbt_tag *node, struct bot_agent *bot);
char *_nbt_parse_tag_list(char *data, struct nbt_list *node, struct bot_agent *bot);
char *_nbt_parse_tag_compound(char *data, struct nbt_compound **compound, struct bot_agent *bot);
char *_nbt_parse_tag_int_array(char *data, struct nbt_int_array *array, struct bot_agent *bot);

char *_nbt_read_string(char *data, struct nbt_string *str, struct bot_agent *bot) {
    /* length */
    data = _read_int16_t(data, &str->length, bot);
    /* allocate and read string */
    str->str = malloc(str->length + 1);
    data = _read(data, str->str, str->length, bot);
    str->str[str->length] = '\0';
    return data;
}

char *_nbt_parse_tag(char *data, struct nbt_tag *node, enum NBT_TAG_TYPE type, int named, struct bot_agent *bot) {
    node->type = type;
    /* read in the name */
    if (named && type != NBT_TAG_END_TYPE) {
        data = _nbt_read_string(data, &node->name, bot);
    } else {
        node->name.length = 0;
        node->name.str = NULL;
    }
    switch (type) {
        case NBT_TAG_END_TYPE:
            break;
        case NBT_TAG_BYTE_TYPE:
            data = _read(data, &node->tag_byte, sizeof(node->tag_byte), bot);
            break;
        case NBT_TAG_SHORT_TYPE:
            data = _read_int16_t(data, &node->tag_short, bot);
            break;
        case NBT_TAG_INT_TYPE:
            data = _read_int32_t(data, &node->tag_int, bot);
            break;
        case NBT_TAG_LONG_TYPE:
            data = _read_int64_t(data, &node->tag_long, bot);
            break;
        case NBT_TAG_FLOAT_TYPE:
            data = _read_float(data, &node->tag_float, bot);
            break;
        case NBT_TAG_DOUBLE_TYPE:
            data = _read_double(data, &node->tag_double, bot);
            break;
        case NBT_TAG_BYTE_ARRAY_TYPE:
            data = _nbt_parse_tag_byte_array(data, &node->tag_byte_array, bot);
            break;
        case NBT_TAG_STRING_TYPE:
            data = _nbt_read_string(data, &node->tag_string, bot);
            break;
        case NBT_TAG_LIST_TYPE:
            data = _nbt_parse_tag_list(data, &node->tag_list, bot);
            break;
        case NBT_TAG_COMPOUND_TYPE:
            data = _nbt_parse_tag_compound(data, &node->tag_compound, bot);
            break;
        case NBT_TAG_INT_ARRAY:
            data = _nbt_parse_tag_int_array(data, &node->tag_int_array, bot);
            break;
    }
    return data;
}



char *_nbt_parse_tag_byte_array(char *data, struct nbt_byte_array *byte_array, struct bot_agent *bot) {
    data = _read_int32_t(data, &byte_array->length, bot);
    byte_array->data = malloc(byte_array->length);
    data = _read(data, byte_array->data, byte_array->length, bot);
    return data;
}

char *_nbt_parse_tag_list(char *data, struct nbt_list *list, struct bot_agent *bot) {
    int8_t type;
    data = _read(data, &type, sizeof(type), bot);
    list->type = type;
    data = _read_int32_t(data, &list->length, bot);
    list->elements = malloc(sizeof(struct nbt_tag) * list->length);
    for (int32_t i = 0; i < list->length; i++) {
        struct nbt_tag *node = &list->elements[i];
        data = _nbt_parse_tag(data, node, type, 0, bot);    
    }
    return data;
}

char *_nbt_parse_tag_compound(char *data, struct nbt_compound **compound, struct bot_agent *bot) {
    int8_t type;
    data = _read(data, &type, sizeof(type), bot);
    struct nbt_compound tmp;
    struct nbt_compound *current = &tmp;
    for (; type != NBT_TAG_END_TYPE; data = _read(data, &type, sizeof(type), bot)) {
        current->next = malloc(sizeof(struct nbt_compound)); 
        current = current->next;
        data = _nbt_parse_tag(data, &current->payload, type, 1, bot);
    }
    current->next = NULL;
    *compound = tmp.next;
    return data;
}

char *_nbt_parse_tag_int_array(char *data, struct nbt_int_array *array, struct bot_agent *bot) {
    data = _read_int32_t(data, &array->length, bot);
    array->data = malloc(array->length * sizeof(int32_t));
    for (int i = 0; i < array->length; i++) {
        data = _read_int32_t(data, &array->data[i], bot);
    }
    return data;
}

char *nbt_parse(char *data, struct nbt_tag *node, struct bot_agent *bot) {
    int8_t type;
    char *ptr = _read(data, &type, sizeof(type), bot);
    ptr = _nbt_parse_tag(ptr, node, type, 1, bot);
    return ptr;
}

void free_nbt_compound(struct nbt_compound *compound) {
    if (compound == NULL) {
        return;
    }
    free_nbt_compound(compound->next);
    free_nbt(&compound->payload);
    free(compound);
}

void free_nbt(struct nbt_tag *nbt) {
    /* free the string */     
    if (nbt->name.str != NULL) {
        free(nbt->name.str);
    }
    switch(nbt->type) {
        case NBT_TAG_END_TYPE:
            break;
        case NBT_TAG_BYTE_TYPE:
            break;
        case NBT_TAG_SHORT_TYPE:
            break;
        case NBT_TAG_INT_TYPE:
            break;
        case NBT_TAG_LONG_TYPE:
            break;
        case NBT_TAG_FLOAT_TYPE:
            break;
        case NBT_TAG_DOUBLE_TYPE:
            break;
        case NBT_TAG_BYTE_ARRAY_TYPE:
            free(nbt->tag_byte_array.data);
            break;
        case NBT_TAG_STRING_TYPE:
            free(nbt->tag_string.str);
            break;
        case NBT_TAG_LIST_TYPE:
        {
            struct nbt_list *tag_list = &nbt->tag_list;
            for (int i = 0; i < nbt->tag_list.length; i++) {
                free_nbt(&tag_list->elements[i]);
            }
            free(tag_list->elements);
            break;
        }
        case NBT_TAG_COMPOUND_TYPE:
        {
            struct nbt_compound *tag_compound = nbt->tag_compound;
            free_nbt_compound(tag_compound);
            break;
        }
        case NBT_TAG_INT_ARRAY:
            free(nbt->tag_int_array.data);
            break;
    }
}
