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

void *hash_table_erase(hash_table_t *map, int key)
{
    unsigned index = hash_function(key, map->size);
    stacks_t *s = map->nodes[index];
    stacks_t *prev = NULL;

    while (s) {
        if (s->key == key && prev)
            prev->next = s->next;
        if (s->key == key && !prev)
            map->nodes[index] = s->next;
        if (s->key == key)
            return pop_stack(&s);
        prev = s;
        s = s->next;
    }
    return NULL;
}

void hashtable_clear(hash_table_t *map)
{
    for (unsigned i = 0; i < map->size; i++)
        while (map->nodes[i])
            free(pop_stack(&(map->nodes[i])));
}
