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

huff_node_t *huffmanize(huff_queue_t *hq)
{
    while (hq->size != 1)
    {
        // printf("--------------------(size=%d) byte=%d\n", hq->size, *(uint8_t *)hq->head->byte);
        huff_node_t *left = huff_dequeue(hq);
        // printf("    size=%d\n", hq->size);
        huff_node_t *right = huff_dequeue(hq);
        // printf("    size=%d\n", hq->size);

        uint8_t *internal_node = malloc(sizeof(uint8_t));
        *internal_node = 42;
        huff_enqueue(hq, (void *)internal_node, left->frequency + right->frequency, left, right);
        // printf("    size=%d\n", hq->size);
        // printf("  OUTSIDE>>>size(%d) head=%p tail=%p\n", hq->size, hq->head, NULL);
    }

    return hq->head;

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
        printf("%d\n", *(uint8_t *)ht->byte);
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
        stack_copy_to_list(aux_stack, bytes[*(uint8_t *)ht->byte].zipped_bits);
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

huff_node_t *create_empty_binary_tree()
{
    huff_node_t *bt = malloc(sizeof(huff_node_t));
    bt->left = NULL;
    bt->right = NULL;
    return bt;
}

huff_node_t *create_binary_tree(void *item, huff_node_t *left, huff_node_t *right)
{
    huff_node_t *bt = create_empty_binary_tree();
    bt->left = left;
    bt->right = right;
    bt->byte = item;
    return bt;
}

huff_node_t *mock_tree()
{
    uint8_t *a = malloc(sizeof(uint8_t));
    *a = 'A';
    uint8_t *b = malloc(sizeof(uint8_t));
    *b = 'B';
    uint8_t *c = malloc(sizeof(uint8_t));
    *c = 'C';
    uint8_t *d = malloc(sizeof(uint8_t));
    *d = 'D';
    uint8_t *e = malloc(sizeof(uint8_t));
    *e = 'E';
    uint8_t *f = malloc(sizeof(uint8_t));
    *f = 'F';
    uint8_t *ast = malloc(sizeof(uint8_t));
    *ast = '*';
    return create_binary_tree(
        (void *)ast,
        create_binary_tree(
            (void *)ast,
            create_binary_tree((void *)c, NULL, NULL),
            create_binary_tree((void *)b, NULL, NULL)),
        create_binary_tree(
            (void *)ast,
            create_binary_tree(
                (void *)ast,
                create_binary_tree(
                    (void *)ast,
                    create_binary_tree((void *)f, NULL, NULL),
                    create_binary_tree((void *)e, NULL, NULL)),
                create_binary_tree((void *)d, NULL, NULL)),
            create_binary_tree((void *)a, NULL, NULL)));
}

bool zip_to_tmp_file(FILE *input, uint8_t *trash_size, byte_info_t bytes[256])
{
    if (input == NULL)
    {
        return false;
    }

    FILE *tmp = fopen("tmp", "wb");

    uint8_t byte = 0;
    uint8_t compressed_byte = 0;
    uint8_t compressed_bit_index = 7;
    while (fread(&byte, sizeof(uint8_t), 1, input) != 0)
    {
        printf("%c\n", byte);
        list_node_t *current_zipped_bit = bytes[byte].zipped_bits->head;
        while (current_zipped_bit != NULL)
        {
            printf("    (z_bit=%d) | bit_index=%d\n", *(uint8_t *)current_zipped_bit->item, compressed_bit_index);
            if (*(uint8_t *)current_zipped_bit->item == 1)
            {
                compressed_byte = set_bit(compressed_byte, compressed_bit_index);
            }
            printf("    byte=");
            print_as_bin(compressed_byte, 8);

            if (compressed_bit_index == 0)
            {
                fwrite(&compressed_byte, sizeof(uint8_t), 1, tmp);
                compressed_bit_index = 7;
                compressed_byte = 0;
            }
            else
            {
                compressed_bit_index -= 1;
            }

            current_zipped_bit = current_zipped_bit->next;
        }
    }

    if (compressed_bit_index < 7)
    {
        fwrite(&compressed_byte, sizeof(uint8_t), 1, tmp);
    }

    printf("compressed_bit_index = %d\n", compressed_bit_index);
    printf("trash = %d\n", compressed_bit_index + 1);
    *trash_size = compressed_bit_index + 1;
    fclose(tmp);
    return true;
}

bool set_header(FILE *output, huff_node_t *ht, uint8_t trash_size)
{
    uint16_t tree_size = get_huff_tree_size(ht);
    printf("trash=%d, tree_size=%d\n", trash_size, tree_size);

    uint8_t header_size_bytes[2];
    // tree_size = 590;
    // print_as_bin(tree_size, 16);
    header_size_bytes[0] = trash_size << 5 | tree_size >> 8;
    header_size_bytes[1] = tree_size;

    // uint16_t size_bytes = trash_size << 13 | tree_size;
    fwrite(&header_size_bytes, sizeof(uint8_t), 2, output);
    // print_as_bin(size_bytes, 16);

    write_preorder_huff_tree(ht, output);
    return true;
}

bool zip(FILE *input, char zipped_path[MAX_FILENAME_SIZE], huff_node_t *ht, byte_info_t bytes[256])
{
    if (input == NULL || ht == NULL)
    {
        return false;
    }

    fseek(input, 0, SEEK_SET);
    FILE *output = fopen(zipped_path, "wb");

    uint8_t trash_size = 0;

    zip_to_tmp_file(input, &trash_size, bytes);

    set_header(output, ht, trash_size);

    FILE *tmp_file = fopen("tmp", "rb");
    uint8_t byte = 0;
    while (fread(&byte, sizeof(uint8_t), 1, tmp_file) != 0)
    {
        fwrite(&byte, sizeof(uint8_t), 1, output);
    }

    fclose(tmp_file);
    fclose(output);
    return true;
}

void print_preorder_huff_tree(huff_node_t *ht)
{
    if (ht != NULL)
    {
        if (ht->left == NULL & ht->right == NULL)
        {
            if (*(uint8_t *)ht->byte == '\\' || *(uint8_t *)ht->byte == '*')
            {
                uint8_t c = '\\';
                printf("%d\n", c);
            }

            printf("%d\n", *(uint8_t *)ht->byte);
        }
        else
        {
            printf("%d\n", *(uint8_t *)ht->byte);
        }

        print_preorder_huff_tree(ht->left);
        print_preorder_huff_tree(ht->right);
    }
}

int main(void)
{
    char file_path[MAX_FILENAME_SIZE - 5];
    if (DEBUG)
    {
        strcpy(file_path, "examples/bocchi.jpg");
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
    huff_queue_t *hq = huff_create_queue();
    for (int i = 0; i < 256; i++)
    {
        if (bytes[i].frequency != 0)
        {
            huff_enqueue(hq, (void *)&bytes[i].byte, bytes[i].frequency, NULL, NULL);
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
    // huff_node_t *current = hq->head;
    // while (current != NULL)
    // {
    //     printf("%d (%c) : %lld\n", *(uint8_t *)current->byte, *(uint8_t *)current->byte, current->frequency);
    //     current = current->next;
    // }
    // printf("size=%d\n", hq->size);
    huff_node_t *root = huffmanize(hq);
    // huff_node_t *root = mock_tree();
    // preorder(root);
    print_preorder_huff_tree(root);

    printf("\n");
    stack_t *aux_stack = stack_create(print_byte_int);
    get_zipped_bits(root, bytes, aux_stack);

    // for (int i = 0; i < 256; i++)
    // {
    //     if (bytes[i].frequency != 0)
    //     {
    //         printf("%d (%c) => ", bytes[i].byte, bytes[i].byte);
    //         list_print(bytes[i].zipped_bits);
    //     }
    // }

    char zipped_path[MAX_FILENAME_SIZE];
    get_zipped_path(zipped_path, file_path);
    printf("new->%s\n", zipped_path);
    zip(input, zipped_path, root, bytes);

    // printf("\nsize=%d\n", hh->size);

    fclose(input);

    return 0;
}