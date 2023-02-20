/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** noop.c
*/

#include "myftp.h"

void *memdup(void *base, size_t size)
{
    void *new = malloc(size);

    if (!new)
        return NULL;
    memcpy(new, base, size);
    return new;
}

void handle_noop_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    if (strcmp(command, "NOOP\r\n"))
        return dputs(RESPONSE_UNKNOW_CMD, client->fd);
    dputs(RESPONSE_OK, client->fd);
}
