#include "protocol.h"
#include "queue.h"
#include "types.h"
#include "serial.h"
#include <uv.h>

int send_packet(struct bot_agent *bot, struct packet_write_buffer *b, size_t len);

#define BILLION (1000 * 1000 * 1000)

//void packet_throttler(void *arg) {
//    struct bot_agent *bot = arg;
//    /* Pop from the queue, send packet, then sleep in a loop */
//    while (1) {
//        struct packet_buffer p = queue_pop(&bot->packet_throttle_queue);
//        if (p.payload_len > 0) {
//            printf("throttled\n");
//            send_packet(bot, p.payload, p.payload_len);
//        }
//        nanosleep(&bot->throttle_sleep, NULL);
//    }
//}

void packet_throttle_cb (uv_timer_t* handle) {
    struct bot_agent *bot = handle->data;
    struct packet_buffer p = queue_pop(&bot->packet_throttle_queue);
    if (p.payload_len > 0) {
        struct packet_write_buffer b;
        init_packet_write_buffer(&b, VARINT_MAXLEN, p.payload_len);
        _push(&b, p.payload, p.payload_len);
        send_packet(bot, &b, p.payload_len);
    }
}

void init_throttle(struct bot_agent *bot, uint64_t milliseconds) {
    queue_init(&bot->packet_throttle_queue);
    //bot->throttle_sleep.tv_sec = nanoseconds / BILLION; 
    //bot->throttle_sleep.tv_nsec = nanoseconds % BILLION;
    //uv_thread_create(&bot->packet_throttle_thread, packet_throttler, bot);
    uv_timer_init(&bot->loop, &bot->packet_throttle_timer);
    bot->packet_throttle_timer.data = bot;
    uv_timer_start(&bot->packet_throttle_timer, packet_throttle_cb, milliseconds, milliseconds);
}
