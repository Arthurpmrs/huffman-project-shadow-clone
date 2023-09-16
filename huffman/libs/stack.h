#ifndef HUFFMAN_STACK_H_
#define HUFFMAN_STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct stack_node stack_node_t;
struct stack_node
{
    void *item;
    stack_node_t *next;
};
typedef struct stack stack_t;
struct stack
{
    stack_node_t *top;
    uint32_t size;
    void (*print)(void *);
};

stack_t *stack_create(void (*print)(void *));

int stack_is_empty(stack_t *stack);

void stack_push(stack_t *stack, void *item);

void *stack_pop(stack_t *stack);

void *stack_peek(stack_t *stack);

void stack_print(stack_t *stack);

#endif // HUFFMAN_STACK_H_