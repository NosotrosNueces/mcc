#include "queue.h"

void queue_init(struct queue *q) {
    uv_sem_init(&q->fill_count, 0);
    uv_sem_init(&q->empty_count, QUEUE_CAP);
    q->head = q->tail = 0;
}

/* Non-blocking operation. Returns -1 if queue is full */
int queue_push(struct queue *q, struct packet_buffer b) {
    int success = uv_sem_trywait(&q->empty_count);
    if (success != 0) {
        return -1;
    }
    q->buf[q->tail] = b;
    q->tail = (q->tail + 1) % QUEUE_CAP;
    uv_sem_post(&q->fill_count);
    return 0;
}

/* Non-blocking operation. Returns {0, 0} if queue is empty */
struct packet_buffer queue_pop(struct queue *q) {
    int success = uv_sem_trywait(&q->fill_count);
    if (success != 0) {
        return (struct packet_buffer){.payload_len = 0, .payload = NULL};
    }
    struct packet_buffer result = q->buf[q->head];
    q->head = (q->head + 1) % QUEUE_CAP;
    uv_sem_post(&q->empty_count);
    return result;
}
