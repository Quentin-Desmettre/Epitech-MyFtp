/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** list.c
*/

#include "myftp.h"

static void close_client_f(char const *message,
client_t *client, int write_fd, FILE *f)
{
    dputs(message, client->fd);
    close(client->data_fd);
    close(write_fd);
    fclose(f);
    exit(0);
}

void send_list_to_client(client_t *client, char const *file)
{
    struct sockaddr_in data_addr;
    UNUSED socklen_t addrlen = sizeof(data_addr);
    int fd;
    int write_fd = accept(client->data_fd,
    (struct sockaddr *)&data_addr, &addrlen);
    FILE *file_buf;
    char cmd_buf[PATH_MAX + 10];

    if (chdir(client->cwd) < 0 || strlen(file) > PATH_MAX)
        close_client_f(RESPONSE_FILE_LOCAL_ERROR, client, write_fd, NULL);
    strcpy(cmd_buf, "ls -l ");
    strcat(cmd_buf, file);
    file_buf = popen(cmd_buf, "r");
    if (!file_buf || write_fd < 0 || (fd = fileno(file_buf)) < 0)
        close_client_f(RESPONSE_NOTHING_DONE, client, write_fd, file_buf);
    fd = dup(fd);
    fclose(file_buf);
    send_fd_data_to_client(client, fd, write_fd);
}

void handle_active_list(char const *path, client_t *client)
{
}

void handle_list_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *path = command + 5;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if ((strlen(path) < 2 || command[4] != ' ') && strcmp(command, "LIST\r\n"))
        return dputs(RESPONSE_STX_ERROR, client->fd);
    if (!strcmp(command, "LIST\r\n"))
        path = ".";
    else
        path[strlen(path) - 2] = 0;
    if (client->is_passive)
        handle_passive(path, client, send_list_to_client);
    else if (client->is_active)
        handle_active_list(path, client);
    else
        dputs(RESPONSE_NOTHING_DONE, client->fd);
    client->is_active = false;
    client->is_passive = false;
}
