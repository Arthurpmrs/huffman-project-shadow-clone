#include "binary_tree.h"

binary_tree_t *create_empty_binary_tree()
{
    binary_tree_t *bt = malloc(sizeof(binary_tree_t));
    bt->left = NULL;
    bt->right = NULL;
    return bt;
}

binary_tree_t *create_binary_tree(void *item, binary_tree_t *left, binary_tree_t *right)
{
    binary_tree_t *bt = create_empty_binary_tree();
    bt->left = left;
    bt->right = right;
    bt->item = item;
    return bt;
}

binary_tree_t *reconstruct_tree(uint16_t *i, uint16_t tree_size,
                                uint8_t preorder_tree[tree_size])
{
    uint8_t *item = malloc(sizeof(uint8_t));

    if (preorder_tree[*i] == '*')
    {
        *item = '*';
        *i += 1;
        binary_tree_t *left = reconstruct_tree(i, tree_size, preorder_tree);
        binary_tree_t *right = reconstruct_tree(i, tree_size, preorder_tree);
        return create_binary_tree((void *)item, left, right);
    }
    else
    {
        if (preorder_tree[*i] == '\\')
        {
            *item = preorder_tree[*i + 1];
            *i += 2;
        }
        else
        {
            *item = preorder_tree[*i];
            *i += 1;
        }

        return create_binary_tree((void *)item, NULL, NULL);
    }
}

void print_pre_order(binary_tree_t *bt, void (*print_fn)(void *))
{
    if (bt != NULL)
    {
        print_fn(bt->item);
        print_pre_order(bt->left, print_fn);
        print_pre_order(bt->right, print_fn);
    }
}