#pragma once

#include <stdint.h>
#include "types.h"

void nbt_print(struct nbt_tag *n);
char *nbt_parse(char *data, struct nbt_tag *node, struct bot_agent *bot);
void free_nbt(struct nbt_tag *nbt);
