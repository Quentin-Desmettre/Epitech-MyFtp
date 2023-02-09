/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** hash_table.c
*/

#include "hash_table.h"
#include <string.h>
#include <stdlib.h>

static unsigned hash_function(int key, unsigned max)
{
    unsigned val = 0;

    for (size_t i = 0; i < sizeof(int); i++)
        val += (key >> (i * 8)) & 0xFF;
    return val % max;
}

static void *stack_find(stacks_t *stack, int key)
{
    while (stack) {
        if (stack->key == key)
            return stack->data;
        stack = stack->next;
    }
    return NULL;
}

hash_table_t *hash_table_create(unsigned size)
{
    hash_table_t *h = malloc(sizeof(hash_table_t));

    h->nodes = malloc(sizeof(stacks_t *) * size);
    memset(h->nodes, 0, sizeof(stacks_t *) * size);
    h->size = size;
    return h;
}

void hash_table_insert(hash_table_t *h, int key, void *value)
{
    unsigned index = hash_function(key, h->size);

    push_to_stack(&(h->nodes[index]), key, value);
}

void *hash_table_find(hash_table_t *h, int key)
{
    unsigned index = hash_function(key, h->size);

    return stack_find(h->nodes[index], key);
}
