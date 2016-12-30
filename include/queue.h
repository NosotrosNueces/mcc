#pragma once

#include <stddef.h>
#include <stdint.h>
#include <uv.h>

#define QUEUE_CAP 2048

struct packet_buffer {
    size_t payload_len;
    char *payload;
};

/* Only 1 producer and 1 consumer, no need for */
struct queue {
    uv_sem_t fill_count, empty_count;
    /* Pop from head, push to tail */
    uint32_t head, tail; 
    struct packet_buffer buf[QUEUE_CAP];
};

void queue_init(struct queue *q);

/* Non-blocking operation. Returns -1 if queue is full */
int queue_push(struct queue *q, struct packet_buffer b);

/* Non-blocking operation. Returns {0, 0} if queue is empty */
struct packet_buffer queue_pop(struct queue *q);
