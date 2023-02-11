/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** ftp.h
*/

#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pwd.h>
#include "hash_table.h"

// Utils
#define UNUSED __attribute__((unused))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? ((size_t)(-(x))) : (size_t)(x))
#define HASHCAST(x) ((void *)(size_t)(x))

// Constants
#define ANON_USER_LOGIN "Anonymous"
#define MAX_CLIENTS ((size_t)(1048576))
#define MAX_INPUT_SIZE ((size_t)(1610612736)) // 1.5 Go
#define RESPONSE_CONNECTED "220 FTP connection ready.\r\n"
#define RESPONSE_UNKNOW_CMD "500 Syntax error, command unrecognized.\r\n"


#define RESPONSE_STX_ERROR "501 Syntax error in parameters or arguments.\r\n"
#define RESPONSE_LOGGED_IN "230 User logged in, proceed.\r\n"
#define RESPONSE_NOT_LOGGED_IN "530 Not logged in.\r\n"
#define RESPONSE_USER_FOUND "331 User name okay, need password.\r\n"

#define RESPONSE_INVALID_SEQUENCE "503 Bad sequence of commands.\r\n"

UNUSED static const char *USAGE =
    "USAGE: ./myftp port path\n\tport  is the port number on which the server "
    "socket listens\n\tpath  is the path to the home directory for the "
    "Anonymous user\n";

// Structures
typedef struct {
    int max_pid;
    fd_set *read_set;
    fd_set *write_set;
    fd_set *except_set;
} fd_data_t;

typedef struct {
    int fd;

    char *i_buf;
    size_t i_buf_size;

    char *uname;
    char *pswd;
    struct passwd *user_data;

    char const *cwd;

    bool is_logged_in;
    char const *last_command;
} client_t;

typedef struct {
    int nb_client;
    int server_fd;
    int *client_fds;
    hash_table_t *clients;
    hash_table_t *cmd_map;
    char *anon_dir;
} server_t;

typedef struct {
    int port;
    char const * const anon_home_dir;
    bool is_help;
    bool is_error;
} args_t;

typedef void (*cmd_handler_t)(char *, client_t *, server_t *);

// Server
void server_destroy(server_t *server);
struct sockaddr_in get_server_socket_config(args_t const *args);
server_t *server_init(args_t const *args);

// Args
args_t get_args(int ac, char **av);

// Fd data
int get_first_input_available(fd_data_t *data, server_t *server);
fd_data_t fd_data_init(server_t *server);
void fd_data_destroy(fd_data_t *fd_data);
bool is_in_fd_array(int *fd_array, int len, int fd);
void remove_fd_from_array(int **array, int *len, int fd);

// Strings
bool str_ends_with(char *str, int str_len, char *end);
void append_str(char **str, size_t *str_len, char *to_append, int to_append_len);
void dputs(char *str, int fd);
void put_upper_case(char *str);

// Handlers
void handle_pass_command(char *command, client_t *client, server_t *serv);
void handle_user_command(char *command, client_t *client, server_t *serv);
