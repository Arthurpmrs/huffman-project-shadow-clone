#ifndef TEST_STACK_
#define TEST_STACK_

#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../libs/stack.h"

int ts_init_suite(void);

int ts_clean_suite(void);

void ts_test_create_empty_stack(void);

void ts_test_stack_is_empty(void);

void ts_test_stack_push(void);

void ts_test_stack_pop(void);

void ts_test_stack_pop_is_empty(void);

void ts_test_stack_peek(void);

void ts_test_stack_peek_is_empty(void);

#endif // TEST_STACK_