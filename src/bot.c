#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <uv.h>
#include <zlib.h>
#include "bot.h"
#include "protocol.h"
#include "types.h"

void init_callbacks(struct _callbacks *callbacks);

/* Populates the bot struct with default values */
void init_bot(struct bot_agent *bot, char *name) {
    bot->name = name;
    bot->current_state = LOGIN;
    uv_loop_init(&bot->loop);
	uv_tcp_init(&bot->loop, &bot->socket);
    bot->socket.data = bot;
    bot->current_state = HANDSHAKE;
    bot->mcc_status = MCC_OK;

    bot->packet_capacity = 512;
    bot->packet_length = 0;
    bot->packet_bytes_read = 0;
    bot->packet_buffer = malloc(bot->packet_capacity);

    bot->compression_enabled = false;
    bot->compression_threshold = -1;
    bot->compression_stream.zalloc = Z_NULL;
    bot->compression_stream.zfree = Z_NULL;
    bot->compression_stream.opaque = Z_NULL;
    deflateInit(&bot->compression_stream, Z_DEFAULT_COMPRESSION);

    bot->decompression_stream.zalloc = Z_NULL;
    bot->decompression_stream.zfree = Z_NULL;
    bot->decompression_stream.opaque = Z_NULL;
    bot->decompression_stream.avail_in = 0;
    bot->decompression_stream.next_in = Z_NULL;
    inflateInit(&bot->decompression_stream);

    bot->encryption_enabled = 0;

    init_callbacks(&bot->callbacks);
}

