#ifndef MARSHAL_H
#define MARSHAL_H

#include <stdint.h>
#include "bot.h"
#include "protocol.h"

int varint64(char *data, int64_t *value);
int varint32(char *data, int32_t *value);
int varint64_encode(uint64_t value, char *data, int len);
int varint32_encode(uint32_t value, char *data, int len);

void reverse(void *number, int len);

int format_packet(bot_t *bot, void *packet_data, void **packet_raw_ptr);
int decode_packet(bot_t *bot, void *packet_raw, void *packet_data);
void free_packet(void *);
vint32_t peek_packet(bot_t *, void *);

#endif /* MARSHAL_H */
