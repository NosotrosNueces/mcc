#ifndef MARSHAL_H
#define MARSHAL_H

int varint64(char *data, int64_t *value);
int varint32(char *data, int32_t *value);
int varint64_encode(uint64_t value, char *data, int len);
int varint32_encode(uint32_t value, char *data, int len);

void reverse(void *number, int len);

int format_packet(bot_t *bot, void *packet_data, void **packet_raw_ptr);
int decode_packet(void *packet_raw, void *packet_data);

#endif /* MARSHAL_H */