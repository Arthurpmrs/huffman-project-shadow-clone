#ifndef HUFFMAN_BINARY_TREE_H_
#define HUFFMAN_BINARY_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct binary_tree binary_tree_t;
struct binary_tree
{
    void *item;
    binary_tree_t *left;
    binary_tree_t *right;
};

binary_tree_t *create_empty_binary_tree();

binary_tree_t *create_binary_tree(void *item, binary_tree_t *left,
                                  binary_tree_t *right);

/**
 * @brief Reconstructs the Huffman tree from preorder string
 *
 * @param i Auxiliar index (used to go through the preorder string - must be 0)
 * @param tree_size Size of the Huffman binary tree
 * @param preorder_tree String holding the Huffman tree in preorder
 * @return binary_tree_t* Reconstructed Huffman tree
 */
binary_tree_t *reconstruct_tree(uint16_t *i, uint16_t tree_size,
                                uint8_t preorder_tree[tree_size]);

void print_pre_order(binary_tree_t *bt, void (*print_fn)(void *));

#endif // HUFFMAN_BINARY_TREE_H_