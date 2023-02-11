/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** hash_table.h
*/

#pragma once

typedef struct stack {
    void *data;
    void *key;
    struct stack *next;
} stacks_t;

typedef struct {
    stacks_t **nodes;
    unsigned size;
} hash_table_t;

void *hash_table_find(hash_table_t *h, void *key);
void hash_table_insert(hash_table_t *h, void *key, void *value);
void push_to_stack(stacks_t **stack, void *key, void *data);
hash_table_t *hash_table_create(unsigned size);
void hashtable_clear(hash_table_t *map);
void *hash_table_erase(hash_table_t *map, void *key);
unsigned hash_function(void *key, unsigned max);
hash_table_t *init_command_map(void);
