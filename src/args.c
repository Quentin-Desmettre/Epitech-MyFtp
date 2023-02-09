/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** args.c
*/

#include "myftp.h"
#include <linux/limits.h>

static void check_path_validity(args_t *args)
{
    char currentDir[PATH_MAX];

    if (!args->anon_home_dir || args->is_error)
        return;
    if (!getcwd(currentDir, PATH_MAX) ||
    chdir(args->anon_home_dir) < 0 || chdir(currentDir) < 0) {
        args->is_error = true;
        return;
    }
}

args_t get_args(int ac, char **av)
{
    args_t args = {
        .port = 0,
        .anon_home_dir = (ac == 3 ? av[2] : NULL),
        .is_help = false,
        .is_error = false
    };
    char *endPtr = NULL;

    if (ac != 3) {
        args.is_error = true;
    } else if (ac == 2) {
        args.is_help = !strcmp(av[1], "-help");
        args.is_error = !args.is_help;
    } else {
        args.port = strtol(av[1], &endPtr, 10);
        check_path_validity(&args);
        args.is_error = !(av[1][0] != '\0' && endPtr && *endPtr == '\0');
    }
    return args;
}
