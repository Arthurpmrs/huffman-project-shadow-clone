#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "libs/linked_list.h"

#define DEBUG 1
#define MAX_FILENAME_SIZE 256

typedef struct byte_info byte_info_t;
struct byte_info
{
    uint8_t byte;
    uint64_t frequency;
    list_t *mapped_bits;
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
        bytes[i].mapped_bits = NULL;
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
        printf("%d\n", byte);
        bytes[byte].frequency += 1;
    }

    return true;
}

int main(void)
{
    char file_path[MAX_FILENAME_SIZE - 5];
    if (DEBUG)
    {
        strcpy(file_path, "examples/kobayashi.jpg");
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

    if (DEBUG)
    {
        for (int i = 0; i < 256; i++)
        {
            printf("byte=%d (freq=%d)\n", bytes[i].byte, bytes[i].frequency);
        }
    }

    char zipped_path[MAX_FILENAME_SIZE];
    get_zipped_path(zipped_path, file_path);
    printf("new->%s\n", zipped_path);
    fclose(input);

    return 0;
}