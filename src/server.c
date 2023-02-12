/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** server.c
*/

#include "myftp.h"

void client_destroy(client_t *client)
{
    close(client->fd);
    free(client->i_buf);
    free(client->uname);
    free(client);
}

void server_destroy(server_t *server)
{
    client_t *client;

    for (int i = 0; i < server->nb_client; i++) {
        client = hash_table_erase(server->clients,
        HASHCAST(server->client_fds[i]));
        client_destroy(client);
    }
    free(server->client_fds);
    close(server->server_fd);
    hashtable_clear(server->clients);
    hashtable_clear(server->cmd_map);
    free(server);
}

struct sockaddr_in get_server_socket_config(args_t const *args)
{
    return (struct sockaddr_in){
        .sin_family = AF_INET,
        .sin_port = htons(args->port),
        .sin_addr.s_addr = INADDR_ANY
    };
}

static bool server_bind(server_t *server, args_t const *args)
{
    struct sockaddr_in server_socket_config = get_server_socket_config(args);

    return (bind(server->server_fd, (struct sockaddr *)&server_socket_config,
    sizeof(server_socket_config)) >= 0) ? true : false;
}

server_t *server_init(args_t const *args)
{
    server_t *server = malloc(sizeof(server_t));

    server->client_fds = NULL;
    server->nb_client = 0;
    server->clients = hash_table_create(MAX_CLIENTS, int_cmp, int_hash);
    server->cmd_map = init_command_map();
    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server->run = true;
    server->anon_dir = args->anon_home_dir;
    if (server->server_fd < 0 || !server_bind(server, args) ||
    listen(server->server_fd, MAX_CLIENTS) < 0) {
        server_destroy(server);
        return NULL;
    }
    return server;
}
