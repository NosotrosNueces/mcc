#include <stdio.h>
#include "types.h"
#include "bot.h"
#include "protocol.h"

void *_read(void *buffer, void *storage, size_t size);
void *_read_vint32(void *buffer, int32_t *val);
void *_read_string(void *buffer, char **strptr, int32_t *len);

int main() {
    struct bot_agent bot;
    init_bot(&bot, "foo");
    join_server(&bot, "localhost", 25565);
    uv_run(&bot.loop, UV_RUN_DEFAULT);
    while(1);
    uv_loop_close(&bot.loop);
    

    //FILE *f = fopen("hello_world.nbt", "rb");
    //if (f == NULL) {
    //    return -1;
    //}
    //fseek(f, 0, SEEK_END);
    //long length = ftell(f);
    //fseek(f, 0, SEEK_SET);
    //char *nbt = malloc(length);
    //fread(nbt, length, sizeof(char), f);
    //uint32_t bytes_read;
    //struct nbt_tag *root = nbt_parse(nbt, &bytes_read);
    //nbt_print(root);
    //printf("Bytes read: %u\n", bytes_read);
    //printf("length: %ld\n", length);
}
