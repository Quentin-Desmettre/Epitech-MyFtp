/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** cwd_cdup.c
*/

#include "myftp.h"

bool change_to_dir(char const *base_dir, char const *new_dir,
char final_dir[PATH_MAX], client_t *client)
{
    char current_dir[PATH_MAX];
    char tmp_final_dir[PATH_MAX];

    strcpy(tmp_final_dir, base_dir);
    if (getcwd(current_dir, PATH_MAX) == NULL)
        return false;
    if (chdir(base_dir) == -1)
        return false;
    if (chdir(new_dir) == -1 || getcwd(final_dir, PATH_MAX) == NULL ||
    strlen(final_dir) < strlen(client->root_dir) ||
    strncmp(final_dir, client->root_dir, strlen(client->root_dir))) {
        strcpy(final_dir, tmp_final_dir);
        chdir(current_dir);
        return false;
    }
    chdir(current_dir);
    return true;
}

void handle_cwd_command(char *command, client_t *client, UNUSED server_t *serv)
{
    char *cmd = strdup(command + 4);

    if (!client->is_logged_in)
        return free(cmd), dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strlen(cmd) <= 2 || command[3] != ' ')
        return free(cmd), dputs(RESPONSE_NOTHING_DONE, client->fd);
    cmd[strlen(cmd) - 2] = '\0';
    if (change_to_dir(client->cwd, cmd, client->cwd, client))
        dputs(RESPONSE_FILE_ACT_DONE, client->fd);
    else
        dputs(RESPONSE_NOTHING_DONE, client->fd);
    free(cmd);
}

void handle_cdup_command(UNUSED char *command,
client_t *client, server_t *serv)
{
    if (strcmp(command, "CDUP\r\n"))
        return dputs(RESPONSE_STX_ERROR, client->fd);
    handle_cwd_command("CWD ..\r\n", client, serv);
}
