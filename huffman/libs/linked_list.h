#ifndef HUFFMAN_LINKED_LIST_H_
#define HUFFMAN_LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct list_node list_node_t;
struct list_node
{
    void *item;
    list_node_t *next;
};

typedef struct list list_t;
struct list
{
    list_node_t *head;
    list_node_t *tail;
    uint64_t size;
    void (*print)(void *);
};

list_t *list_create(void (*print)(void *));

bool list_is_empty(list_t *list);

bool list_add_to_head(list_t *list, void *item);

bool list_add_to_tail(list_t *list, void *item);

void *list_delete_from_head(list_t *list);

bool list_delete(list_t *list);

#endif // HUFFMAN_LINKED_LIST_H_