#include "bot.h"
#include "protocol.h"
#include "marshal.h"
#include "client.h"
#include "pipe.h"
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>

#define MAX_BOTS 256
#define MAILBOX_CAPACITY 1 << 10
#define POLL_TIMEOUT 10000
#define SLEEP_INTERVAL_US 50000 // tick(us), 50000 = 0.05s = 20Hz

//uint32_t num_bots;
//bot_t *bot_net[MAX_BOTS];

uint32_t num_bots;
bot_t **bot_list;
pipe_t **pipes;
pipe_t **timer_pipes;

/* These threads represent the main lines of execution for each bot.
 * Each bot has a method named "main" that is run.
 */
pthread_t *bot_threads;

/* These threads recognize game events and push them onto a queue for
 * a corresponding callback_executor in callback_executors.
 */
pthread_t *callback_listeners;

/* These threads are event handlers for each bot.
 */
pthread_t *callback_executors;

/* These threads handle delayed and recurring actions. Actions are pushed onto
 * a corresponding schedule_executor in schedule_executors.
 */
pthread_t *schedulers;

/* These threads execute actions queued by the corresponding scheduler.
 * Actions are executed in the order they were queued.
 */
pthread_t *schedule_executors;

void *callback_listener(void *index);
void *callback_executor(void *index);
void *scheduler(void *index);
void *schedule_executor(void *index);
void *bot_thread(void *bot);

void packet_callback(bot_t *bot, void *packet_struct, void *function);

/* Subtract the ‘struct timeval’ values X and Y,
 * storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 * See: http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
 */
