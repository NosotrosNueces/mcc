#include "bot.h"

void client_run(bot_t *, uint32_t);

void *bot_thread(void *);

void *receiver(void *);

void signal_handler(int);
