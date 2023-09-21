#include "test_linked_list.h"

void tll_print_int(void *item)
{
    printf("%d ", *(int *)item);
}

// Test suite initialization function
int tll_init_suite(void)
{
    return 0; // Return 0 on success
}

// Test suite cleanup function
int tll_clean_suite(void)
{
    return 0; // Return 0 on success
}

void tll_test_create_empty_linked_list(void)
{
    list_t *list = list_create(tll_print_int);

    CU_ASSERT_PTR_NOT_NULL_FATAL(list)
    CU_ASSERT_PTR_NULL(list->head);
    CU_ASSERT_PTR_NULL(list->tail);
    CU_ASSERT_EQUAL(list->size, 0);
}

void tll_test_list_is_empty(void)
{
    list_t *list = list_create(tll_print_int);

    CU_ASSERT_TRUE(list_is_empty(list));
}

void tll_test_list_add_to_head(void)
{
    list_t *list = list_create(tll_print_int);

    int value = 12;
    list_add_to_head(list, (void *)&value);

    CU_ASSERT_EQUAL(*(int *)list->head->item, 12);
    CU_ASSERT_EQUAL(*(int *)list->tail->item, 12);
    CU_ASSERT_EQUAL(list->size, 1);

    int value_2 = 100;
    list_add_to_head(list, (void *)&value_2);

    CU_ASSERT_EQUAL(*(int *)list->head->item, 100);
    CU_ASSERT_EQUAL(*(int *)list->tail->item, 12);
    CU_ASSERT_EQUAL(list->size, 2);
}

void tll_test_list_add_to_tail(void)
{
    list_t *list = list_create(tll_print_int);

    int value = 75;
    list_add_to_tail(list, (void *)&value);

    CU_ASSERT_EQUAL(*(int *)list->head->item, 75);
    CU_ASSERT_EQUAL(*(int *)list->tail->item, 75);
    CU_ASSERT_EQUAL(list->size, 1);

    int value_2 = 256;
    list_add_to_tail(list, (void *)&value_2);

    CU_ASSERT_EQUAL(*(int *)list->head->item, 75);
    CU_ASSERT_EQUAL(*(int *)list->tail->item, 256);
    CU_ASSERT_EQUAL(list->size, 2);
}

void tll_test_list_delete_from_head(void)
{
    list_t *list = list_create(tll_print_int);

    int value = 12;
    list_add_to_head(list, (void *)&value);

    int value_2 = 100;
    list_add_to_head(list, (void *)&value_2);

    int value_3 = 256;
    list_add_to_head(list, (void *)&value_3);

    CU_ASSERT_EQUAL(list->size, 3);
    CU_ASSERT_EQUAL(*(int *)list_delete_from_head(list), 256);
    CU_ASSERT_EQUAL(list->size, 2);
    CU_ASSERT_EQUAL(*(int *)list_delete_from_head(list), 100);
    CU_ASSERT_EQUAL(list->size, 1);
    CU_ASSERT_EQUAL(*(int *)list_delete_from_head(list), 12);
    CU_ASSERT_EQUAL(list->size, 0);
}

void tll_test_list_delete(void)
{
    list_t *list = list_create(tll_print_int);

    int value = 12;
    list_add_to_head(list, (void *)&value);

    int value_2 = 100;
    list_add_to_head(list, (void *)&value_2);

    int value_3 = 256;
    list_add_to_head(list, (void *)&value_3);

    CU_ASSERT_EQUAL(list->size, 3);
    CU_ASSERT_TRUE(list_delete(list));
    CU_ASSERT_EQUAL(list->size, 0);
}