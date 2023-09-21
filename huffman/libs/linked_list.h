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

/**
 * Creates a new linked list.
 *
 * @param print Function to print a list item.
 * @return list_t* Pointer to the new linked list.
 */
list_t *list_create(void (*print)(void *));

/**
 * Checks if a linked list is empty.
 *
 * @param list Pointer to a linked list.
 * @return true if the linked list is empty, false otherwise.
 */
bool list_is_empty(list_t *list);

/**
 * Adds an item to the head of a linked list.
 *
 * @param list Pointer to a linked list.
 * @param item The item to be added.
 * @return true if the item was added successfully, false otherwise.
 */
bool list_add_to_head(list_t *list, void *item);

/**
 * Adds an item to the tail of a linked list.
 *
 * @param list Pointer to a linked list.
 * @param item The item to be added.
 * @return true if the item was added successfully, false otherwise.
 */
bool list_add_to_tail(list_t *list, void *item);

/**
 * Deletes an item from the head of a linked list.
 *
 * @param list Pointer to a linked list.
 * @return void* Pointer to the deleted item.
 */
void *list_delete_from_head(list_t *list);

/**
 * Deletes all nodes from a linked list.
 *
 * @param list Pointer to a linked list.
 * @return true if the list was successfully deleted, false otherwise.
 */
bool list_delete(list_t *list);

/**
 * Prints a linked list.
 *
 * @param list Pointer to a linked list.
 */
bool list_print(list_t *list);

#endif // HUFFMAN_LINKED_LIST_H_