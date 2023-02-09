/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** main.c
*/

#include "myftp.h"

static int bad_config(args_t *args, char **av)
{
    if (args->is_help) {
        printf("%s", USAGE);
        return 0;
    } else if (args->is_error) {
        fprintf(stderr, "Invalid arguments.\n");
        fprintf(stderr, "%s", USAGE);
    } else
        perror(av[0]);
    return 84;
}

int main(int ac, char **av)
{
    args_t args = get_args(ac, av);
    server_t *server = ((args.is_error || args.is_help) ? NULL : server_init(&args));

    if (!server)
        return bad_config(&args, av);
    // server_run(server);
    server_destroy(server);
    return 0;
}
