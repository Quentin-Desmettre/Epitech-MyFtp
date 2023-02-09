/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** stack.c
*/

#include "hash_table.h"
#include <stdlib.h>

void push_to_stack(stacks_t **stack, int key, void *data)
{
    stacks_t *s = malloc(sizeof(stacks_t));

    s->data = data;
    s->key = key;
    s->next = *stack;
    *stack = s;
}

void *pop_stack(stacks_t **s)
{
    void *data = (*s)->data;
    stacks_t *next = (*s)->next;

    free(*s);
    *s = next;
    return data;
}

void hashtable_clear(hash_table_t *map)
{
    for (unsigned i = 0; i < map->size; i++)
        while (map->nodes[i])
            free(pop_stack(&(map->nodes[i])));
}
