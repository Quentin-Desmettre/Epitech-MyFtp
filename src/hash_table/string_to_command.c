/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** string_to_command.c
*/

#include "myftp.h"
#include "help_messages.h"

static bool send_file_content(char *content, size_t size, int data_fd)
{
    size_t bytes_sent = 0;
    int result;

    while (bytes_sent < size) {
        result = write(data_fd, content + bytes_sent, size - bytes_sent);
        if (result < 0)
            return false;
        bytes_sent += result;
    }
    return true;
}

void send_fd_data_to_client(client_t *client, int fd, int write_fd)
{
    char buf[4096];
    int nb_read;

    while ((nb_read = read(fd, buf, 4096)) > 0)
        if (!send_file_content(buf, nb_read, write_fd))
            close_client(RESPONSE_FILE_TRANSFER_ABORTED,
            client, write_fd, fd);
    if (nb_read < 0)
        close_client(RESPONSE_FILE_LOCAL_ERROR, client, write_fd, fd);
    close_client(RESPONSE_FILE_TRANSFER_ENDED, client, write_fd, fd);
}

hash_table_t *init_command_map(void)
{
    hash_table_t *map = hash_table_create(10, str_cmp, str_hash);

    hash_table_insert(map, "USER", handle_user_command);
    hash_table_insert(map, "PASS", handle_pass_command);
    hash_table_insert(map, "CWD", handle_cwd_command);
    hash_table_insert(map, "CDUP", handle_cdup_command);
    hash_table_insert(map, "QUIT", handle_quit_command);
    hash_table_insert(map, "DELE", handle_dele_command);
    hash_table_insert(map, "PWD", handle_pwd_command);
    hash_table_insert(map, "PASV", handle_pasv_command);
    hash_table_insert(map, "PORT", handle_port_command);
    hash_table_insert(map, "HELP", handle_help_command);
    hash_table_insert(map, "NOOP", handle_noop_command);
    hash_table_insert(map, "RETR", handle_retr_command);
    hash_table_insert(map, "STOR", handle_stor_command);
    hash_table_insert(map, "LIST", handle_list_command);
    return map;
}

hash_table_t *init_command_help_map(void)
{
    hash_table_t *map = hash_table_create(10, str_cmp, str_hash);

    hash_table_insert(map, "USER", (void *)USER_HELP_MESSAGE);
    hash_table_insert(map, "PASS", (void *)PASS_HELP_MESSAGE);
    hash_table_insert(map, "CWD", (void *)CWD_HELP_MESSAGE);
    hash_table_insert(map, "CDUP", (void *)CDUP_HELP_MESSAGE);
    hash_table_insert(map, "QUIT", (void *)QUIT_HELP_MESSAGE);
    hash_table_insert(map, "DELE", (void *)DELE_HELP_MESSAGE);
    hash_table_insert(map, "PWD", (void *)PWD_HELP_MESSAGE);
    hash_table_insert(map, "PASV", (void *)PASV_HELP_MESSAGE);
    hash_table_insert(map, "PORT", (void *)PORT_HELP_MESSAGE);
    hash_table_insert(map, "HELP", (void *)HELP_HELP_MESSAGE);
    hash_table_insert(map, "NOOP", (void *)NOOP_HELP_MESSAGE);
    hash_table_insert(map, "RETR", (void *)RETR_HELP_MESSAGE);
    hash_table_insert(map, "STOR", (void *)STOR_HELP_MESSAGE);
    hash_table_insert(map, "LIST", (void *)LIST_HELP_MESSAGE);
    return map;
}
