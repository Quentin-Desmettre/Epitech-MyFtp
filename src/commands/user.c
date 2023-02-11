/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** user.c
*/

#include "myftp.h"

void unlog_client(client_t *client)
{
    client->is_logged_in = false;
    free(client->uname);
    client->uname = NULL;
    free(client->pswd);
    client->pswd = NULL;
}

void log_anon_user(client_t *client, char const *anon_dir)
{
    client->cwd = anon_dir;
    client->user_data = NULL;
    client->is_logged_in = true;
    dputs(RESPONSE_LOGGED_IN, client->fd);
}

void prepare_log_regular_user(client_t *client, struct passwd *user_data)
{
    client->cwd = user_data->pw_dir;
    client->user_data = user_data;
    dputs(RESPONSE_USER_FOUND, client->fd);
}

void handle_user_command(char *command, client_t *client, server_t *serv)
{
    char *username = strdup(command + 5);
    struct passwd *user;

    if (strlen(username) == 0) {
        dputs(RESPONSE_STX_ERROR, client->fd);
        return free(username);
    }
    unlog_client(client);
    username[strlen(username) - 2] = '\0';
    client->last_command = "USER";
    if (!strcmp(username, ANON_USER_LOGIN)) {
        log_anon_user(client, serv->anon_dir);
        return free(username);
    }
    user = getpwnam(username);
    if (user)
        return prepare_log_regular_user(client, user);
    dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    client->last_command = NULL;
    free(username);
}
