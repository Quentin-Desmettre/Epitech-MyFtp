/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** retr.c
*/

#include "myftp.h"

fd_set init_fd_set(int fd)
{
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    return readfds;
}

void send_file_to_client(client_t *client, char const *file_path)
{
    struct sockaddr_in data_addr;
    UNUSED socklen_t addrlen = sizeof(data_addr);
    int fd = -1;
    int write_fd = accept(client->data_fd,
    (struct sockaddr *)&data_addr, &addrlen);

    if (chdir(client->cwd) < 0)
        close_client(RESPONSE_FILE_LOCAL_ERROR, client, write_fd, fd);
    if ((fd = open(file_path, O_RDONLY)) < 0 || write_fd < 0)
        close_client(RESPONSE_NOTHING_DONE, client, write_fd, fd);
    send_fd_data_to_client(client, fd, write_fd);
}

void handle_passive(char const *data, client_t *client,
void (*data_sender)(client_t *, char const *))
{
    fd_set readfds = init_fd_set(client->data_fd);
    int selected;
    int pid;

    dputs(RESPONSE_FILE_TRANSFER_STARTED, client->fd);
    selected = my_select(client->data_fd + 1, &readfds, NULL, NULL);
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
}
