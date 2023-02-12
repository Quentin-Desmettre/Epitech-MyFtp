/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** help.c
*/

#include "myftp.h"
#include "help_messages.h"

void handle_help_command(char *command, client_t *client, server_t *serv)
{
    char *cmd;
    const char *help_message;

    if (!strcmp(command, "HELP\r\n"))
        return dputs(HELP_MESSAGE, client->fd);
    if (command[4] != ' ')
        return dputs(RESPONSE_UNKNOW_CMD, client->fd);
    cmd = strdup(command + 5);
    cmd[strlen(cmd) - 2] = '\0';
    put_upper_case(cmd);
    if (!(help_message = hash_table_find(serv->cmd_help_map, cmd))) {
        printf("not found '%s'\n", cmd);
        return free(cmd), dputs(RESPONSE_UNKNOW_CMD, client->fd);
    }
    dputs(help_message, client->fd);
    free(cmd);
}
