/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** quit.c
*/

#include "myftp.h"

void handle_quit_command(char *command, client_t *client, server_t *serv)
{
    if (strcmp(command, "QUIT\r\n"))
        return dputs(RESPONSE_UNKNOW_CMD, client->fd);
    dputs(RESPONSE_CLOSING, client->fd);
    server_disconnect_client(serv, client->fd);
}
