/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** server_run.c
*/

#include "myftp.h"

void server_handle_new_connection(server_t *server)
{
}

void server_handle_client_input(server_t *server, int client_pid)
{
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
        if (is_in_fd_array(server->client_fds, first_available_fd))
            server_handle_client_input(server, first_available_fd);
    }
}
