/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** data_connection_handler.c
*/

#include "myftp.h"

void close_client(char const *message, client_t *client, int write_fd, int fd)
{
    dputs(message, client->fd);
    close(client->data_fd);
    close(fd);
    close(write_fd);
    exit(0);
}

fd_set init_fd_set(int fd)
{
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    return readfds;
}

static bool send_file_content(char *content, size_t size, int data_fd)
{
    size_t bytes_sent = 0;
    int result;

    while (bytes_sent < size) {
        result = write(data_fd, content + bytes_sent, size - bytes_sent);
        if (result < 0)
            return false;
        bytes_sent += result;
    }
    return true;
}

void send_fd_data_to_client(client_t *client, int fd, int write_fd)
{
    char buf[4096];
    int nb_read;

    while ((nb_read = read(fd, buf, 4096)) > 0)
        if (!send_file_content(buf, nb_read, write_fd))
            close_client(RESPONSE_FILE_TRANSFER_ABORTED,
            client, write_fd, fd);
    if (nb_read < 0)
        close_client(RESPONSE_FILE_LOCAL_ERROR, client, write_fd, fd);
    close_client(RESPONSE_FILE_TRANSFER_ENDED, client, write_fd, fd);
}

void handle_data_connection(char const *data, client_t *client,
void (*data_sender)(client_t *, char const *))
{
    printf("handle_data_connection\n");
    fd_set readfds = init_fd_set(client->data_fd);
    int selected;
    int pid;

    dputs(RESPONSE_FILE_TRANSFER_STARTED, client->fd);
    selected = (client->is_passive ?
    my_select(client->data_fd + 1, &readfds, NULL, NULL) : 1);
    if (selected <= 0)
        return dputs(RESPONSE_FILE_LOCAL_ERROR, client->fd);
    pid = fork();
    if (pid == -1)
        return dputs(RESPONSE_FILE_LOCAL_ERROR, client->fd);
    if (pid == 0)
        return data_sender(client, data);
    close(client->data_fd);
    client->data_fd = -1;
    client->is_passive = false;
    client->is_active = false;
}
