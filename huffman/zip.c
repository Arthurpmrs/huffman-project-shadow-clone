#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "libs/linked_list.h"
#include "libs/stack.h"
#include "libs/huffman.h"
#include "libs/utils.c"

#define DEBUG 1
#define MAX_FILENAME_SIZE 256

typedef struct byte_info byte_info_t;
struct byte_info
{
    uint8_t byte;
    uint64_t frequency;
    list_t *zipped_bits;
};

bool get_zipped_path(char zipped[MAX_FILENAME_SIZE], char file[MAX_FILENAME_SIZE - 5])
{
    strcpy(zipped, file);
    strcat(zipped, ".huff");
}

void init_byte_info_array(byte_info_t bytes[256])
{
    for (int i = 0; i < 256; i++)
    {
        bytes[i].byte = i;
        bytes[i].frequency = 0;
        bytes[i].zipped_bits = list_create(print_byte_int);
    }
}

bool get_byte_frequencies(FILE *input, byte_info_t bytes[256])
{
    if (input == NULL)
    {
        return false;
    }

    uint8_t byte;
    while (fread(&byte, sizeof(uint8_t), 1, input) != 0)
    {
        bytes[byte].frequency += 1;
    }

    return true;
}

int compare_int(void *a, void *b)
{
    return *(uint64_t *)a - *(uint64_t *)b;
}

huff_node_t *huffmanizeQ(huff_heap_t *heap)
{
    while (heap->size != 1)
    {
        huff_node_t *left = hp_dequeue(heap);
        huff_node_t *right = hp_dequeue(heap);

        uint8_t *internal_node = malloc(sizeof(uint8_t));
        *internal_node = 42;
        hp_enqueue(heap, (void *)internal_node, left->frequency + right->frequency, left, right);
    }

    return heap->data[1];

    // if (huff->head->next == NULL)
    // {
    //     return deQ(huff);
    // }

    // HuffNode *left = deQ(huff);
    // HuffNode *right = deQ(huff);

    // enQ(huff, NULL, left->priority + right->priority, left, right);
    // huffmanizeQ(huff);
}

void preorder(huff_node_t *ht)
{
    if (ht != NULL)
    {
        printf("%d ", *(uint8_t *)ht->item);
        preorder(ht->left);
        preorder(ht->right);
    }
}

void stack_push_bit(stack_t *stack, uint8_t bit_value)
{
    uint8_t *bit = malloc(sizeof(uint8_t));
    *bit = bit_value;
    stack_push(stack, (void *)bit);
}

// void list_add_bit_to_head(list_t *list, uint8_t bit_value)
// {
//     uint8_t *bit = malloc(sizeof(uint8_t));
//     *bit = bit_value;
//     list_add_to_head(list, (void *)bit);
// }

void stack_copy_to_list(stack_t *stack, list_t *list)
{
    stack_node_t *current = stack->top;
    while (current != NULL)
    {
        list_add_to_head(list, current->item);
        current = current->next;
    }
}

void get_zipped_bits(huff_node_t *ht, byte_info_t bytes[256], stack_t *aux_stack)
{
    if (ht->left == NULL && ht->right == NULL)
    {
        // printf("%d (%c) => ", *(uint8_t *)ht->item, *(uint8_t *)ht->item);
        // stack_print(aux_stack);
        stack_copy_to_list(aux_stack, bytes[*(uint8_t *)ht->item].zipped_bits);
    }
    else
    {
        stack_push_bit(aux_stack, 0);
        get_zipped_bits(ht->left, bytes, aux_stack);
        stack_pop(aux_stack);

        stack_push_bit(aux_stack, 1);
        get_zipped_bits(ht->right, bytes, aux_stack);
        stack_pop(aux_stack);
    }
}

int main(void)
{
    char file_path[MAX_FILENAME_SIZE - 5];
    if (DEBUG)
    {
        strcpy(file_path, "examples/slide.txt");
    }
    else
    {
        printf("Enter the name of the file to be compressed (relative to cwd): ");
        scanf("%s", file_path);
    }

    FILE *input = fopen(file_path, "rb");
    if (!input)
    {
        printf("File \"%s\" could not be found or accessed.\n", file_path);
        exit(EXIT_FAILURE);
    }

    byte_info_t bytes[256];
    init_byte_info_array(bytes);
    get_byte_frequencies(input, bytes);

    // if (DEBUG)
    // {
    //     for (int i = 0; i < 256; i++)
    //     {
    //         if (bytes[i].frequency != 0)
    //             printf("byte=%d (%c) (freq=%d)\n", bytes[i].byte, bytes[i].byte, bytes[i].frequency);
    //     }
    // }

    huff_heap_t *hh = hp_create(260, print_byte, compare_int);
    for (int i = 0; i < 256; i++)
    {
        if (bytes[i].frequency != 0)
        {
            hp_enqueue(hh, (void *)&bytes[i].byte, bytes[i].frequency, NULL, NULL);
        }
    }

    // hp_print(hh);

    // printf("-----------------\n");
    // while (hh->size != 0)
    // {
    //     huff_node_t *deq = hp_dequeue(hh);
    //     // printf("(freq=%lld) byte=%c\n", deq->frequency, *(uint8_t *)deq->item);
    //     printf("%d\n", *(uint8_t *)deq->item);
    // }
    huff_node_t *root = huffmanizeQ(hh);
    preorder(root);

    printf("\n");
    stack_t *aux_stack = stack_create(print_byte_int);
    get_zipped_bits(root, bytes, aux_stack);

    for (int i = 0; i < 256; i++)
    {
        if (bytes[i].frequency != 0)
        {
            printf("%d (%c) => ", bytes[i].byte, bytes[i].byte);
            list_print(bytes[i].zipped_bits);
        }
    }

    // printf("\nsize=%d\n", hh->size);

    // char zipped_path[MAX_FILENAME_SIZE];
    // get_zipped_path(zipped_path, file_path);
    // printf("new->%s\n", zipped_path);
    fclose(input);

    return 0;
}