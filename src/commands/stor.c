/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** stor.c
*/

#include "myftp.h"

void get_file_from_client(client_t *client, char const *file_path)
{
    struct sockaddr_in data_addr;
    UNUSED socklen_t addrlen = sizeof(data_addr);
    int fd = -1;
    int read_fd = (client->is_passive ? accept(client->data_fd,
    (struct sockaddr *)&data_addr, &addrlen) : client->data_fd);
    char buf[4096];
    int nb_read;

    if (chdir(client->cwd) < 0)
        close_client(RESPONSE_FILE_LOCAL_ERROR, client, read_fd, fd);
    if ((fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0
    || read_fd < 0)
        close_client(RESPONSE_NOTHING_DONE, client, read_fd, fd);
    while ((nb_read = read(read_fd, buf, 4096)) > 0)
        write(fd, buf, nb_read);
    if (nb_read < 0) {
        remove(file_path);
        close_client(RESPONSE_FILE_TRANSFER_ABORTED, client, read_fd, fd);
    }
    close_client(RESPONSE_FILE_TRANSFER_ENDED, client, read_fd, fd);
}

void handle_stor_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *file_path = command + 5;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strlen(file_path) < 2 || command[4] != ' ')
        return dputs(RESPONSE_STX_ERROR, client->fd);
    file_path[strlen(file_path) - 2] = 0;
    if (client->is_passive || client->is_active)
        handle_data_connection(file_path, client, get_file_from_client);
    else
        dputs(RESPONSE_NOTHING_DONE, client->fd);
}
