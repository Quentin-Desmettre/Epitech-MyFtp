/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** server_run.c
*/

#include "myftp.h"

client_t *create_client(struct sockaddr_in *client_data, int client_fd)
{
    client_t *client = malloc(sizeof(client_t));

    memset(client, 0, sizeof(client_t));
    client->client_addr = *client_data;
    client->fd = client_fd;
    return client;
}

void server_handle_new_connection(server_t *server)
{
    struct sockaddr_in client_data;
    socklen_t client_data_size = sizeof(client_data);
    int client_fd = accept(server->server_fd,
    (struct sockaddr *)&client_data, &client_data_size);

    if (client_fd == -1)
        return;
    server->client_fds = realloc(server->client_fds,
    sizeof(int) * (server->nb_client + 1));
    server->client_fds[server->nb_client] = client_fd;
    server->nb_client++;
    hash_table_insert(server->clients,
    HASHCAST(client_fd), create_client(&client_data, client_fd));
    dputs(RESPONSE_CONNECTED, client_fd);
}

void server_disconnect_client(server_t *server, int client_pid)
{
    client_t *client = hash_table_erase(server->clients, HASHCAST(client_pid));

    if (!client)
        return;
    client_destroy(client);
    close(client_pid);
    remove_fd_from_array(&server->client_fds, &server->nb_client, client_pid);
}

void server_run(server_t *server)
{
    fd_data_t fd_data = {0, NULL, NULL, NULL};
    int select_rval;
    struct timeval select_timeout;
    int first_fd;

    while (server->run) {
        select_timeout = TIMEOUT;
        fd_data_destroy(&fd_data);
        fd_data = fd_data_init(server);
        select_rval = select(fd_data.max_pid + 1, fd_data.read_set,
            NULL, NULL, &select_timeout);
        if (select_rval <= 0)
            continue;
        first_fd = get_first_input_available(&fd_data, server);
        if (first_fd == server->server_fd)
            server_handle_new_connection(server);
        if (is_in_fd_array(server->client_fds, server->nb_client, first_fd))
            server_handle_client_input(server, first_fd);
    }
    fd_data_destroy(&fd_data);
}
