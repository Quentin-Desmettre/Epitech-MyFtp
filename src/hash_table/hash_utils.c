/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** hash_utils.c
*/

#include <string.h>
#include <stdbool.h>

bool int_cmp(void *a, void *b)
{
    return (int)(size_t)a == (int)(size_t)b;
}

bool str_cmp(void *a, void *b)
{
    return strcmp((char *)a, (char *)b) == 0;
}

unsigned int_hash(void *integer)
{
    return (int)(size_t)integer;
}

unsigned str_hash(void *str)
{
    unsigned hash = 0;
    char *s = (char *)str;

    for (unsigned i = 0; s[i]; i++)
        hash = hash * 31 + s[i];
    return hash;
}
