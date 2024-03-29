/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** list.c
*/

#include "myftp.h"
#include <dirent.h>

static void close_client_f(char const *message,
client_t *client, int write_fd, FILE *f)
{
    dputs(message, client->fd);
    close(client->data_fd);
    close(write_fd);
    if (f)
        pclose(f);
    exit(0);
}

char *getcmd(char const *file)
{
    char *cmd = calloc(strlen(file) + 23, sizeof(char));

    if (!cmd)
        return NULL;
    sprintf(cmd, "ls -l %s | sed -n '1d;p'", file);
    return cmd;
}

void send_list_to_client(client_t *client, char const *file)
{
    struct sockaddr_in data_addr;
    UNUSED socklen_t addrlen = sizeof(data_addr);
    int write_fd = (client->is_passive ? accept(client->data_fd,
    (struct sockaddr *)&data_addr, &addrlen) : client->data_fd);
    char *cmd = getcmd(file);
    FILE *f = NULL;
    char buf[4096];

    if (!cmd || client_chdir(client) < 0 || !(f = popen(cmd, "r")))
        return free(cmd), close_client_f(RESPONSE_NOTHING_DONE,
        client, write_fd, f);
    if (write_fd < 0)
        return free(cmd), close_client_f("425 Can't "
        "open data connection.\r\n", client, 0, f);
    while (fgets(buf, 4096, f))
        dprintf(write_fd, "%s", buf);
    pclose(f);
    dprintf(client->fd, RESPONSE_FILE_TRANSFER_STARTED, file);
    close_client_f(RESPONSE_FILE_TRANSFER_ENDED, client, write_fd, NULL);
}

void handle_list_command(char *cmd,
client_t *client, UNUSED server_t *serv)
{
    char *path = cmd + 5;
    DIR *dir;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (!strcmp(cmd, "LIST\r\n"))
        path = ".";
    else
        path[strlen(path) - 2] = 0;
    dir = opendir(path);
    if (((strlen(path) < 2 || cmd[4] != ' ') && strcasecmp(cmd, "LIST\r\n"))
    || !dir)
        return (dir ? closedir(dir) : (void)0),
        dputs(RESPONSE_NOTHING_DONE, client->fd);
    closedir(dir);
    if (client->is_passive || client->is_active)
        handle_data_connection(path, client, send_list_to_client);
    else
        dputs("425 Can't open data connection.\r\n", client->fd);
}
