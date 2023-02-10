/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** server_run.c
*/

#include "myftp.h"
#include <sys/ioctl.h>

client_t *create_client(struct sockaddr_in *client_data)
{
    client_t *client = malloc(sizeof(client_t));
    client_t data = {
        .fd = -1,
        .i_buf = NULL,
        .i_buf_size = 0,
        .uname = NULL,
        .pswd = NULL
    };

    memcpy(client, &data, sizeof(client_t));
}

void server_handle_new_connection(server_t *server)
{
    struct sockaddr_in client_data;
    socklen_t client_data_size = sizeof(client_data);
    int client_pid = accept(server->server_fd, &client_data, &client_data_size);

    if (client_pid == -1)
        return;
    server->client_fds = realloc(server->client_fds, sizeof(int) * (server->nb_client + 1));
    server->client_fds[server->nb_client] = client_pid;
    server->nb_client++;
    hash_table_insert(server->clients, client_pid, create_client(&client_data));
    write(client_pid, RESPONSE_CONNECTED, strlen(RESPONSE_CONNECTED));
}

void server_disconnect_client(server_t *server, int client_pid)
{
    client_t *client = hash_table_erase(server->clients, client_pid);

    if (!client)
        return;
    close(client_pid);
    remove_fd_from_array(&server->client_fds, &server->nb_client, client_pid);
}

char *get_available_input(int client_socket, int *available_bytes)
{
    char *input;
    int bytes_read;

    *available_bytes = -1;
    if (ioctl(client_socket, FIONREAD, available_bytes) == -1 || *available_bytes <= 0)
        return NULL;
    input = malloc(sizeof(char) * (*available_bytes));
    bytes_read = read(client_socket, input, *available_bytes);
    *available_bytes = bytes_read;
    if (bytes_read <= 0) {
        free(input);
        return NULL;
    }
    return input;
}

char *extract_command_name(client_t *client)
{
    char *command_name = malloc(sizeof(char) * (client->i_buf_size + 1));

    command_name[0] = '\0';
    for (size_t i = 0; i < client->i_buf_size; i++) {

    }
}

void clean_input_buffer(client_t *client)
{
    client->i_buf = realloc(client->i_buf,
    sizeof(char) * (client->i_buf_size + 1));
    client->i_buf[client->i_buf_size] = '\0';
}

void server_handle_command(server_t *server, client_t *client)
{
    char *command;
    clean_input_buffer(client);

    // Extract the requested command.
    // To do so, extract all characters until space is encounter, or the "\r\n"
}

void server_handle_client_input(server_t *server, int client_pid)
{
    client_t *client = hash_table_find(server->clients, client_pid);
    char *readable_input;
    int available_bytes;

    if (!client)
        return;
    readable_input = get_available_input(client_pid, &available_bytes);
    if (available_bytes < 0)
        return;
    if (available_bytes == 0 ||
    client->i_buf_size + ABS(available_bytes) > MAX_INPUT_SIZE) {
        server_disconnect_client(server, client_pid);
        return;
    }
    append_str(&client->i_buf, &client->i_buf_size,
    readable_input, available_bytes);
    free(readable_input);
    if (str_ends_with(client->i_buf, client->i_buf_size, "\r\n"))
        server_handle_command(server, client);
}

void server_run(server_t *server)
{
    fd_data_t fd_data = {0, NULL, NULL, NULL};
    int select_rval;
    struct timeval select_timeout;
    int first_available_fd;

    while (1) {
        select_timeout = (struct timeval){.tv_sec = 0, .tv_usec = 100};
        fd_data_destroy(&fd_data);
        fd_data = fd_data_init(server);
        select_rval = select(fd_data.max_pid + 1, fd_data.read_set,
            fd_data.write_set, fd_data.except_set, &select_timeout);
        if (select_rval <= 0)
            continue;
        first_available_fd = get_first_input_available(&fd_data, server);
        if (first_available_fd == server->server_fd)
            server_handle_new_connection(server);
        if (is_in_fd_array(server->client_fds, server->nb_client, first_available_fd))
            server_handle_client_input(server, first_available_fd);
    }
}
