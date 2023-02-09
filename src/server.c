/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** server.c
*/

#include "myftp.h"

void server_destroy(server_t *server)
{
    free(server->client_fds);
    close(server->server_fd);
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

static bool server_listen(server_t *server, args_t const *args)
{
    return (listen(server->server_fd, args->port) >= 0) ? true : false;
}

server_t *server_init(args_t const *args)
{
    server_t *server = malloc(sizeof(server_t));

    server->client_fds = malloc(sizeof(int));
    server->client_fds[0] = -2;
    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_fd < 0 ||
    !server_bind(server, args) || !server_listen(server, args)) {
        server_destroy(server);
        return NULL;
    }
    return server;
}

