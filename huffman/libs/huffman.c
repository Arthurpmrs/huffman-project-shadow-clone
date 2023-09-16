#include "huffman.h"

void swap(huff_node_t **a, huff_node_t **b)
{
    huff_node_t *tmp = *a;
    *a = *b;
    *b = tmp;
}

huff_heap_t *hp_create(uint16_t capacity, void (*print)(void *),
                       int (*compare)(void *, void *))
{
    huff_node_t **data = malloc(capacity * sizeof(huff_node_t *));

    if (data == NULL)
    {
        exit(EXIT_FAILURE);
    }

    huff_heap_t *new_heap = malloc(sizeof(huff_heap_t));
    new_heap->size = 0;
    new_heap->capacity = capacity;
    new_heap->data = data;
    new_heap->compare = compare;
    new_heap->print = print;
    return new_heap;
}

huff_node_t *huff_create_node(void *item, uint64_t frequency, huff_node_t *left, huff_node_t *right)
{
    huff_node_t *new_node = malloc(sizeof(huff_node_t));
    new_node->item = item;
    new_node->frequency = frequency;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

uint32_t hp_get_parent_index(uint32_t i)
{
    return i >> 1;
}

uint32_t hp_get_left_index(uint32_t i)
{
    return i << 1;
}

uint32_t hp_get_right_index(uint32_t i)
{
    return (i << 1) + 1;
}

huff_node_t *hp_item_of(huff_heap_t *heap, uint32_t i)
{
    return heap->data[i];
}

void hp_enqueue(huff_heap_t *heap, void *item, uint64_t frequency, huff_node_t *left, huff_node_t *right)
{
    if (heap == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (heap->size >= heap->capacity)
    {
        printf("Heap overflow.\n");
        return;
    }

    huff_node_t *new_node = huff_create_node(item, frequency, left, right);

    heap->size += 1;
    uint32_t new_item_index = heap->size;
    heap->data[new_item_index] = new_node;

    uint32_t parent_index = hp_get_parent_index(heap->size);

    while (parent_index >= 1 &&
           heap->data[new_item_index]->frequency < heap->data[parent_index]->frequency)
    {
        swap(&heap->data[new_item_index], &heap->data[parent_index]);
        new_item_index = parent_index;
        parent_index = hp_get_parent_index(new_item_index);
    }
}

void hp_min_heapify(huff_heap_t *heap, uint32_t origin)
{
    if (heap == NULL)
    {
        exit(EXIT_FAILURE);
    }

    uint32_t smallest;
    uint32_t left_index = hp_get_left_index(origin);
    uint32_t right_index = hp_get_right_index(origin);

    // printf("size=%d\n", heap->size);
    // printf("    left=%d(%d)|right=%d(%d)|origin=%d(%d)\n", left_index,
    //        heap->data[left_index]->frequency, right_index, heap->data[right_index]->frequency,
    //        origin, heap->data[origin]->frequency);

    if (left_index <= heap->size &&
        heap->data[left_index]->frequency < heap->data[origin]->frequency)
    {
        smallest = left_index;
    }
    else
    {
        smallest = origin;
    }

    // printf("            smallest=%d\n", smallest);
    if (right_index <= heap->size &&
        heap->data[right_index]->frequency < heap->data[smallest]->frequency)
    {
        smallest = right_index;
    }

    // printf("            smallest=%d\n", smallest);
    if (heap->data[origin]->frequency != heap->data[smallest]->frequency)
    {
        swap(&heap->data[origin], &heap->data[smallest]);
        hp_min_heapify(heap, smallest);
    }
}

huff_node_t *hp_dequeue(huff_heap_t *heap)
{
    // hp_print(heap);
    if (heap == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (heap->size == 0)
    {
        printf("Heap underflow.\n");
        return NULL;
    }

    huff_node_t *item = heap->data[1];
    heap->data[1] = heap->data[heap->size];
    heap->size -= 1;

    // from root
    // hp_print(heap);
    hp_min_heapify(heap, 1);
    // hp_print(heap);

    return item;
}

void hp_print(huff_heap_t *heap)
{
    for (int i = 1; i < heap->size + 1; i++)
    {
        // heap->print(heap->data[i]->item);
        printf("%lld", heap->data[i]->frequency);
        printf(" ");
    }
    printf("\n");
}

uint16_t get_huff_tree_size(huff_node_t *ht)
{
    if (ht != NULL)
    {
        uint16_t left_size = get_huff_tree_size(ht->left);
        uint16_t right_size = get_huff_tree_size(ht->right);
        return 1 + left_size + right_size;
    }
    return 0;
}

void write_preorder_huff_tree(huff_node_t *ht, FILE *output)
{
    if (ht != NULL)
    {
        if (ht->left == NULL & ht->right == NULL)
        {
            if (*(uint8_t *)ht->item == '\\' || *(uint8_t *)ht->item == '*')
            {
                uint8_t c = '\\';
                fwrite(&c, sizeof(uint8_t), 1, output);
            }

            fwrite((uint8_t *)ht->item, sizeof(uint8_t), 1, output);
        }
        else
        {
            fwrite((uint8_t *)ht->item, sizeof(uint8_t), 1, output);
        }

        write_preorder_huff_tree(ht->left, output);
        write_preorder_huff_tree(ht->right, output);
    }
}

// void *hp_clear(huff_heap_t *heap)
// {
//     free(heap->data);
//     free(heap);
//     return NULL;
// }