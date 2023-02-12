/*
** EPITECH PROJECT, 2022
** B-NWP-400-LIL-4-1-myftp-quentin.desmettre
** File description:
** hash_table.h
*/

#pragma once
#include <stdbool.h>

typedef struct stack {
    void *data;
    void *key;
    struct stack *next;
} stacks_t;

typedef struct {
    stacks_t **nodes;
    unsigned size;
    bool (*cmp)(void *, void *);
    unsigned (*hash)(void *);
} hash_table_t;

void *hash_table_find(hash_table_t *h, void *key);
void hash_table_insert(hash_table_t *h, void *key, void *value);
void push_to_stack(stacks_t **stack, void *key, void *data);

hash_table_t *hash_table_create(unsigned size,
bool (*cmp)(void *, void *), unsigned (*hash)(void *));
bool int_cmp(void *a, void *b);
bool str_cmp(void *a, void *b);
unsigned int_hash(void *integer);
unsigned str_hash(void *str);

void hashtable_clear(hash_table_t *map);
void *hash_table_erase(hash_table_t *map, void *key);
unsigned hash_function(void *key, unsigned max);
hash_table_t *init_command_map(void);
hash_table_t *init_command_help_map(void);
