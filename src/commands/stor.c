/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** stor.c
*/

#include "myftp.h"
#include <stdio.h>
#include <sys/types.h>

void invert_buffer_clean(char **buf, long *buf_size)
{
    for (int i = 0; i < (*buf_size); i++) {
        if ((*buf)[i] == '\n') {
            (*buf_size)++;
            (*buf) = realloc((*buf), (*buf_size) + 1);
            memmove((*buf) + i + 1, (*buf) + i, (*buf_size) - i);
            (*buf)[i] = '\r';
            i++;
        }
    }
}

void clean_buffer(int fd, char *buf, char *last_char, long *nb_read)
{
    printf("Cleaned buffer\n");
    if (*last_char == '\r' && buf[0] != '\n')
        write(fd, "\r", 1);
    for (int i = 0; i < (*nb_read); i++) {
        if (buf[i] == '\r' && i + 1 < (*nb_read) && buf[i + 1] == '\n') {
            memmove(buf + i, buf + i + 1, (*nb_read) - i - 1);
            i--;
            (*nb_read)--;
        }
    }
    if (buf[(*nb_read) - 1] == '\r') {
        *last_char = '\r';
        (*nb_read)--;
    } else
        *last_char = 0;
}

void write_buffer(int fd, char *buf, char *last_char, long nb_read)
{
    clean_buffer(fd, buf, last_char, &nb_read);
    write(fd, buf, nb_read);
}

void get_file_from_client(client_t *client, char const *file_path)
{
    struct sockaddr_in data_addr;
    UNUSED socklen_t addrlen = sizeof(data_addr);
    int fd = -1;
    int read_fd = (client->is_passive ? accept(client->data_fd,
    (struct sockaddr *)&data_addr, &addrlen) : client->data_fd);
    char buf[BUFFER_SIZE];
    long nb_read;
    char last_char = 0;
    if (chdir(client->cwd) < 0)
        close_client(RESPONSE_FILE_LOCAL_ERROR, client, read_fd, fd);
    if ((fd = open(file_path, 01101, 0644)) < 0 || read_fd < 0)
        close_client(RESPONSE_NOTHING_DONE, client, read_fd, fd);
    while ((nb_read = read(read_fd, buf, BUFFER_SIZE)) > 0)
        write_buffer(fd, buf, &last_char, nb_read);
    if (last_char == '\r')
        write(fd, "\r", 1);
    if (nb_read < 0)
        return remove(file_path),
        close_client(RESPONSE_FILE_TRANSFER_ABORTED, client, read_fd, fd);
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
