/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** pass.c
*/

#include "myftp.h"

// The process of checking if a password if ok can be long.
// To optimize the server, we run this process in a separated thread.
void check_password(char *password, client_t *client)
{
    client->is_logged_in = true;
    return dputs(RESPONSE_LOGGED_IN, client->fd);
    int pid = fork();
    char *command;

    if (pid == -1)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    else if (pid != 0)
        return free(password);
    command = malloc(31 + strlen(password) + strlen(client->uname));
    sprintf(command,
    "echo '%s' | su - '%s' 2> /dev/null", password, client->uname);
    if (system(command) == 0) {
        client->is_logged_in = true;
        dputs(RESPONSE_LOGGED_IN, client->fd);
    } else
        dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    free(password);
    free(command);
}

void handle_pass_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *password = strdup(command + 5);
    const char *last_command = client->last_command;
    if (!last_command || strcmp(last_command, "USER"))
        return free(password), dputs(RESPONSE_INVALID_SEQUENCE, client->fd);
    if (client->uname && !strcmp(client->uname, ANON_USER_LOGIN)
    && !strcmp(command, "PASS \r\n")) {
        client->is_logged_in = true;
        return free(password), dputs(RESPONSE_LOGGED_IN, client->fd);
    }
    dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    free(password);
}
