/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** fd_data.c
*/

#include "myftp.h"

int fd_array_len(int *fd_array)
{
    int len = 0;

    for (; fd_array[len] != -1; len++);
    return len;
}

bool is_in_fd_array(int *fd_array, int fd)
{
    int len = fd_array_len(fd_array);

    for (int i = 0; i < len; i++) {
        if (fd_array[i] == fd)
            return true;
    }
    return false;
}

void fd_data_destroy(fd_data_t *fd_data)
{
    free(fd_data->read_set);
    free(fd_data->write_set);
    free(fd_data->except_set);
}

fd_data_t fd_data_init(server_t *server)
{
    fd_set *read_set = malloc(sizeof(fd_set));
    int nb_fd = fd_array_len(server->client_fds);
    int max_fd = server->server_fd;

    FD_ZERO(read_set);
    FD_SET(server->server_fd, read_set);
    for (int i = 0; i < nb_fd; i++) {
        FD_SET(server->client_fds[i], read_set);
        max_fd = MAX(max_fd, server->client_fds[i]);
    }
    return (fd_data_t) {
        .max_pid = server->server_fd,
        .read_set = read_set,
        .write_set = NULL,
        .except_set = NULL
    };
}

int get_first_input_available(fd_data_t *data, server_t *server)
{
    int nb_fd = fd_array_len(server->client_fds);

    if (FD_ISSET(server->server_fd, data->read_set))
        return server->server_fd;
    for (int i = 0; i < nb_fd; i++) {
        if (FD_ISSET(server->client_fds[i], data->read_set))
            return server->client_fds[i];
    }
    return -1;
}
