/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** retr.c
*/

#include "myftp.h"

bool send_file_content(char *content, size_t size, int data_fd)
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

void close_client_with_message(char const *message,
client_t *client, int write_fd)
{
    dputs(message, client->fd);
    close(client->data_fd);
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

void send_data_to_client(client_t *client, char const *file_path)
{
    struct sockaddr_in data_addr;
    UNUSED socklen_t addrlen = sizeof(data_addr);
    int fd;
    char buf[4096];
    int nb_read;
    int write_fd = accept(client->data_fd,
    (struct sockaddr *)&data_addr, &addrlen);

    if (chdir(client->cwd) < 0)
        close_client_with_message(RESPONSE_FILE_LOCAL_ERROR, client, write_fd);
    if ((fd = open(file_path, O_RDONLY)) < 0)
        close_client_with_message(RESPONSE_NOTHING_DONE, client, write_fd);
    while ((nb_read = read(fd, buf, 4096)) > 0)
        if (!send_file_content(buf, nb_read, write_fd))
            close_client_with_message(RESPONSE_FILE_TRANSFER_ABORTED,
            client, write_fd);
    if (nb_read < 0)
        close_client_with_message(RESPONSE_FILE_LOCAL_ERROR, client, write_fd);
    close_client_with_message(RESPONSE_FILE_TRANSFER_ENDED, client, write_fd);
}

void handle_passive(char const *file_path, client_t *client)
{
    fd_set readfds = init_fd_set(client->data_fd);
    struct sockaddr_in data_addr;
    socklen_t addrlen = sizeof(data_addr);
    int selected;
    int pid;

    dputs(RESPONSE_FILE_TRANSFER_STARTED, client->fd);
    selected = my_select(client->data_fd + 1, &readfds, NULL, NULL);
    if (selected < 0)
        return dputs(RESPONSE_FILE_LOCAL_ERROR, client->fd);
    pid = fork();
    if (pid == -1)
        return dputs(RESPONSE_FILE_LOCAL_ERROR, client->fd);
    if (pid == 0)
        return send_data_to_client(client, file_path);
    close(client->data_fd);
}
