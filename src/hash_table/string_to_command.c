/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** string_to_command.c
*/

#include "hash_table.h"
#include "myftp.h"

void handle_cwd_command(char *command, client_t *client, server_t *serv)
{
}

void handle_cdup_command(char *command, client_t *client, server_t *serv)
{
}

void handle_quit_command(char *command, client_t *client, server_t *serv)
{
}

void handle_dele_command(char *command, client_t *client, server_t *serv)
{
}

void handle_pwd_command(char *command, client_t *client, server_t *serv)
{
}

void handle_pasv_command(char *command, client_t *client, server_t *serv)
{
}

void handle_port_command(char *command, client_t *client, server_t *serv)
{
}

void handle_help_command(char *command, client_t *client, server_t *serv)
{
}

void handle_noop_command(char *command, client_t *client, server_t *serv)
{
}

void handle_retr_command(char *command, client_t *client, server_t *serv)
{
}

void handle_stor_command(char *command, client_t *client, server_t *serv)
{
}

void handle_list_command(char *command, client_t *client, server_t *serv)
{
}

hash_table_t *init_command_map(void)
{
    hash_table_t *map = hash_table_create(10);

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
