/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** port.c
*/

#include "myftp.h"

bool get_address_infos(char const *command, struct sockaddr_in *s_in)
{
    unsigned char ips[4] = {0, 0, 0, 0};
    unsigned char ports[2] = {0, 0};
    char ip_buf[16];

    memset(ip_buf, 0, 16);
    if (sscanf(command, "PORT %hhu,%hhu,%hhu,%hhu,%hhu,%hhu\r\n",
    &ips[0], &ips[1], &ips[2], &ips[3], &ports[0], &ports[1]) != 6)
        return false;
    snprintf(ip_buf, 16, "%hhu.%hhu.%hhu.%hhu",
        ips[0], ips[1], ips[2], ips[3]);
    if (ip_buf[15] || !inet_aton(ip_buf, &s_in->sin_addr))
        return false;
    s_in->sin_family = AF_INET;
    s_in->sin_port = htons(ports[0] * 256 + ports[1]);
    return true;
}

void handle_port_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    struct sockaddr_in s_in;

    if (!client->is_logged_in)
        return dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    disable_current_mode(client);
    if (!get_address_infos(command, &s_in))
        return dputs(RESPONSE_STX_ERROR, client->fd);
    client->data_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->data_fd == -1 || connect(client->data_fd,
    (struct sockaddr *)&s_in, sizeof(s_in)) == -1)
        return dputs(RESPONSE_NOTHING_DONE, client->fd);
    client->is_active = true;
    client->is_passive = false;
    dputs(RESPONSE_OK, client->fd);
}
