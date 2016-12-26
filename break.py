#!/usr/bin/env python

import sys, json
from collections import defaultdict

BREAKING_TIME_MULTIPLIER = 5

def c_float_to_string(f):
    if f == float('inf'):
        return 'INFINITY'
    return str(f)

def generate_headers():
    code = ''
    code += '#include <math.h>\n'
    code += '#include <stdio.h>\n'
    code += '#include "break.h"\n\n'
    return code

def generate_block_break_function(func_type, func_name, field, break_json):
    code = ''
    code +=    '{:s} {:s}(int block_data) {{\n'.format(func_type, func_name) + \
               '    int index = block_data >> 4;\n' + \
               '    return break_data_table[index].{:s};\n'.format(field) + \
               '}\n'
    return code

def generate_block_break_table(break_json):
    code = ''
    code +=                   'static const struct block_break_data break_data_table[] = {\n'

    # Generate lookup table of break data
    for i in range(256):
        if str(i) in break_json:
            data = break_json[str(i)]
            print(str(i))
            code +=    '    {\n' + \
                       '        .name               = "{:s}",\n'.format(data['name']) + \
                       '        .hardness           = {:s},\n'.format(c_float_to_string(data['hardness'])) + \
                       '        .break_time_diamond = {:s},\n'.format(c_float_to_string(data['break_time_diamond'])) + \
                       '        .break_time_gold    = {:s},\n'.format(c_float_to_string(data['break_time_gold'])) + \
                       '        .break_time_hand    = {:s},\n'.format(c_float_to_string(data['break_time_hand'])) + \
                       '        .break_time_iron    = {:s},\n'.format(c_float_to_string(data['break_time_iron'])) + \
                       '        .break_time_shears  = {:s},\n'.format(c_float_to_string(data['break_time_shears'])) + \
                       '        .break_time_stone   = {:s},\n'.format(c_float_to_string(data['break_time_stone'])) + \
                       '        .break_time_sword   = {:s},\n'.format(c_float_to_string(data['break_time_sword'])) + \
                       '        .break_time_wood    = {:s},\n'.format(c_float_to_string(data['break_time_wood']))
            # Compute bitmask
            code +=    '        .best_tool          = {:s},\n'.format(' | '.join(data['best_tool'])) + \
                       '    },\n'
        else:
            code +=    '    (struct block_break_data){0},\n'
    code +=    '};\n'
    return code;


def generate_break_c(break_json):
    code = generate_headers() + \
        generate_block_break_table(break_json) + '\n' + \
        generate_block_break_function('const char', '*block_name', 'name', break_json) + '\n' + \
        generate_block_break_function('double', 'block_hardness', 'hardness', break_json) + '\n' + \
        generate_block_break_function('double', 'block_break_time_hand', 'hardness', break_json)
    with open('src/break.c', 'w') as f:
        f.write(code)

def generate_break_h(break_json):
    code = ''
    code +=    '#include "constants.h"\n' + \
               '#include <stdint.h>\n' + \
               '\n' + \
               '#define MINECRAFT_TOOL_AXE          ((uint32_t)0x01)\n' + \
               '#define MINECRAFT_TOOL_PICKAXE      ((uint32_t)0x02)\n' + \
               '#define MINECRAFT_TOOL_SHEARS       ((uint32_t)0x04)\n' + \
               '#define MINECRAFT_TOOL_SHOVEL       ((uint32_t)0x08)\n' + \
               '#define MINECRAFT_TOOL_SWORD        ((uint32_t)0x10)\n' + \
               '#define MINECRAFT_TOOL_OTHER        ((uint32_t)0x20)\n' + \
               '\n' + \
               'double block_hardness(int block_data);\n' + \
               'double block_break_time_hand(int block_data);\n' + \
               'double block_break_time_shears(int block_data);\n' + \
               'double block_break_time_sword(int block_data);\n' + \
               'const char * block_name(int block_data);\n' + \
               '\n' + \
               'struct block_break_data {\n' + \
               '    const char *name;\n' + \
               '    double hardness;\n' + \
               '    double break_time_diamond;\n' + \
               '    double break_time_gold;\n' + \
               '    double break_time_hand;\n' + \
               '    double break_time_iron;\n' + \
               '    double break_time_shears;\n' + \
               '    double break_time_stone;\n' + \
               '    double break_time_sword;\n' + \
               '    double break_time_wood;\n' + \
               '    uint32_t best_tool;\n' + \
               '};\n' + \
               '\n'

    with open('include/break.h', 'w') as f:
        f.write(code)

def main(argv):
    if len(argv) < 2:
        return 1
    with open(argv[1], 'r') as f:
        break_json = json.load(f)
    generate_break_h(break_json)
    generate_break_c(break_json)
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))
