#include "test_stack.h"

void ts_print_int(void *item)
{
    printf("%d ", *(int *)item);
}

int ts_init_suite(void)
{
    return 0;
}

int ts_clean_suite(void)
{
    return 0;
}

void ts_test_create_empty_stack(void)
{
    stack_t *stack = stack_create(ts_print_int);

    CU_ASSERT_PTR_NOT_NULL_FATAL(stack)
    CU_ASSERT_PTR_NULL(stack->top);
    CU_ASSERT_EQUAL(stack->size, 0);
}

void ts_test_stack_is_empty(void)
{
    stack_t *stack = stack_create(ts_print_int);

    CU_ASSERT_TRUE(stack_is_empty(stack));
}

void ts_test_stack_push(void)
{
    stack_t *stack = stack_create(ts_print_int);

    int value = 12;
    stack_push(stack, (void *)&value);

    CU_ASSERT_EQUAL(*(int *)stack->top->item, 12);
    CU_ASSERT_EQUAL(stack->size, 1);

    int value_2 = 100;
    stack_push(stack, (void *)&value_2);

    CU_ASSERT_EQUAL(*(int *)stack->top->item, 100);
    CU_ASSERT_EQUAL(stack->size, 2);
}

void ts_test_stack_pop(void)
{
    stack_t *stack = stack_create(ts_print_int);

    int value = 12;
    stack_push(stack, (void *)&value);

    int value_2 = 100;
    stack_push(stack, (void *)&value_2);

    int value_3 = 200;
    stack_push(stack, (void *)&value_3);

    CU_ASSERT_EQUAL(*(int *)stack_pop(stack), 200);
    CU_ASSERT_EQUAL(*(int *)stack_pop(stack), 100);
    CU_ASSERT_EQUAL(*(int *)stack_pop(stack), 12);
    CU_ASSERT_EQUAL(stack->size, 0);
}

void ts_test_stack_pop_is_empty(void)
{
    stack_t *stack = stack_create(ts_print_int);
    CU_ASSERT_PTR_NULL(stack_pop(stack));
}

void ts_test_stack_peek(void)
{
    stack_t *stack = stack_create(ts_print_int);

    int value = 12;
    stack_push(stack, (void *)&value);

    int value_2 = 100;
    stack_push(stack, (void *)&value_2);

    int value_3 = 200;
    stack_push(stack, (void *)&value_3);

    CU_ASSERT_EQUAL(*(int *)stack_peek(stack), 200);
    CU_ASSERT_EQUAL(stack->size, 3);
}

void ts_test_stack_peek_is_empty(void)
{
    stack_t *stack = stack_create(ts_print_int);

    CU_ASSERT_PTR_NULL(stack_peek(stack));
}