#include "bot.h"
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include "protocol.h"
#include <errno.h>
#include <stdio.h>

void client_run(bot_t *, uint32_t);

void *bot_thread(void *);

void *receiver(void *);

void signal_handler(int);
