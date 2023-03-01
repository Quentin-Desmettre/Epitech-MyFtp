/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** retr_active.c
*/

#include "myftp.h"

void send_file_to_client(client_t *client, char const *file_path)
{
    printf("send_file_to_client\n");
    struct sockaddr_in data_addr;
    UNUSED socklen_t addrlen = sizeof(data_addr);
    int fd = -1;
    int write_fd = (client->is_passive ? accept(client->data_fd,
    (struct sockaddr *)&data_addr, &addrlen) : client->data_fd);

    if (client_chdir(client) < 0)
        close_client(RESPONSE_NOTHING_DONE, client, write_fd, fd);
    if ((fd = open(file_path, O_RDONLY)) < 0 || write_fd < 0)
        close_client(RESPONSE_NOTHING_DONE, client, write_fd, fd);
    send_fd_data_to_client(client, fd, write_fd);
}

void handle_retr_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *file_path = command + 5;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strlen(file_path) < 2 || command[4] != ' ')
        return dputs(RESPONSE_NOTHING_DONE, client->fd);
    file_path[strlen(file_path) - 2] = 0;
    if (access(file_path, R_OK))
        return dputs(RESPONSE_NOTHING_DONE, client->fd);
    if (client->is_passive || client->is_active)
        handle_data_connection(file_path, client, send_file_to_client);
    else
        dputs("425 Can't open data connection.\r\n", client->fd);
}
