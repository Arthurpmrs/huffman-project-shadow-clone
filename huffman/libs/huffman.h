#ifndef PLOTTING_HEAP_H_
#define PLOTTING_HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct huff_node huff_node_t;
struct huff_node
{
    void *byte;
    uint64_t frequency;
    huff_node_t *next;
    huff_node_t *left;
    huff_node_t *right;
};

typedef struct huff_queue huff_queue_t;
struct huff_queue
{
    uint16_t size;
    huff_node_t *head;
};

huff_queue_t *huff_create_queue();

void huff_enqueue(huff_queue_t *hq, void *byte, uint64_t frequency,
                  huff_node_t *left, huff_node_t *right);

huff_node_t *huff_dequeue(huff_queue_t *hq);

huff_node_t *huff_get_tree_from_queue(huff_queue_t *hq);

uint16_t huff_get_tree_size(huff_node_t *ht);

void huff_write_tree_to_file(huff_node_t *ht, FILE *output);

void huff_print_tree(huff_node_t *ht);
#endif