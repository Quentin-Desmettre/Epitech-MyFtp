/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** strings.c
*/

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void append_str(char **str, size_t *str_len, char *to_append, int to_append_len)
{
    *str = realloc(*str, *str_len + to_append_len);
    memcpy(*str + *str_len, to_append, to_append_len);
    (*str_len) += to_append_len;
}

bool str_ends_with(char *str, int str_len, char *end)
{
    int end_len = strlen(end);

    if (str_len < end_len)
        return false;
    return !strncmp(str + str_len - end_len, end, end_len);
}

void put_upper_case(char *str)
{
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] -= 32;
    }
}

void dputs(char *str, int fd)
{
    write(fd, str, strlen(str));
}
