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
        command_name[i] = (i_buf[i] >= 'a' && i_buf[i] <= 'z')
        ? i_buf[i] - 32 : i_buf[i];
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
    if (ioctl(client_socket, FIONREAD, available_bytes) == -1 ||
    *available_bytes <= 0)
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
    if (!command)
        return dputs(RESPONSE_UNKNOW_CMD, client->fd);
    put_upper_case(command);
    for (int i = 0; command[i]; i++)
        client->i_buf[i] = (client->i_buf[i] >= 'a' && client->i_buf[i] <= 'z')
        ? client->i_buf[i] - 32 : client->i_buf[i];
    handler = hash_table_find(server->cmd_map, command);
    printf("Got '%s' as input\n", client->i_buf);
    handler(client->i_buf, client, server);
    free(client->i_buf);
    client->i_buf = NULL;
    client->i_buf_size = 0;
    free(command);
}

void server_handle_client_input(server_t *server, int client_pid)
{
    client_t *client = hash_table_find(server->clients, HASHCAST(client_pid));
    int rd_bytes;
    char *readable_input = (client ?
    get_available_input(client_pid, &rd_bytes) : 0);

    if (!client || rd_bytes < 0)
        return;
    if (rd_bytes == 0 ||
    client->i_buf_size + ABS(rd_bytes) > MAX_INPUT_SIZE)
        return server_disconnect_client(server, client_pid);
    append_str(&client->i_buf, &client->i_buf_size, readable_input, rd_bytes);
    free(readable_input);
    if (str_ends_with(client->i_buf, client->i_buf_size, "\r\n"))
        server_handle_command(server, client);
    if (hash_table_find(server->clients, HASHCAST(client_pid)) == client &&
    str_ends_with(client->i_buf, client->i_buf_size, "\r\n")) {
        free(client->i_buf);
        client->i_buf = NULL;
        client->i_buf_size = 0;
    }
}
