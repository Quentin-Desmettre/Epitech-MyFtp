/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** pwd.c
*/

#include "myftp.h"

void handle_pwd_command(char *command, client_t *client, UNUSED server_t *serv)
{
    if (client->is_logged_in == false)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strcmp(command, "PWD\r\n"))
        return dputs(RESPONSE_STX_ERROR, client->fd);
    dprintf(client->fd, "257 \"%s\" is the current directory.\r\n",
    client->cwd);
}
