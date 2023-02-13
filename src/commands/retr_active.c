/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** retr_active.c
*/

#include "myftp.h"

void handle_active(char const *file_path, client_t *client)
{
}

void handle_retr_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *file_path = command + 5;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strlen(file_path) < 2 || command[4] != ' ')
        return dputs(RESPONSE_STX_ERROR, client->fd);
    file_path[strlen(file_path) - 2] = 0;
    if (client->is_passive)
        return handle_passive(file_path, client);
    else if (client->is_active)
        return handle_active(file_path, client);
    return dputs(RESPONSE_NOTHING_DONE, client->fd);
}
