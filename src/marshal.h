#pragma once

#include <stdint.h>
#include "bot.h"
#include "protocol.h"

#define expect_more(x) (x & 0x80)
// align pointer x to y
// y is assumed to be a power of 2
#define align(x, y) ((uint64_t)((x) + (y) - 1) & (uint64_t)(~(y) + 1))

int varint64(char *data, int64_t *value);
int varint32(char *data, int32_t *value);
int varint64_encode(int64_t value, char *data, int len);
int varint32_encode(int32_t value, char *data, int len);

uint64_t reverse(uint64_t number, int len);

int format_packet(bot_t *bot, void *packet_data, void *packet_raw);
int decode_packet(bot_t *bot, void *packet_raw, void *packet_data);
void free_packet(void *);
vint32_t peek_packet(bot_t *, void *);
size_t format_sizeof(char);
void *push(void *, void *, size_t);
uint64_t value_at(void *, size_t);
