/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** dele.c
*/

#include "myftp.h"

void handle_dele_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *cmd = strdup(command + 5);
    char *filePath;

    if (!client->is_logged_in)
        return free(cmd), dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strlen(cmd) <= 2 || command[4] != ' ')
        return free(cmd), dputs(RESPONSE_NOTHING_DONE, client->fd);
    if (strlen(client->cwd) + strlen(cmd) + 2 > PATH_MAX)
        return free(cmd), dputs(RESPONSE_NOTHING_DONE, client->fd);
    cmd[strlen(cmd) - 2] = '\0';
    filePath = malloc(strlen(client->cwd) + strlen(cmd) + 2);
    sprintf(filePath, "%s/%s", client->cwd, cmd);
    if (remove(filePath) == -1)
        dputs(RESPONSE_NOTHING_DONE, client->fd);
    else
        dputs(RESPONSE_FILE_ACT_DONE, client->fd);
    free(cmd);
    free(filePath);
}
