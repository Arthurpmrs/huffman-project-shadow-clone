#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "libs/linked_list.h"
#include "libs/stack.h"
#include "libs/huffman.h"
#include "libs/utils.c"

#define DEBUG 0
#define MAX_FILENAME_SIZE 1024
#define TMP_FILE_NAME "tmp"

typedef struct byte_info byte_info_t;
struct byte_info
{
    uint8_t byte;
    uint64_t frequency;
    list_t *huff_bit_sequence;
};

void get_zipped_path(char zipped[MAX_FILENAME_SIZE], char file[MAX_FILENAME_SIZE - 5])
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
        bytes[i].huff_bit_sequence = list_create(print_byte_int);
    }
}

void print_byte_info(byte_info_t bytes[256])
{
    printf("--------------- Byte Info Array ---------------\n");
    for (int i = 0; i < 256; i++)
    {
        if (bytes[i].frequency == 0)
        {
            continue;
        }

        printf("Byte %d (Freq=%ld) | ", bytes[i].byte, bytes[i].frequency);
        printf("bit_sequence=");
        list_node_t *current = bytes[i].huff_bit_sequence->head;
        while (current != NULL)
        {
            printf("%d", *(uint8_t *)current->item);
            current = current->next;
        }
        printf("\n");
    }
    printf("\n");
}

void get_byte_frequencies(FILE *input, byte_info_t bytes[256])
{
    uint8_t byte;
    while (fread(&byte, sizeof(uint8_t), 1, input) != 0)
    {
        bytes[byte].frequency += 1;
    }
}

huff_node_t *get_huff_tree(byte_info_t bytes[256])
{
    huff_queue_t *hq = huff_create_queue();

    // Populate the Huffman priority queue
    for (int i = 0; i < 256; i++)
    {
        if (bytes[i].frequency != 0)
        {
            huff_enqueue(hq, (void *)&bytes[i].byte, bytes[i].frequency, NULL, NULL);
        }
    }

    return huff_get_tree_from_queue(hq);
}

void get_bit_sequences(huff_node_t *ht, byte_info_t bytes[256], stack_t *aux_stack)
{
    if (ht->left == NULL && ht->right == NULL)
    {
        stack_copy_to_list(aux_stack, bytes[*(uint8_t *)ht->byte].huff_bit_sequence);
    }
    else
    {
        stack_push_bit(aux_stack, 0);
        get_bit_sequences(ht->left, bytes, aux_stack);
        stack_pop(aux_stack);

        stack_push_bit(aux_stack, 1);
        get_bit_sequences(ht->right, bytes, aux_stack);
        stack_pop(aux_stack);
    }
}

bool zip_to_tmp_file(FILE *input, byte_info_t bytes[256], uint8_t *trash_size)
{
    if (input == NULL)
    {
        return false;
    }

    FILE *tmp = fopen(TMP_FILE_NAME, "wb");

    uint8_t byte = 0;
    uint8_t compressed_byte = 0;
    uint8_t compressed_byte_index = 7;
    while (fread(&byte, sizeof(uint8_t), 1, input) != 0)
    {
        list_node_t *current_huff_bit = bytes[byte].huff_bit_sequence->head;
        while (current_huff_bit != NULL)
        {
            // Check if the bit needs to be set
            if (*(uint8_t *)current_huff_bit->item)
            {
                compressed_byte = set_bit(compressed_byte, compressed_byte_index);
            }

            // Check if the current byte ended
            if (compressed_byte_index == 0)
            {
                fwrite(&compressed_byte, sizeof(uint8_t), 1, tmp);
                compressed_byte = 0;
                compressed_byte_index = 7;
            }
            else
            {
                compressed_byte_index -= 1;
            }

            current_huff_bit = current_huff_bit->next;
        }
    }

    // Checks if there is any trash
    if (compressed_byte_index < 7)
    {
        fwrite(&compressed_byte, sizeof(uint8_t), 1, tmp);
        *trash_size = compressed_byte_index + 1;
    }

    fclose(tmp);
    return true;
}

bool write_header_to_file(FILE *output, huff_node_t *ht, uint8_t trash_size)
{
    if (output == NULL || ht == NULL)
    {
        return false;
    }

    uint16_t tree_size = huff_get_tree_size(ht);

    uint8_t header_size_bytes[2];
    header_size_bytes[0] = trash_size << 5 | tree_size >> 8;
    header_size_bytes[1] = tree_size;
    fwrite(&header_size_bytes, sizeof(uint8_t), 2, output);

    huff_write_tree_to_file(ht, output);

    return true;
}

bool copy_bytes_from_tmp_file(FILE *output)
{
    if (output == NULL)
    {
        return false;
    }

    FILE *tmp = fopen(TMP_FILE_NAME, "rb");

    uint8_t byte = 0;
    while (fread(&byte, sizeof(uint8_t), 1, tmp) != 0)
    {
        fwrite(&byte, sizeof(uint8_t), 1, output);
    }

    fclose(tmp);

    return true;
}

bool zip(FILE *input, char zipped[MAX_FILENAME_SIZE], huff_node_t *ht, byte_info_t bytes[256])
{
    if (input == NULL || ht == NULL)
    {
        return false;
    }

    // Reset input file pointer
    fseek(input, 0, SEEK_SET);

    uint8_t trash_size = 0;
    if (!zip_to_tmp_file(input, bytes, &trash_size))
    {
        return false;
    }

    FILE *output = fopen(zipped, "wb");

    write_header_to_file(output, ht, trash_size);
    copy_bytes_from_tmp_file(output);

    fclose(output);

    return true;
}

int main(void)
{
    // ############# Read path of file to be zipped #############
    char file_path[MAX_FILENAME_SIZE - 5];
    if (DEBUG)
    {
        // strcpy(file_path, "examples/bocchi.jpg");
        strcpy(file_path, "examples/slide.txt");
    }
    else
    {
        printf("Enter the name of the file to be compressed (relative to cwd): ");
        scanf("%s", file_path);
    }

    // ############# Open file #############
    FILE *input = fopen(file_path, "rb");
    if (!input)
    {
        printf("File \"%s\" could not be found or accessed.\n", file_path);
        exit(EXIT_FAILURE);
    }

    // ############# Count byte frequencies #############
    byte_info_t bytes[256];
    init_byte_info_array(bytes);
    get_byte_frequencies(input, bytes);

    // ############# Create Huffman Tree #############
    huff_node_t *ht = get_huff_tree(bytes);
    if (DEBUG)
    {
        printf("\n------------ Huff Tree (Preorder) ------------\n");
        huff_print_tree(ht);
        printf("\ntree_size=%d\n\n", huff_get_tree_size(ht));
    }

    // ############# Get Huffman bit sequence for every byte #############
    stack_t *aux_stack = stack_create(print_byte_int);
    get_bit_sequences(ht, bytes, aux_stack);

    if (DEBUG)
    {
        print_byte_info(bytes);
    }

    // ############# Zip to file #############
    char zipped_path[MAX_FILENAME_SIZE];
    get_zipped_path(zipped_path, file_path);

    if (!zip(input, zipped_path, ht, bytes))
    {
        printf("Something went wrong in the zipping process.\n");
        exit(EXIT_FAILURE);
    }

    printf("[SUCCESS] File %s zipped to %s\n", file_path, zipped_path);

    fclose(input);
    return 0;
}