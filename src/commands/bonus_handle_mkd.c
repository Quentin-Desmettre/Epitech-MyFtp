/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** bonus_handle_mkd.c
*/

#include "myftp.h"
#include <sys/types.h>
#include <dirent.h>

void analyze_dir(const char *path, size_t path_len, struct dirent *p, int *r)
{
    int r2 = -1;
    size_t len = path_len + strlen(p->d_name) + 2;
    char *buf = (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) ? NULL
    : malloc(len);
    struct stat statbuf;

    if (!buf)
        return;
    snprintf(buf, len, "%s/%s", path, p->d_name);
    if (!stat(buf, &statbuf))
        r2 = S_ISDIR(statbuf.st_mode) ? remove_directory(buf) : unlink(buf);
    *r = r2;
    free(buf);
}

int remove_directory(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = 0;
    struct dirent *p;

    if (!d) {
        perror("opendir");
        return -1;
    }
    while (!r && (p = readdir(d)))
        analyze_dir(path, path_len, p, &r);
    closedir(d);
    if (!r)
        return rmdir(path);
    return r;
}

void handle_mkd_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *dir = command + 4;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strlen(command) < 7 || command[3] != ' ')
        return dputs(RESPONSE_STX_ERROR, client->fd);
    if (strlen(client->cwd) + strlen(dir) + 2 > PATH_MAX)
        return dputs(RESPONSE_NOTHING_DONE, client->fd);
    dir[strlen(dir) - 2] = '\0';
    if (mkdir(dir, 0777) == -1)
        dputs(RESPONSE_NOTHING_DONE, client->fd);
    else
        dprintf(client->fd, "257 \"%s\" created.\r\n", dir);
}

void handle_rmd_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    char *dir = command + 4;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    if (strlen(command) < 7 || command[3] != ' ')
        return dputs(RESPONSE_STX_ERROR, client->fd);
    if (strlen(client->cwd) + strlen(dir) + 2 > PATH_MAX)
        return dputs(RESPONSE_NOTHING_DONE, client->fd);
    dir[strlen(dir) - 2] = '\0';
    if (remove_directory(dir) == -1)
        dputs(RESPONSE_NOTHING_DONE, client->fd);
    else
        dputs(RESPONSE_FILE_ACT_DONE, client->fd);
}
