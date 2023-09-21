#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test_binary_tree.h"
#include "test_linked_list.h"
#include "test_stack.h"

CU_ErrorCode tbt_setup_test(void)
{
    CU_pSuite suite = CU_add_suite("Binary Tree Test", tbt_init_suite,
                                   tbt_clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test case to the suite
    if (CU_add_test(suite, "1", tbt_test_create_empty_binary_tree) == NULL ||
        CU_add_test(suite, "2", tbt_test_create_binary_tree) == NULL ||
        CU_add_test(suite, "3", tbt_test_reconstruct_tree) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

CU_ErrorCode tll_setup_test(void)
{
    CU_pSuite suite = CU_add_suite("Linked List Test", tll_init_suite,
                                   tll_clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test case to the suite
    if (CU_add_test(suite, "1", tll_test_create_empty_linked_list) == NULL ||
        CU_add_test(suite, "2", tll_test_list_is_empty) == NULL ||
        CU_add_test(suite, "3", tll_test_list_add_to_head) == NULL ||
        CU_add_test(suite, "4", tll_test_list_add_to_tail) == NULL ||
        CU_add_test(suite, "5", tll_test_list_delete_from_head) == NULL ||
        CU_add_test(suite, "6", tll_test_list_delete) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

CU_ErrorCode ts_setup_test(void)
{
    CU_pSuite suite = CU_add_suite("Stack Test", ts_init_suite,
                                   ts_clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test case to the suite
    if (CU_add_test(suite, "1", ts_test_create_empty_stack) == NULL ||
        CU_add_test(suite, "2", ts_test_stack_is_empty) == NULL ||
        CU_add_test(suite, "3", ts_test_stack_push) == NULL ||
        CU_add_test(suite, "4", ts_test_stack_pop) == NULL ||
        CU_add_test(suite, "5", ts_test_stack_pop_is_empty) == NULL ||
        CU_add_test(suite, "6", ts_test_stack_peek) == NULL ||
        CU_add_test(suite, "7", ts_test_stack_peek_is_empty) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

int main()
{
    // Initialize the CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        printf("Initialization of CUnit Test Registry failed.");
        return CU_get_error();
    }

    if (tbt_setup_test() != CUE_SUCCESS)
    {
        printf("Setup of Binary Tree Test Suite failed.");
        return CU_get_error();
    }

    if (tll_setup_test() != CUE_SUCCESS)
    {
        printf("Setup of Linked List Test Suite failed.");
        return CU_get_error();
    }

    if (ts_setup_test() != CUE_SUCCESS)
    {
        printf("Setup of Stack Test Suite failed.");
        return CU_get_error();
    }

    // Run the tests
    CU_basic_run_tests();

    // Cleanup and return the test status
    CU_cleanup_registry();
    return CU_get_error();
}