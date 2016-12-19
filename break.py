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

def generate_switch_case_block_function(break_json, name, key, default):
    code = ''
    d = defaultdict(lambda: [])
    for id, data in break_json.items():
        d[data[key]].append(data['name'])

    code += 'double ' + name + '(int block_data) {\n'
    code += '    enum MINECRAFT_BLOCK block_id = block_data >> 4;\n'
    code += '    switch(block_id) {\n'
    for val in sorted(d.keys(), reverse=True):
        blocks = d[val]
        for block in blocks:
            code += '        case ' + block + ':\n'
        code += '            return ' + c_float_to_string(val) + ';\n'
    code += '        default:\n'
    code += '            return ' + default + ';\n'
    code += '    }\n'
    code += '}\n\n'
    return code


def generate_hardness_function(break_json):
    return generate_switch_case_block_function(break_json, 'block_hardness', 'hardness', 'NAN')

def generate_break_time_hand_function(break_json):
    return generate_switch_case_block_function(break_json, 'block_break_time_hand', 'break_time_hand', 'NAN')

def generate_break_time_shears_function(break_json):
    return generate_switch_case_block_function(break_json, 'block_break_time_shears', 'break_time_shears', 'NAN')

def generate_break_time_sword_function(break_json):
    return generate_switch_case_block_function(break_json, 'block_break_time_sword', 'break_time_sword', 'NAN')

def generate_block_name_function(break_json):
    code = ''
    code += 'const char *block_name(int block_data) {\n'
    code += '    enum MINECRAFT_BLOCK block_id = block_data >> 4;\n'
    code += '    switch(block_id) {\n'
    for val in sorted(break_json.keys(), key=lambda x: int(x)):
        data = break_json[val]
        code += '        case ' + data['name'] + ':\n'
        code += '            return "' + data['name']+'";\n'
    code += '    }\n'
    code += '}\n'
    return code

def generate_break_c(break_json):
    code = generate_headers() +                                 \
            generate_hardness_function(break_json) +            \
            generate_break_time_hand_function(break_json) +     \
            generate_break_time_shears_function(break_json) +   \
            generate_break_time_sword_function(break_json) +    \
            generate_block_name_function(break_json)

    with open('src/break.c', 'w') as f:
        f.write(code)

def generate_break_h(break_json):
    code = ''
    code += '#include "constants.h"\n'
    code += '\n'
    code += 'double block_hardness(int block_data);\n'
    code += 'double block_break_time_hand(int block_data);\n'
    code += 'double block_break_time_shears(int block_data);\n'
    code += 'double block_break_time_sword(int block_data);\n'
    code += 'const char * block_name(int block_data);\n'

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
