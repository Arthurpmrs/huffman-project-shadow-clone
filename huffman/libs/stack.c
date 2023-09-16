#include "stack.h"

stack_t *stack_create(void (*print)(void *))
{
    stack_t *new_stack = malloc(sizeof(stack_t));
    new_stack->top = NULL;
    new_stack->size = 0;
    new_stack->print = print;
    return new_stack;
}

int stack_is_empty(stack_t *stack)
{
    return stack->top == NULL;
}

void stack_push(stack_t *stack, void *item)
{
    stack_node_t *new_node = malloc(sizeof(stack_node_t));
    new_node->item = item;

    if (stack_is_empty(stack))
    {
        stack->top = new_node;
        new_node->next = NULL;
    }
    else
    {
        new_node->next = stack->top;
        stack->top = new_node;
    }

    stack->size += 1;
}

void *stack_pop(stack_t *stack)
{
    if (stack_is_empty(stack))
    {
        printf("List is empty.\n");
        return NULL;
    }

    stack_node_t *deletion_p = stack->top;
    void *item = stack->top->item;

    stack->top = stack->top->next;

    free(deletion_p);
    stack->size -= 1;

    return item;
}

void *stack_peek(stack_t *stack)
{
    return stack->top->item;
}

void stack_print(stack_t *stack)
{
    stack_node_t *current = stack->top;
    while (current != NULL)
    {
        stack->print(current->item);
        current = current->next;
    }
    printf("\n");
}