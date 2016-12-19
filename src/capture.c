#include <stdio.h>
#include "base64.h"
#include "capture.h"
#include "types.h"


/* For the time being, a capture of all minecraft traffic will be stored in the following JSON format:
    {
        
        [
            {'boundTo':<server/client>, 'payload': <base64-encoded payload>},
            ...
        ]
    }
 */

static const char head[] = "{\n    [\n";
static const char tail[] = "    ]\n}";

void init_capture(struct bot_agent *bot, const char *file) {
    bot->capture = fopen(file, "wb");
    fprintf(bot->capture, "%s", head);

    fprintf(bot->capture, "%s", tail);
}


void capture_packet(struct bot_agent *bot, enum packet_direction direction, void *payload, size_t payload_len) {
    /* generate base-64 encoded string */ 
    size_t encoded_len = 1 + BASE64_LENGTH (payload_len);
    char *encoded = malloc(encoded_len);
    base64_encode(payload, payload_len, encoded, encoded_len);
   
    /* Strip away the tail */
    fseek(bot->capture, -(sizeof(tail) - 1), SEEK_CUR);

    fprintf(bot->capture, "        {\n");
    fprintf(bot->capture, "            'bountTo': '%s',\n",
            direction == PACKET_DIRECTION_CLIENTBOUND ? "clientbound" : "serverbound");
    fprintf(bot->capture, "            'payload': '%s'\n", encoded);
    fprintf(bot->capture, "        },\n");
    fprintf(bot->capture, "%s", tail);
    free(encoded);
}

void end_capture(struct bot_agent *bot) {
    fclose(bot->capture);
}
