#include "huffman.h"

bool is_special_node(huff_node_t *node)
{
    bool is_leaf = node->left == NULL &&
                   node->right == NULL;

    bool is_special_char = *(uint8_t *)node->byte == '\\' ||
                           *(uint8_t *)node->byte == '*';

    return is_leaf && is_special_char;
}

huff_queue_t *huff_create_queue()
{
    huff_queue_t *new_queue = malloc(sizeof(huff_queue_t));
    new_queue->head = NULL;
    new_queue->size = 0;
    return new_queue;
}

void huff_enqueue(huff_queue_t *hq, void *byte, uint64_t frequency,
                  huff_node_t *left, huff_node_t *right)
{
    huff_node_t *new_node = malloc(sizeof(huff_node_t));
    new_node->byte = byte;
    new_node->frequency = frequency;
    new_node->left = left;
    new_node->right = right;

    if (hq->head == NULL)
    {
        new_node->next = NULL;
        hq->head = new_node;
    }
    else
    {
        if (frequency < hq->head->frequency)
        {
            new_node->next = hq->head;
            hq->head = new_node;
        }
        else
        {
            huff_node_t *current = hq->head;
            while (current->next != NULL &&
                   frequency > current->next->frequency)
            {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    }
    hq->size += 1;
}

huff_node_t *huff_dequeue(huff_queue_t *hq)
{
    if (hq->head == NULL)
    {
        printf("The queue is empty.\n");
        return NULL;
    }

    huff_node_t *dequeded = hq->head;

    hq->head = hq->head->next;

    hq->size -= 1;

    return dequeded;
}

huff_node_t *huff_get_tree_from_queue(huff_queue_t *hq)
{
    while (hq->size != 1)
    {
        huff_node_t *left = huff_dequeue(hq);
        huff_node_t *right = huff_dequeue(hq);

        uint8_t *internal_node = malloc(sizeof(uint8_t));
        *internal_node = 42;
        huff_enqueue(hq, (void *)internal_node, left->frequency + right->frequency, left, right);
    }

    return hq->head;
}

uint16_t huff_get_tree_size(huff_node_t *ht)
{
    if (ht != NULL)
    {
        uint16_t left_size = huff_get_tree_size(ht->left);
        uint16_t right_size = huff_get_tree_size(ht->right);

        if (is_special_node(ht))
        {
            return 2 + left_size + right_size;
        }
        else
        {
            return 1 + left_size + right_size;
        }
    }
    return 0;
}

void huff_write_tree_to_file(huff_node_t *ht, FILE *output)
{
    if (ht != NULL)
    {
        if (is_special_node(ht))
        {
            uint8_t c = '\\';
            fwrite(&c, sizeof(uint8_t), 1, output);
        }

        fwrite((uint8_t *)ht->byte, sizeof(uint8_t), 1, output);

        huff_write_tree_to_file(ht->left, output);
        huff_write_tree_to_file(ht->right, output);
    }
}

void huff_print_tree(huff_node_t *ht)
{
    if (ht != NULL)
    {
        if (is_special_node(ht))
        {
            printf("%d", '\\');
        }

        printf("%d", *(uint8_t *)ht->byte);

        huff_print_tree(ht->left);
        huff_print_tree(ht->right);
    }
}