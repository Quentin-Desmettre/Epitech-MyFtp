/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** noop.c
*/

#include "myftp.h"

void handle_noop_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    if (strcmp(command, "NOOP\r\n"))
        return dputs(RESPONSE_UNKNOW_CMD, client->fd);
    dputs(RESPONSE_OK, client->fd);
}