int timeval_subtract (struct timeval *result,
                      struct timeval *x,
                      struct timeval *y)
{
    long y_sec = y->tv_sec;
    long y_usec = y->tv_usec;
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y_usec) {
        int nsec = (y_usec - x->tv_usec) / 1000000 + 1;
        y_usec -= 1000000 * nsec;
        y_sec += nsec;
    }
    if (x->tv_usec - y_usec > 1000000) {
        int nsec = (x->tv_usec - y_usec) / 1000000;
        y_usec += 1000000 * nsec;
        y_sec -= nsec;
    }

    /* Compute the time remaining to wait.
     *      tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y_sec;
    result->tv_usec = x->tv_usec - y_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y_sec;
}

void client_run(bot_t **bots, uint32_t num)
{
    // create 1 thread for receiving packets, and 1 for each bot
    uint64_t i;
    num_bots = num;
    bot_list = bots;

    // create all bot pipes
    pipes = calloc(num_bots, sizeof (pipe_t*));
    timer_pipes = calloc(num_bots, sizeof (pipe_t*));
    for (i = 0; i < num; i++) {
        pipes[i] = pipe_new(sizeof(void *), 0);
        timer_pipes[i] = pipe_new(sizeof(void *), 0);
    }

    // Create and start event listener threads.
    callback_listeners = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(callback_listeners + i, NULL, callback_listener, (void *)i);

    // Create and start callback threads that execute on events.
    callback_executors = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(callback_executors + i, NULL, callback_executor, (void *)i);

    // Create and start scheduler threads that handle timers and reccuring
    // actions.
    schedulers = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(schedulers + i, NULL, scheduler, (void *)i);

    // Create and start schedule executors that run jobs given by a scheduler.
    schedule_executors = calloc(num, sizeof(pthread_t));
    for (i = 0; i < num; i++)
        pthread_create(schedule_executors + i, NULL, schedule_executor, (void *)i);

    // Create bot main threads.
    bot_threads = calloc(num, sizeof (pthread_t));
    for(i = 0; i < num; i++)
        pthread_create(bot_threads + i, NULL, bot_thread, bot_list[i]);

    // wait for all threads to finish
    // TODO: support for exit codes
    for(i = 0; i < num; i++) {
        pthread_join(bot_threads[i], NULL);
        pthread_join(callback_listeners[i], NULL);
        pthread_join(callback_executors[i], NULL);
        pthread_join(schedulers[i], NULL);
        pthread_join(schedule_executors[i], NULL);
    }

    for (i = 0; i < num; i++) {
        pipe_free(pipes[i]);
        pipe_free(timer_pipes[i]);
    }

    free(bot_threads);
    free(callback_listeners);
    free(callback_executors);
    free(schedulers);
    free(schedule_executors);
}

void *bot_thread(void *bot)
{
    ((bot_t *)bot)->_data->bot_main(bot);
    return NULL;
}

void *callback_listener(void *index)
{
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];
    int ready;
    struct pollfd fds;
    fds.fd = bot->_data->socketfd;
    fds.events = POLLIN;

    pipe_producer_t *p = pipe_producer_new(pipes[i]);
    while (1) {
        ready = poll(&fds, 1, -1);
        if (ready < 0)
            perror("event listener");
        // read packet
        // send signal to corresponding thread
        if (fds.revents & POLLIN ) {
            // socket is ready to be read from
            void *data = protocol_decode(bot);
            pipe_push(p, &data, 1);
        }
    }

    pipe_producer_free(p);

    return NULL;
}

void *callback_executor(void *index)
{
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];

    int bytes_read;
    protocol_dummy_t *data;
    pipe_consumer_t *p = pipe_consumer_new(pipes[i]);
    while((bytes_read = pipe_pop(p, &data, 1))) {
        if (data == NULL) continue;

        int state = bot->_data->current_state;
        int pid = data->packet_id;

        function *func = &(bot->_data->callbacks[state][pid]);

        while(func->next) {
            packet_callback(bot, data, func->f);
            func = func->next;
        }
        free_packet(data);
    }

    pipe_consumer_free(p);

    return NULL;
}

void *scheduler(void *index)
{
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];
    pipe_producer_t *p = pipe_producer_new(timer_pipes[i]);
    struct timespec rqtp = {(SLEEP_INTERVAL_US)/1000000,
               (1000*SLEEP_INTERVAL_US)%1000000000
    };
    struct timeval tp, dt;
    struct timespec dummy;

    while (1) {
        nanosleep(&rqtp, &dummy);
        // Check for all functions that need to be called.
        timed_function *func = *(bot->_data->timers);
        while (func->f &&
               func->interval->tv_sec >= 0 && func->interval->tv_usec > 0) {
            gettimeofday(&tp, NULL);
            timeval_subtract(&dt, &tp, func->last_time_called);
            timeval_subtract(&dt, &dt, func->interval);
            // dt = current_time - last_call_time - interval_time
            if (dt.tv_sec >= 0 && dt.tv_usec > 0) {
                pipe_push(p, &func, 1);
            }
            func = func->next;
        }
    }

    pipe_producer_free(p);

    return NULL;
}

void *schedule_executor(void *index)
{
    int i = (uint64_t) index;
    bot_t *bot = bot_list[i];

    int bytes_read;
    timed_function *timer;
    pipe_consumer_t *p = pipe_consumer_new(timer_pipes[i]);
    // Execute whatever is in the pipe.
    while((bytes_read = pipe_pop(p, &timer, 1))) {
        if (timer == NULL) continue;
        gettimeofday(timer->last_time_called, NULL);
        if (timer->f) {
            if (!timer->repeat_count) {
                unregister_timer(bot, timer);
            } else {
                timer->f(bot, NULL);
                if (timer->repeat_count > 0)
                    --(timer->repeat_count);
            }
        }
    }

    pipe_consumer_free(p);

    return NULL;
}

void packet_callback(bot_t *bot, void *packet_struct, void *function)
{
    int packet_id = ((protocol_dummy_t *)packet_struct)->packet_id;
    switch (bot->_data->current_state) {
    case HANDSHAKE:
        switch (packet_id) {
        }
        break;
    case LOGIN:
        switch (packet_id) {
        case 0x00: {
            login_clientbound_disconnect_t *p = packet_struct;
            void (*f)(bot_t *, char *) = function;
            f(bot, p->json);
            break;
        }
        case 0x02: {
            login_clientbound_success_t *p = packet_struct;
            void (*f)(bot_t *, char *, char *) = function;
            f(bot, p->uuid, p->username);
            break;
        }
        case 0x03: {
            login_clientbound_set_compression_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t) = function;
            f(bot, p->threshold);
            break;
        }
        }
        break;
    case STATUS:
        switch (packet_id) {
        case 0x00: {
            status_clientbound_response_t *p = packet_struct;
            void (*f)(bot_t *, char *) = function;
            f(bot, p->json);
            break;
        }
        case 0x01: {
            status_clientbound_ping_t *p = packet_struct;
            void (*f)(bot_t *, int64_t) = function;
            f(bot, p->time);
            break;
        }
        }
        break;
    case PLAY:
        //if (packet_id) printf("Sending packet id: 0x%x\n", packet_id);
        switch (packet_id) {
        case 0x00: {
            play_clientbound_keepalive_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t) = function;
            f(bot, p->keepalive_id);
            break;
        }
        case 0x01: {
            play_clientbound_join_game_t *p = packet_struct;
            void (*f)(bot_t *, int32_t,
                      uint8_t,
                      int8_t,
                      uint8_t,
                      uint8_t,
                      char*) = function;
            f(bot, p->entity_id,
              p->gamemode,
              p->dimension,
              p->difficulty,
              p->max_players,
              p->level_type);
            break;
        }
        case 0x02: {
            play_clientbound_chat_t *p = packet_struct;
            void (*f)(bot_t *, char *, int8_t) = function;
            f(bot, p->json, p->position);
            break;
        }
        case 0x03: {
            play_clientbound_time_update_t *p = packet_struct;
            void (*f)(bot_t *, int64_t, int64_t) = function;
            f(bot, p->age, p->time);
            break;
        }
        case 0x04: {
            play_clientbound_entity_equipment_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t, int16_t, slot_t) = function;
            f(bot, p->entity_id, p->slot, p->item);
            break;
        }
        case 0x05: {
            play_clientbound_spawn_position_t *p = packet_struct;
            void (*f)(bot_t *, position_t) = function;
            f(bot, p->location);
            break;
        }
        case 0x06: {
            play_clientbound_update_health_t *p = packet_struct;
            void (*f)(bot_t *, float, vint32_t, float) = function;
            f(bot, p->health, p->food, p->saturation);
            break;
        }
        case 0x07: {
            play_clientbound_respawn_t *p = packet_struct;
            void (*f)(bot_t *, int32_t, uint8_t, uint8_t, char *) = function;
            f(bot, p->dimension, p->difficulty, p->gamemode, p->level_type);
            break;
        }
        case 0x08: {
            play_clientbound_position_t *p = packet_struct;
            void (*f)(bot_t *, double,
                      double,
                      double,
                      float,
                      float,
                      int8_t) = function;
            f(bot, p->x, p->y, p->z, p->yaw, p->pitch, p->flags);
            break;
        }
        case 0x09: {
            play_clientbound_item_change_t *p = packet_struct;
            void (*f)(bot_t *, int8_t) = function;
            f(bot, p->slot);
            break;
        }
        case 0x0A: {
            play_clientbound_use_bed_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t, position_t) = function;
            f(bot, p->entity_id, p->location);
            break;
        }
        case 0x0B: {
            play_clientbound_animation_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t, uint8_t) = function;
            f(bot, p->entity_id, p->animation);
            break;
        }
        case 0x0C: {
            play_clientbound_spawn_player_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      __uint128_t,
                      int32_t,
                      int32_t,
                      int32_t,
                      int8_t,
                      int8_t,
                      int16_t,
                      metadata_t) = function;
            f(bot, p->entity_id,
              p->uuid,
              p->x,
              p->y,
              p->z,
              p->yaw,
              p->pitch,
              p->item,
              p->metadata);
            break;
        }
        case 0x0D: {
            play_clientbound_collect_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t, vint32_t) = function;
            f(bot, p->collected_entity_id, p->collector_entity_id);
            break;
        }
        case 0x0E: {
            play_clientbound_spawn_object_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int8_t,
                      int32_t,
                      int32_t,
                      int32_t,
                      int8_t,
                      int8_t,
                      data_t) = function;
            f(bot, p->entity_id,
              p->type,
              p->x,
              p->y,
              p->z,
              p->yaw,
              p->pitch,
              p->data);
            break;
        }
        case 0x0F: {
            play_clientbound_spawn_mob_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      uint8_t,
                      int32_t,
                      int32_t,
                      int32_t,
                      int8_t,
                      int8_t,
                      int8_t,
                      int16_t,
                      int16_t,
                      int16_t,
                      metadata_t) = function;
            f(bot, p->entity_id,
              p->type,
              p->x,
              p->y,
              p->z,
              p->yaw,
              p->pitch,
              p->head_pitch,
              p->dx,
              p->dy,
              p->dz,
              p->metadata);
            break;
        }
        case 0x10: {
            play_clientbound_spawn_painting_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      char *,
                      position_t,
                      uint8_t) = function;
            f(bot, p->entity_id,
              p->title,
              p->location,
              p->direction);
            break;
        }
        case 0x11: {
            play_clientbound_spawn_xp_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int32_t,
                      int32_t,
                      int32_t,
                      int16_t) = function;
            f(bot, p->entity_id,
              p->x,
              p->y,
              p->z,
              p->count);
            break;
        }
        case 0x12: {
            play_clientbound_entity_velocity_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int16_t,
                      int16_t,
                      int16_t) = function;
            f(bot, p->entity_id,
              p->dx,
              p->dy,
              p->dz);
            break;
        }
        case 0x13: {
            play_clientbound_entity_destroy_entities_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t, vint32_t *) = function;
            f(bot, p->count, p->entity_ids);
            break;
        }
        case 0x14: {
            play_clientbound_entity_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t) = function;
            f(bot, p->entity_id);
            break;
        }
        case 0x15: {
            play_clientbound_entity_move_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int8_t,
                      int8_t,
                      int8_t,
                      bool) = function;
            f(bot, p->entity_id,
              p->dx,
              p->dy,
              p->dz,
              p->on_ground);
            break;
        }
        case 0x16: {
            play_clientbound_entity_look_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int8_t,
                      int8_t,
                      bool,
                      int8_t) = function;
            f(bot, p->entity_id,
              p->yaw,
              p->pitch,
              p->on_ground,
              p->pitch_fraction);
            break;
        }
        case 0x17: {
            play_clientbound_entity_look_move_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int8_t,
                      int8_t,
                      int8_t,
                      int8_t,
                      int8_t,
                      bool) = function;
            f(bot, p->entity_id,
              p->dx,
              p->dy,
              p->dz,
              p->yaw,
              p->pitch,
              p->on_ground);
            break;
        }
        case 0x18: {
            play_clientbound_entity_teleport_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int32_t,
                      int32_t,
                      int32_t,
                      int8_t,
                      int8_t,
                      bool) = function;
            f(bot, p->entity_id,
              p->x,
              p->y,
              p->z,
              p->yaw,
              p->pitch,
              p->on_ground);
            break;
        }
        case 0x19: {
            play_clientbound_entity_head_look_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int8_t) = function;
            f(bot, p->entity_id, p->yaw);
            break;
        }
        case 0x1A: {
            play_clientbound_entity_status_t *p = packet_struct;
            void (*f)(bot_t *, int32_t,
                      int8_t) = function;
            f(bot, p->entity_id, p->status);
            break;
        }
        case 0x1B: {
            play_clientbound_entity_attach_t *p = packet_struct;
            void (*f)(bot_t *, int32_t,
                      int32_t,
                      bool) = function;
            f(bot, p->entity_id, p->vehicle_id, p->leash);
            break;
        }
        case 0x1D: {
            play_clientbound_entity_effect_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int8_t,
                      int8_t,
                      vint32_t,
                      bool) = function;
            f(bot, p->entity_id,
              p->effect_id,
              p->amplifier,
              p->duration,
              p->hide);
            break;
        }
        case 0x1E: {
            play_clientbound_entity_clear_effect_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t, int8_t) = function;
            f(bot, p->entity_id, p->effect_id);
            break;
        }
        case 0x20: {
            play_clientbound_entity_properties_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int32_t,
                      property_t *) = function;
            f(bot, p->entity_id, p->count, p->properties);
            break;
        }
        case 0x1F: {
            play_clientbound_set_xp_t *p = packet_struct;
            void (*f)(bot_t *, float, int32_t, int32_t) = function;
            f(bot, p->xp_bar, p->level, p->xp);
            break;
        }
        case 0x21: {
            play_clientbound_chunk_data_t *p = packet_struct;
            void (*f)(bot_t *, int32_t,
                      int32_t,
                      bool,
                      uint16_t,
                      vint32_t,
                      int8_t *) = function;
            f(bot, p->chunk_x,
              p->chunk_z,
              p->continuous,
              p->bitmap,
              p->size,
              p->data);
            break;
        }
        case 0x22: {
            play_clientbound_multi_block_change_t *p = packet_struct;
            void (*f)(bot_t *, int32_t,
                      int32_t,
                      vint32_t,
                      record_t *) = function;
            f(bot, p->chunk_x,
              p->chunk_z,
              p->count,
              p->records);
            break;
        }
        case 0x23: {
            play_clientbound_block_change_t *p = packet_struct;
            void (*f)(bot_t *, position_t, vint32_t) = function;
            f(bot, p->location, p->block_id);
            break;
        }
        case 0x24: {
            play_clientbound_block_action_t *p = packet_struct;
            void (*f)(bot_t *, position_t,
                      uint8_t,
                      uint8_t,
                      vint32_t) = function;
            f(bot, p->location,
              p->byte1,
              p->byte2,
              p->type);
            break;
        }
        case 0x25: {
            play_clientbound_block_break_animation_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t, position_t, int8_t) = function;
            f(bot, p->entity_id, p->location, p->stage);
            break;
        }
        case 0x26: {
            play_clientbound_chunk_bulk_t *p = packet_struct;
            void (*f)(bot_t *, bool,
                      vint32_t,
                      int32_t,
                      int32_t,
                      uint16_t,
                      int8_t *) = function;
            f(bot, p->skylight,
              p->column_count,
              p->chunk_x,
              p->chunk_z,
              p->bitmap,
              p->data);
            break;
        }
        case 0x27: {
            play_clientbound_explosion_t *p = packet_struct;
            void (*f)(bot_t *, float,
                      float,
                      float,
                      float,
                      int32_t,
                      record_t *,
                      float,
                      float,
                      float) = function;
            f(bot, p->x,
              p->y,
              p->z,
              p->radius,
              p->count,
              p->records,
              p->dx,
              p->dy,
              p->dz);
            break;
        }
        case 0x28: {
            play_clientbound_effect_t *p = packet_struct;
            void (*f)(bot_t *, int32_t,
                      position_t,
                      int32_t,
                      bool) = function;
            f(bot, p->effect_id,
              p->location,
              p->data,
              p->relative);
            break;
        }
        case 0x29: {
            play_clientbound_sound_effect_t *p = packet_struct;
            void (*f)(bot_t *, char *,
                      int32_t,
                      int32_t,
                      int32_t,
                      float,
                      uint8_t) = function;
            f(bot, p->sound_name,
              p->x,
              p->y,
              p->z,
              p->volume,
              p->pitch);
            break;
        }
        case 0x2C: {
            play_clientbound_entity_spawn_global_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t,
                      int8_t,
                      int32_t,
                      int32_t,
                      int32_t) = function;
            f(bot, p->entity_id,
              p->type,
              p->x,
              p->y,
              p->z);
            break;
        }
        case 0x33: {
            play_clientbound_update_sign_t *p = packet_struct;
            void (*f)(bot_t *, position_t,
                      chat_t,
                      chat_t,
                      chat_t,
                      chat_t) = function;
            f(bot, p->location,
              p->line1,
              p->line2,
              p->line3,
              p->line4);
            break;
        }
        case 0x3F: {
            play_clientbound_plugin_message_t *p = packet_struct;
            void (*f)(bot_t *, char *, int8_t *) = function;
            f(bot, p->channel, p->data);
            break;
        }
        case 0x40: {
            play_clientbound_plugin_disconnect_t *p = packet_struct;
            void (*f)(bot_t *, char *) = function;
            f(bot, p->reason);
            break;
        }
        case 0x41: {
            play_clientbound_plugin_difficulty_t *p = packet_struct;
            void (*f)(bot_t *, uint8_t) = function;
            f(bot, p->difficulty);
            break;
        }
        case 0x46: {
            play_clientbound_set_compression_t *p = packet_struct;
            void (*f)(bot_t *, vint32_t) = function;
            f(bot, p->threshold);
            break;
        }
        }
        break;
    default:
        break;
    }
}
