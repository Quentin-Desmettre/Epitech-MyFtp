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
    client->user_data = NULL;
}

void log_anon_user(client_t *client, char const *anon_dir)
{
    strcpy(client->cwd, anon_dir);
    client->uname = strdup(ANON_USER_LOGIN);
    client->root_dir = strdup(anon_dir);
    client->user_data = NULL;
    dputs(RESPONSE_USER_FOUND, client->fd);
}

void log_regular_user(client_t *client, struct passwd *user_data, char *uname)
{
    strcpy(client->cwd, user_data->pw_dir);
    client->user_data = user_data;
    client->root_dir = strdup(user_data->pw_dir);
    client->uname = uname;
    dputs(RESPONSE_USER_FOUND, client->fd);
}

void handle_user_command(char *command, client_t *client, server_t *serv)
{
    char *username = strdup(command + 5);
    struct passwd *user;

    if (strlen(username) <= 2 || command[4] != ' ') {
        dputs(RESPONSE_NOTHING_DONE, client->fd);
        return free(username);
    }
    unlog_client(client);
    username[strlen(username) - 2] = '\0';
    client->last_command = "USER";
    if (!strcmp(username, ANON_USER_LOGIN)) {
        log_anon_user(client, serv->anon_dir);
        return free(username);
    }
    if ((user = getpwnam(username)))
        return log_regular_user(client, user, username);
    dputs(RESPONSE_USER_FOUND, client->fd);
    free(username);
}
