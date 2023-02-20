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
    char current_dir[PATH_MAX];
    char anon_dir[PATH_MAX];

    if (!args->anon_home_dir || args->is_error)
        return;
    if (!getcwd(current_dir, PATH_MAX)) {
        args->is_error = true;
        return;
    }
    if (chdir(args->anon_home_dir) < 0) {
        chdir(current_dir);
        args->is_error = true;
        return;
    }
    if (!getcwd(anon_dir, PATH_MAX)) {
        chdir(current_dir);
        args->is_error = true;
        return;
    }
    strcpy(args->anon_home_dir, anon_dir);
}

args_t get_args(int ac, char **av)
{
    args_t args = {
        .port = 0, .is_help = false, .is_error = false
    };
    char *endPtr = NULL;

    if (ac == 3 && strlen(av[2]) < PATH_MAX)
        strcpy(args.anon_home_dir, av[2]);
    if (ac != 3) {
        args.is_error = true;
    } else if (ac == 2) {
        args.is_help = !strcmp(av[1], "-help");
        args.is_error = !args.is_help;
    } else {
        args.port = strtol(av[1], &endPtr, 10);
        check_path_validity(&args);
        args.is_error = args.is_error ||
        !(av[1][0] != '\0' && endPtr && *endPtr == '\0');
    }
    return args;
}
