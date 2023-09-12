#ifndef PLOTTING_HEAP_H_
#define PLOTTING_HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct huff_node huff_node_t;
struct huff_node
{
    void *item;
    uint64_t frequency;
    huff_node_t *left;
    huff_node_t *right;
};

typedef struct huff_heap huff_heap_t;
struct huff_heap
{
    uint64_t size;
    uint64_t capacity;
    huff_node_t **data;
    void (*print)(void *);
    int (*compare)(void *, void *);
};

huff_heap_t *hp_create(uint16_t capacity, void (*print)(void *),
                       int (*compare)(void *, void *));

uint32_t hp_get_parent_index(uint32_t i);

uint32_t hp_get_left_index(uint32_t i);

uint32_t hp_get_right_index(uint32_t i);

huff_node_t *hp_item_of(huff_heap_t *heap, uint32_t i);

void hp_enqueue(huff_heap_t *heap, void *item, uint64_t frequency, huff_node_t *left, huff_node_t *right);

void hp_min_heapify(huff_heap_t *heap, uint32_t origin);

huff_node_t *hp_dequeue(huff_heap_t *heap);

void hp_print(huff_heap_t *heap);

#endif