void init_callbacks(struct _callbacks *callbacks) {
    callbacks->clientbound_login_disconnect_cb = NULL;
    callbacks->clientbound_login_encryption_request_cb = NULL;
    callbacks->clientbound_login_login_success_cb = NULL;
    callbacks->clientbound_login_set_compression_cb = NULL;
    callbacks->clientbound_status_response_cb = NULL;
    callbacks->clientbound_status_pong_cb = NULL;
    callbacks->clientbound_play_spawn_object_cb = NULL;
    callbacks->clientbound_play_spawn_experience_orb_cb = NULL;
    callbacks->clientbound_play_spawn_global_entity_cb = NULL;
    callbacks->clientbound_play_spawn_mob_cb = NULL;
    callbacks->clientbound_play_spawn_painting_cb = NULL;
    callbacks->clientbound_play_spawn_player_cb = NULL;
    callbacks->clientbound_play_animation_cb = NULL;
    callbacks->clientbound_play_statistics_cb = NULL;
    callbacks->clientbound_play_block_break_animation_cb = NULL;
    callbacks->clientbound_play_update_block_entity_cb = NULL;
    callbacks->clientbound_play_block_action_cb = NULL;
    callbacks->clientbound_play_block_change_cb = NULL;
    callbacks->clientbound_play_boss_bar_cb = NULL;
    callbacks->clientbound_play_server_difficulty_cb = NULL;
    callbacks->clientbound_play_tab_complete_cb = NULL;
    callbacks->clientbound_play_chat_message_cb = NULL;
    callbacks->clientbound_play_multi_block_change_cb = NULL;
    callbacks->clientbound_play_confirm_transaction_cb = NULL;
    callbacks->clientbound_play_close_window_cb = NULL;
    callbacks->clientbound_play_open_window_cb = NULL;
    callbacks->clientbound_play_window_items_cb = NULL;
    callbacks->clientbound_play_window_property_cb = NULL;
    callbacks->clientbound_play_set_slot_cb = NULL;
    callbacks->clientbound_play_set_cooldown_cb = NULL;
    callbacks->clientbound_play_plugin_message_cb = NULL;
    callbacks->clientbound_play_named_sound_effect_cb = NULL;
    callbacks->clientbound_play_disconnect_cb = NULL;
    callbacks->clientbound_play_entity_status_cb = NULL;
    callbacks->clientbound_play_explosion_cb = NULL;
    callbacks->clientbound_play_unload_chunk_cb = NULL;
    callbacks->clientbound_play_change_game_state_cb = NULL;
    callbacks->clientbound_play_keep_alive_cb = NULL;
    callbacks->clientbound_play_chunk_data_cb = NULL;
    callbacks->clientbound_play_effect_cb = NULL;
    callbacks->clientbound_play_particle_cb = NULL;
    callbacks->clientbound_play_join_game_cb = NULL;
    callbacks->clientbound_play_map_cb = NULL;
    callbacks->clientbound_play_entity_relative_move_cb = NULL;
    callbacks->clientbound_play_entity_look_and_relative_move_cb = NULL;
    callbacks->clientbound_play_entity_look_cb = NULL;
    callbacks->clientbound_play_entity_cb = NULL;
    callbacks->clientbound_play_vehicle_move_cb = NULL;
    callbacks->clientbound_play_open_sign_editor_cb = NULL;
    callbacks->clientbound_play_player_abilities_cb = NULL;
    callbacks->clientbound_play_combat_event_cb = NULL;
    callbacks->clientbound_play_player_list_item_cb = NULL;
    callbacks->clientbound_play_player_position_and_look_cb = NULL;
    callbacks->clientbound_play_use_bed_cb = NULL;
    callbacks->clientbound_play_destroy_entities_cb = NULL;
    callbacks->clientbound_play_remove_entity_effect_cb = NULL;
    callbacks->clientbound_play_resource_pack_send_cb = NULL;
    callbacks->clientbound_play_respawn_cb = NULL;
    callbacks->clientbound_play_entity_head_look_cb = NULL;
    callbacks->clientbound_play_world_border_cb = NULL;
    callbacks->clientbound_play_camera_cb = NULL;
    callbacks->clientbound_play_held_item_change_cb = NULL;
    callbacks->clientbound_play_display_scoreboard_cb = NULL;
    callbacks->clientbound_play_entity_metadata_cb = NULL;
    callbacks->clientbound_play_attach_entity_cb = NULL;
    callbacks->clientbound_play_entity_velocity_cb = NULL;
    callbacks->clientbound_play_entity_equipment_cb = NULL;
    callbacks->clientbound_play_set_experience_cb = NULL;
    callbacks->clientbound_play_update_health_cb = NULL;
    callbacks->clientbound_play_scoreboard_objective_cb = NULL;
    callbacks->clientbound_play_set_passengers_cb = NULL;
    callbacks->clientbound_play_teams_cb = NULL;
    callbacks->clientbound_play_update_score_cb = NULL;
    callbacks->clientbound_play_spawn_position_cb = NULL;
    callbacks->clientbound_play_time_update_cb = NULL;
    callbacks->clientbound_play_title_cb = NULL;
    callbacks->clientbound_play_sound_effect_cb = NULL;
    callbacks->clientbound_play_player_list_header_and_footer_cb = NULL;
    callbacks->clientbound_play_collect_item_cb = NULL;
    callbacks->clientbound_play_entity_teleport_cb = NULL;
    callbacks->clientbound_play_entity_properties_cb = NULL;
    callbacks->clientbound_play_entity_effect_cb = NULL;
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

void on_connect(uv_connect_t *connect, int status) {
    if (status < 0) {
        fprintf(stderr, "Could not connect to server.\n");
        assert(0);
    }

    struct bot_agent *bot = connect->data;
    send_handshaking_serverbound_handshake(bot, PROTOCOL_VERSION, bot->server_addr, bot->server_port, 2);
    bot->current_state = LOGIN;
    send_login_serverbound_login_start(bot, bot->name);
    free(connect);
    
    
    /* start reading from the socket */
    uv_read_start((uv_stream_t *)&bot->socket, alloc_buffer, read_socket);

}

void join_server_ipaddr(struct bot_agent *bot, char *server_ip, int port_number)
{
	uv_connect_t *connect = malloc(sizeof(uv_connect_t));
    connect->data = bot;
    bot->server_addr = server_ip;
    bot->server_port = port_number;

	struct sockaddr_in dest;
	uv_ip4_addr(server_ip, port_number, &dest);
    //printf("%s:%d\n", server_ip, port_number); 
	uv_tcp_connect(connect, &bot->socket, (const struct sockaddr*)&dest, on_connect);
}

void getaddrinfo_cb(uv_getaddrinfo_t *req, int status, struct addrinfo *response) {
    if (status < 0) {
        fprintf(stderr, "Server not found.\n");
        assert(0);
    }

    struct bot_agent *bot = req->data;
    struct sockaddr_in *first = (struct sockaddr_in *)response->ai_addr;
    bot->server_port = ntohs(first->sin_port);
        
    join_server_ipaddr(bot, inet_ntoa(first->sin_addr), bot->server_port);

    //uv_ip4_name((struct sockaddr_in*) res->ai_addr, addr, 16);
    free(req);
}

void join_server_hostname(struct bot_agent *bot, char *server_hostname, char *service) {
    struct addrinfo hints;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    bot->server_addr = server_hostname;
    uv_getaddrinfo_t *req = malloc(sizeof(uv_getaddrinfo_t));
    req->data = bot;
    uv_getaddrinfo(&bot->loop, req, getaddrinfo_cb, server_hostname, service, NULL);
}

void disconnect(struct bot_agent *bot)
{
    uv_close((uv_handle_t *)&bot->socket, NULL);
}

