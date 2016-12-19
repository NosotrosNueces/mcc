#include <stdio.h>
#include "nbt.h"

void _nbt_print(struct nbt_tag *n, int indent_level, int named);

void indent(int indent_level) {
    for (int i = 0; i < indent_level; i++) {
        putchar(' ');
        putchar(' ');
        putchar(' ');
        putchar(' ');
    }
}

void _nbt_print_tag_byte(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Byte('%s'): %d\n", named ? n->name.str : "None", n->tag_byte);
}

void _nbt_print_tag_short(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Short('%s'): %d\n", named ? n->name.str : "None", n->tag_short);
}

void _nbt_print_tag_int(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Int('%s'): %d\n", named ? n->name.str : "None", n->tag_int);
}

void _nbt_print_tag_long(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Long('%s'): %ld\n", named ? n->name.str : "None", n->tag_long);
}

void _nbt_print_tag_float(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Float('%s'): %f\n", named ? n->name.str : "None", n->tag_float);
}

void _nbt_print_tag_double(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Double('%s'): %f\n", named ? n->name.str : "None", n->tag_double);
}

void _nbt_print_tag_byte_array(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Byte_Array('%s'): [%d bytes]\n", named ? n->name.str : "None", n->tag_byte_array.length);
}

void _nbt_print_tag_string(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_String('%s'): '%s'\n", named ? n->name.str : "None", n->tag_string.str);
}

void _nbt_print_tag_list(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_List('%s'): %d entries\n", named ? n->name.str : "None", n->tag_list.length);
    indent(indent_level);
    printf("{\n");
    for (int i = 0; i < n->tag_list.length; i++) {
        _nbt_print(&n->tag_list.elements[i], indent_level + 1, 0);
    }
    indent(indent_level);
    printf("}\n");
}

void _nbt_print_tag_compound(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Compound('%s')\n", named ? n->name.str : "None");
    indent(indent_level);
    printf("{\n");
    struct nbt_compound *current = n->tag_compound;
    while (current != NULL) {
        _nbt_print(&current->payload, indent_level + 1, 1);
        current = current->next;
    }
    indent(indent_level);
    printf("}\n");
}

void _nbt_print_tag_int_array(struct nbt_tag *n, int indent_level, int named) {
    indent(indent_level);
    printf("TAG_Int_Array('%s'): [%d ints]\n", named ? n->name.str : "None", n->tag_int_array.length);
}

void _nbt_print(struct nbt_tag *n, int indent_level, int named) {
    switch(n->type) {
        case NBT_TAG_END_TYPE:
            /* someone dun fucked up */
            break;
        case NBT_TAG_BYTE_TYPE:
            _nbt_print_tag_byte(n, indent_level, named);
            break;
        case NBT_TAG_SHORT_TYPE:
            _nbt_print_tag_short(n, indent_level, named);
            break;
        case NBT_TAG_INT_TYPE:
            _nbt_print_tag_int(n, indent_level, named);
            break;
        case NBT_TAG_LONG_TYPE:
            _nbt_print_tag_long(n, indent_level, named);
            break;
        case NBT_TAG_FLOAT_TYPE:
            _nbt_print_tag_float(n, indent_level, named);
            break;
        case NBT_TAG_DOUBLE_TYPE:
            _nbt_print_tag_double(n, indent_level, named);
            break;
        case NBT_TAG_BYTE_ARRAY_TYPE:
            _nbt_print_tag_byte_array(n, indent_level, named);
            break;
        case NBT_TAG_STRING_TYPE:
            _nbt_print_tag_string(n, indent_level, named);
            break;
        case NBT_TAG_LIST_TYPE:
            _nbt_print_tag_list(n, indent_level, named);
            break;
        case NBT_TAG_COMPOUND_TYPE:
            _nbt_print_tag_compound(n, indent_level, named);
            break;
        case NBT_TAG_INT_ARRAY:
            _nbt_print_tag_int_array(n, indent_level, named);
            break;
    }
}

void nbt_print(struct nbt_tag *n) {
    _nbt_print(n, 0, 1);
}
