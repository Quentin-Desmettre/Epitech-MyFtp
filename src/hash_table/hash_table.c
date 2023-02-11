/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** hash_table.c
*/

#include "hash_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void *stack_find(stacks_t *stack, void *key,
bool (*cmp)(void *, void *))
{
    while (stack) {
        if (cmp(stack->key, key)) {
            return stack->data;
        }
        stack = stack->next;
    }
    return NULL;
}

hash_table_t *hash_table_create(unsigned size,
bool (*cmp)(void *, void *), unsigned (*hash)(void *))
{
    hash_table_t *h = malloc(sizeof(hash_table_t));

    h->nodes = malloc(sizeof(stacks_t *) * size);
    memset(h->nodes, 0, sizeof(stacks_t *) * size);
    h->size = size;
    h->cmp = cmp;
    h->hash = hash;
    return h;
}

void hash_table_insert(hash_table_t *h, void *key, void *value)
{
    unsigned index = h->hash(key) % h->size;

    push_to_stack(&(h->nodes[index]), key, value);
}

void *hash_table_find(hash_table_t *h, void *key)
{
    unsigned index = h->hash(key) % h->size;

    return stack_find(h->nodes[index], key, h->cmp);
}
