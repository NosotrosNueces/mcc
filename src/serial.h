#pragma once
#include <stdint.h>
#include "types.h"


struct packet_write_buffer {
    uint32_t capacity;
    char *base;
    char *ptr;
};

int varint64(char *data, int32_t bytes_left, int64_t *value);
int varint32(char *data, int32_t bytes_left, int32_t *value);
int varint64_encode(int64_t value, char *data, int len);
int varint32_encode(int32_t value, char *data, int len);

#ifndef htonll
uint64_t htonll(uint64_t number);
#endif

#ifndef ntohll
uint64_t ntohll(uint64_t number);
#endif
char *_read(char *buffer, void *storage, size_t size, struct bot_agent *bot);
char *_read_int16_t(char *buffer, int16_t *val, struct bot_agent *bot);
char *_read_uint16_t(char *buffer, uint16_t *val, struct bot_agent *bot);
char *_read_int32_t(char *buffer, int32_t *val, struct bot_agent *bot);
char *_read_uint32_t(char *buffer, uint32_t *val, struct bot_agent *bot);
char *_read_int64_t(char *buffer, int64_t *val, struct bot_agent *bot);
char *_read_uint64_t(char *buffer, uint64_t *val, struct bot_agent *bot);
char *_read_float(char *buffer, float *val, struct bot_agent *bot);
char *_read_double(char *buffer, double *val, struct bot_agent *bot);
char *_read_vint32(char *buf, vint32_t *val, struct bot_agent *bot);
char *_read_vint64(char *buf, vint64_t *val, struct bot_agent *bot);
char *_read_string(char *buf, char **strptr, int32_t *str_len, struct bot_agent *bot);
char *_read_slot(char *packet_raw, struct slot_type *slot_data, struct bot_agent *bot);
void _push(struct packet_write_buffer *buffer, void *data, size_t size);
void _push_int16_t(struct packet_write_buffer *buffer, int16_t val);
void _push_uint16_t(struct packet_write_buffer *buffer, uint16_t val);
void _push_int32_t(struct packet_write_buffer *buffer, int32_t val);
void _push_uint32_t(struct packet_write_buffer *buffer, uint32_t val);
void _push_int64_t(struct packet_write_buffer *buffer, int64_t val);
void _push_uint64_t(struct packet_write_buffer *buffer, uint64_t val);
void _push_float(struct packet_write_buffer *buffer, float val);
void _push_double(struct packet_write_buffer *buffer, double val);
void _push_vint32(struct packet_write_buffer *buffer, vint32_t val);
void _push_vint64(struct packet_write_buffer *buffer, vint64_t val);
void _push_string(struct packet_write_buffer *buffer, char *str);
void _push_slot(struct packet_write_buffer *buffer, struct slot_type *slot_data);

void pad_length(struct packet_write_buffer *buffer);
void init_packet_write_buffer(struct packet_write_buffer *buffer, uint32_t capacity);
