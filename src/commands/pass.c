/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** pass.c
*/

#include "myftp.h"

bool is_valid_password(char const *password, struct passwd *user_data)
{
    return true;
}

void handle_pass_command(char *command, client_t *client, server_t *serv)
{
    char *password = strdup(command + 5);
    const char *last_command = client->last_command;
    if (strlen(password) <= 2 && strcmp(client->uname, ANON_USER_LOGIN)) {
        dputs(RESPONSE_STX_ERROR, client->fd);
        return free(password);
    } else if (strlen(password) <= 2) {
        dputs(RESPONSE_LOGGED_IN, client->fd);
        client->is_logged_in = true;
        return free(password);
    }
    password[strlen(password) - 2] = '\0';
    client->last_command = "PASS";
    if (!last_command || strcmp(last_command, "USER"))
        return free(password), dputs(RESPONSE_INVALID_SEQUENCE, client->fd);
    if (is_valid_password(password, client->user_data)) {
        client->is_logged_in = true;
        dputs(RESPONSE_LOGGED_IN, client->fd);
    } else
        dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
}
