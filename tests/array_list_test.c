#include "rigid.h"

#include <unity.h>

#include <stddef.h>
#include <limits.h>
#include <stdio.h>

void setUp(void) {}
void tearDown(void) {}

void test_ArrayList_Create_DoesNotReturnNull(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);
    TEST_ASSERT_NOT_NULL(array_list);
}

void test_ArrayList_Create_AssignsBytesPerElementToValueProvided(void)
{
#define BYTES_PER_ELEMENT (sizeof(int))

    struct rg_ArrayList *array_list = \
            rg_ArrayList_Create(BYTES_PER_ELEMENT, 1);
    TEST_ASSERT_TRUE(array_list->_bytes_per_element == BYTES_PER_ELEMENT);

#undef BYTES_PER_ELEMENT
#define BYTES_PER_ELEMENT (sizeof(char))

    array_list = rg_ArrayList_Create(BYTES_PER_ELEMENT, 1);
    TEST_ASSERT_TRUE(array_list->_bytes_per_element == BYTES_PER_ELEMENT);

#undef BYTES_PER_ELEMENT
}

void test_ArrayList_Create_AssignsNumAllocatedElementsToInitialLength(void)
{
#define ARRAY_LIST_LENGTH 10

    struct rg_ArrayList *array_list = \
            rg_ArrayList_Create(sizeof(int), ARRAY_LIST_LENGTH);
    TEST_ASSERT_TRUE(array_list->_num_allocated_elements == ARRAY_LIST_LENGTH);

#undef ARRAY_LIST_LENGTH
#define ARRAY_LIST_LENGTH 20

    array_list = rg_ArrayList_Create(sizeof(int), ARRAY_LIST_LENGTH);
    TEST_ASSERT_TRUE(array_list->_num_allocated_elements == ARRAY_LIST_LENGTH);

#undef ARRAY_LIST_LENGTH
}

void test_ArrayList_Create_AssignsListLengthToInitialLength(void)
{
    struct rg_ArrayList *array_list;

    array_list = rg_ArrayList_Create(sizeof(int), 1);
    TEST_ASSERT_EQUAL_UINT(1, array_list->_list_length);
    rg_ArrayList_Destroy(array_list);

    array_list = rg_ArrayList_Create(sizeof(int), 3);
    TEST_ASSERT_EQUAL_UINT(3, array_list->_list_length);
    rg_ArrayList_Destroy(array_list);
}

void test_ArrayList_Create_DataIsNotNull(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);
    TEST_ASSERT_NOT_NULL(array_list->_data);
}

void test_ArrayList_Create_NumAllocatedElementsIsNeverZero(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 0);
    TEST_ASSERT_NOT_EQUAL_UINT(0, array_list->_num_allocated_elements);
    rg_ArrayList_Destroy(array_list);
}

void test_ArrayList_GetItem_DoesNotReturnNullForIndexZero(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);
    int *first_item_in_list = rg_ArrayList_GetItem(array_list, 0);
    TEST_ASSERT_NOT_NULL(first_item_in_list);
}

void test_ArrayList_GetItem_ReturnsPointerToDataInStructForIndexZero(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);
    int *first_item_in_list = rg_ArrayList_GetItem(array_list, 0);
    TEST_ASSERT_EQUAL_PTR(&array_list->_data[0], first_item_in_list);
}

void test_ArrayList_GetItem_ReturnsPointerToItemInStructForAnyIndex(void)
{
    {
        struct rg_ArrayList *array_list = \
                rg_ArrayList_Create(sizeof(int32_t), 100);
        int32_t *item;

        item = rg_ArrayList_GetItem(array_list, 1);
        TEST_ASSERT_EQUAL_PTR(&array_list->_data[1 * sizeof *item], item);

        item = rg_ArrayList_GetItem(array_list, 12);
        TEST_ASSERT_EQUAL_PTR(&array_list->_data[12 * sizeof *item], item);

        rg_ArrayList_Destroy(array_list);
    }

    {
        struct rg_ArrayList *array_list = \
                rg_ArrayList_Create(sizeof(int8_t), 100);
        int8_t *item;

        item = rg_ArrayList_GetItem(array_list, 1);
        TEST_ASSERT_EQUAL_PTR(&array_list->_data[1 * sizeof *item], item);

        item = rg_ArrayList_GetItem(array_list, 33);
        TEST_ASSERT_EQUAL_PTR(&array_list->_data[33 * sizeof *item], item);

        rg_ArrayList_Destroy(array_list);
    }
}

