/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** server_handle_client_input.c
*/

#include "myftp.h"

char *extract_command_name(client_t *client, server_t *serv)
{
    size_t buf_size = client->i_buf_size;
    char *command_name = malloc(sizeof(char) * (buf_size + 1));
    char *i_buf = client->i_buf;

    command_name[0] = '\0';
    for (size_t i = 0; i < buf_size; i++) {
        command_name[i] = client->i_buf[i];
        command_name[i + 1] = '\0';
        if (hash_table_find(serv->cmd_map, command_name) &&
        ((i + 1 < buf_size && i_buf[i + 1] == ' ') ||
        (i + 2 < buf_size && i_buf[i + 1] == '\r' && i_buf[i + 2] == '\n')))
            return command_name;
    }
    free(command_name);
    return NULL;
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

void clean_input_buffer(client_t *client)
{
    client->i_buf = realloc(client->i_buf,
    sizeof(char) * (client->i_buf_size + 1));
    client->i_buf[client->i_buf_size] = '\0';
}

void server_handle_command(server_t *server, client_t *client)
{
    char *command;
    cmd_handler_t handler;

    clean_input_buffer(client);
    command = extract_command_name(client, server);
    if (!command) {
        dputs(RESPONSE_UNKNOW_CMD, client->fd);
        return;
    }
    put_upper_case(command);
    handler = hash_table_find(server->cmd_map, command);
    handler(command, client, server);
    free(command);
}

void server_handle_client_input(server_t *server, int client_pid)
{
    client_t *client = hash_table_find(server->clients, HASHCAST(client_pid));
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
