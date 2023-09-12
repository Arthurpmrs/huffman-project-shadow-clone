#include "linked_list.h"

list_t *list_create(void (*print)(void *))
{
    list_t *new_list = malloc(sizeof(list_t));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->size = 0;
    new_list->print = print;

    return new_list;
}

bool list_is_empty(list_t *list)
{
    return list->head == NULL;
}

bool list_add_to_head(list_t *list, void *item)
{
    if (list == NULL)
    {
        return false;
    }

    list_node_t *new_node = malloc(sizeof(list_node_t));
    new_node->item = item;

    if (list_is_empty(list))
    {
        list->head = new_node;
        list->tail = new_node;
        new_node->next = NULL;
    }
    else
    {
        new_node->next = list->head;
        list->head = new_node;
    }
    list->size += 1;
    return true;
}

bool list_add_to_tail(list_t *list, void *item)
{
    if (list == NULL)
    {
        return false;
    }

    list_node_t *new_node = malloc(sizeof(list_node_t));
    new_node->item = item;
    new_node->next = NULL;

    if (list_is_empty(list))
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size += 1;
    return true;
}

void *list_delete_from_head(list_t *list)
{
    if (list_is_empty(list))
    {
        printf("List is empty.\n");
        return NULL;
    }

    list_node_t *deletion_p = list->head;
    void *item = list->head->item;

    if (list->head == list->tail)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        list->head = list->head->next;
    }

    free(deletion_p);
    list->size -= 1;

    return item;
}

bool list_delete(list_t *list)
{
    if (list == NULL)
    {
        return false;
    }

    while (list->size != 0)
    {
        list_delete_from_head(list);
    }

    return true;
}

bool list_print(list_t *list)
{
    if (list_is_empty(list))
    {
        printf("List is empty.\n");
        return false;
    }

    list_node_t *current = list->head;

    while (current != NULL)
    {
        list->print(current->item);
        current = current->next;
    }
    printf("\n");

    return true;
}