void test_ArrayList_GetItem_ReturnsNullForInvalidIndices(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 10);
    int *item;

    item = rg_ArrayList_GetItem(array_list, 10);
    TEST_ASSERT_NULL(item);

    item = rg_ArrayList_GetItem(array_list, UINT_MAX);
    TEST_ASSERT_NULL(item);
}

void test_ArrayList_Insert_IncreasesArrayLength(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);
    int value;

    value = 1;
    rg_ArrayList_Insert(array_list, 0, &value);
    TEST_ASSERT_EQUAL_INT(2, array_list->_list_length);

    value = 3;
    rg_ArrayList_Insert(array_list, 0, &value);
    TEST_ASSERT_EQUAL_INT(3, array_list->_list_length);
}

void test_ArrayList_Insert_GrowsMemoryIfNoSpaceLeft(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);

    int value_to_insert = 0;

    do {
        rg_ArrayList_Insert(array_list, 0, &value_to_insert);
    }
    while (array_list->_list_length < array_list->_num_allocated_elements);

    size_t old_num_allocated_elements = array_list->_num_allocated_elements;
    rg_ArrayList_Insert(array_list, 0, &value_to_insert);

    TEST_ASSERT_GREATER_THAN_UINT(
            old_num_allocated_elements,
            array_list->_num_allocated_elements
    );

    rg_ArrayList_Destroy(array_list);
}

void test_ArrayList_Insert_OverwritesOldElements(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 0);

    int value;

    value = 1;
    rg_ArrayList_Insert(array_list, 0, &value);
    TEST_ASSERT_EQUAL_INT(value, ((int*)array_list->_data)[0]);

    value = 2;
    rg_ArrayList_Insert(array_list, 1, &value);
    TEST_ASSERT_EQUAL_INT(value, ((int*)array_list->_data)[1]);

    rg_ArrayList_Destroy(array_list);
}

void test_ArrayList_Insert_ShiftsTrailingElementsForwards(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 0);

    /* Start off by inserting a few values "normally" */
#define VALUES_SIZE 4
    int values[VALUES_SIZE] = { 4, 3, 2, 1 };

    for (int i = 0; i < VALUES_SIZE; ++i) {
        rg_ArrayList_Insert(array_list, i, &values[i]);
    }

    /* Then insert a value to the beginning, and make sure all the values are
     * in the right place. */
    int x = 0;
    rg_ArrayList_Insert(array_list, 0, &x);

    for (int i = 1; i <= VALUES_SIZE; ++i) {
        TEST_ASSERT_EQUAL_INT(
                values[i - 1],
                *(int*)rg_ArrayList_GetItem(array_list, i)
        );
    }
#undef VALUES_SIZE
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ArrayList_Create_DoesNotReturnNull);
    RUN_TEST(test_ArrayList_Create_AssignsBytesPerElementToValueProvided);
    RUN_TEST(test_ArrayList_Create_AssignsNumAllocatedElementsToInitialLength);
    RUN_TEST(test_ArrayList_Create_AssignsListLengthToInitialLength);
    RUN_TEST(test_ArrayList_Create_DataIsNotNull);
    RUN_TEST(test_ArrayList_Create_NumAllocatedElementsIsNeverZero);

    RUN_TEST(test_ArrayList_GetItem_DoesNotReturnNullForIndexZero);
    RUN_TEST(test_ArrayList_GetItem_ReturnsPointerToDataInStructForIndexZero);
    RUN_TEST(test_ArrayList_GetItem_ReturnsPointerToItemInStructForAnyIndex);
    RUN_TEST(test_ArrayList_GetItem_ReturnsNullForInvalidIndices);

    RUN_TEST(test_ArrayList_Insert_IncreasesArrayLength);
    RUN_TEST(test_ArrayList_Insert_GrowsMemoryIfNoSpaceLeft);
    RUN_TEST(test_ArrayList_Insert_OverwritesOldElements);
    RUN_TEST(test_ArrayList_Insert_ShiftsTrailingElementsForwards);

    return UNITY_END();
}

