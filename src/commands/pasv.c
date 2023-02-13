/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** pasv.c
*/

#include "myftp.h"

void send_port_data(struct sockaddr_in *data_addr, client_t *client)
{
    unsigned char address_parts[4];
    unsigned short port_parts[2];

    sscanf(inet_ntoa(data_addr->sin_addr), "%hhu.%hhu.%hhu.%hhu",
    &address_parts[0], &address_parts[1],
    &address_parts[2], &address_parts[3]);
    port_parts[0] = ntohs(data_addr->sin_port) / 256;
    port_parts[1] = ntohs(data_addr->sin_port) % 256;
    dprintf(client->fd, "227 Entering Passive Mode "
    "(%hhu,%hhu,%hhu,%hhu,%hu,%hu)\r\n", address_parts[0], address_parts[1],
    address_parts[2], address_parts[3], port_parts[0], port_parts[1]);
}

void handle_pasv_command(char *command,
client_t *client, UNUSED server_t *serv)
{
    int data_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in data_addr;
    socklen_t addrlen = sizeof(data_addr);


    if (strcmp(command, "PASV\r\n"))
        return (void)close(data_socket), dputs(RESPONSE_STX_ERROR, client->fd);
    if (!client->is_logged_in)
        return (void)close(data_socket),
        dputs(RESPONSE_NOT_LOGGED_IN, client->fd);
    memcpy(&data_addr, &client->client_addr, addrlen);
    data_addr.sin_port = 0;
    if (data_socket < 0 || bind(data_socket,
    (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0 ||
    getsockname(data_socket, (struct sockaddr*) &data_addr, &addrlen) < 0)
        return (void)close(data_socket), dputs(RESPONSE_STX_ERROR, client->fd);
    listen(data_socket, 1);
    send_port_data(&data_addr, client);
    client->data_fd = data_socket;
    client->is_passive = true;
}
