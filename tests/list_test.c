#include "rigid.h"

#include <unity.h>

#include <limits.h>
#include <stdio.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_List_Create_DoesNotReturnNull(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);
    TEST_ASSERT_NOT_NULL(list);
}

void test_List_Create_AssignsBytesPerElementToValueProvided(void)
{
#define BYTES_PER_ELEMENT (sizeof(int))

    struct rg_List *list = \
            rg_List_Create(BYTES_PER_ELEMENT, 1);
    TEST_ASSERT_TRUE(list->_bytes_per_element == BYTES_PER_ELEMENT);

#undef BYTES_PER_ELEMENT
#define BYTES_PER_ELEMENT (sizeof(char))

    list = rg_List_Create(BYTES_PER_ELEMENT, 1);
    TEST_ASSERT_TRUE(list->_bytes_per_element == BYTES_PER_ELEMENT);

#undef BYTES_PER_ELEMENT
}

void test_List_Create_AssignsNumAllocatedElementsToInitialLength(void)
{
#define LIST_LENGTH 10

    struct rg_List *list = \
            rg_List_Create(sizeof(int), LIST_LENGTH);
    TEST_ASSERT_TRUE(list->_num_allocated_elements == LIST_LENGTH);

#undef LIST_LENGTH
#define LIST_LENGTH 20

    list = rg_List_Create(sizeof(int), LIST_LENGTH);
    TEST_ASSERT_TRUE(list->_num_allocated_elements == LIST_LENGTH);

#undef LIST_LENGTH
}

void test_List_Create_AssignsListLengthToInitialLength(void)
{
    struct rg_List *list;

    list = rg_List_Create(sizeof(int), 1);
    TEST_ASSERT_EQUAL_UINT(1, list->_list_length);
    rg_List_Destroy(list);

    list = rg_List_Create(sizeof(int), 3);
    TEST_ASSERT_EQUAL_UINT(3, list->_list_length);
    rg_List_Destroy(list);
}

void test_List_Create_DataIsNotNull(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);
    TEST_ASSERT_NOT_NULL(list->_data);
}

void test_List_Create_NumAllocatedElementsIsNeverZero(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 0);
    TEST_ASSERT_NOT_EQUAL_UINT(0, list->_num_allocated_elements);
    rg_List_Destroy(list);
}

void test_List_GetIndex_DoesNotReturnNullForIndexZero(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);
    int *first_item_in_list = rg_List_Get(list, 0);
    TEST_ASSERT_NOT_NULL(first_item_in_list);
}

void test_List_GetIndex_ReturnsPointerToDataInStructForIndexZero(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);
    int *first_item_in_list = rg_List_Get(list, 0);
    TEST_ASSERT_EQUAL_PTR(&list->_data[0], first_item_in_list);
}

void test_List_GetIndex_ReturnsPointerToItemInStructForAnyIndex(void)
{
    {
        struct rg_List *list = \
                rg_List_Create(sizeof(int32_t), 100);
        int32_t *item;

        item = rg_List_Get(list, 1);
        TEST_ASSERT_EQUAL_PTR(&list->_data[1 * sizeof *item], item);

        item = rg_List_Get(list, 12);
        TEST_ASSERT_EQUAL_PTR(&list->_data[12 * sizeof *item], item);

        rg_List_Destroy(list);
    }

    {
        struct rg_List *list = \
                rg_List_Create(sizeof(int8_t), 100);
        int8_t *item;

        item = rg_List_Get(list, 1);
        TEST_ASSERT_EQUAL_PTR(&list->_data[1 * sizeof *item], item);

        item = rg_List_Get(list, 33);
        TEST_ASSERT_EQUAL_PTR(&list->_data[33 * sizeof *item], item);

        rg_List_Destroy(list);
    }
}

void test_List_GetIndex_ReturnsNullForInvalidIndices(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 10);
    int *item;

    item = rg_List_Get(list, 10);
    TEST_ASSERT_NULL(item);

    item = rg_List_Get(list, UINT_MAX);
    TEST_ASSERT_NULL(item);
}

void test_List_Insert_IncreasesListLength(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);
    int value;

    value = 1;
    rg_List_Insert(list, 0, &value);
    TEST_ASSERT_EQUAL_INT(2, list->_list_length);

    value = 3;
    rg_List_Insert(list, 0, &value);
    TEST_ASSERT_EQUAL_INT(3, list->_list_length);
}

void test_List_Insert_GrowsMemoryIfNoSpaceLeft(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);

    int value_to_insert = 0;

    do {
        rg_List_Insert(list, 0, &value_to_insert);
    }
    while (list->_list_length < list->_num_allocated_elements);

    size_t old_num_allocated_elements = list->_num_allocated_elements;
    rg_List_Insert(list, 0, &value_to_insert);

    TEST_ASSERT_GREATER_THAN_UINT(
            old_num_allocated_elements,
            list->_num_allocated_elements
    );

    rg_List_Destroy(list);
}

void test_List_Insert_OverwritesOldElements(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 0);

    int value;

    value = 1;
    rg_List_Insert(list, 0, &value);
    TEST_ASSERT_EQUAL_INT(value, ((int*)list->_data)[0]);

    value = 2;
    rg_List_Insert(list, 1, &value);
    TEST_ASSERT_EQUAL_INT(value, ((int*)list->_data)[1]);

    rg_List_Destroy(list);
}

void test_List_Insert_ShiftsTrailingElementsForwards(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 0);

    /* Start off by inserting a few values "normally" */
#define VALUES_SIZE 4
    int values[VALUES_SIZE] = { 4, 3, 2, 1 };

    for (int i = 0; i < VALUES_SIZE; ++i) {
        rg_List_Insert(list, i, &values[i]);
    }

    /* Then insert a value to the beginning, and make sure all the values are
     * in the right place. */
    int x = 0;
    rg_List_Insert(list, 0, &x);

    for (int i = 1; i <= VALUES_SIZE; ++i) {
        TEST_ASSERT_EQUAL_INT(
                values[i - 1],
                *(int*)rg_List_Get(list, i)
        );
    }
#undef VALUES_SIZE
}

void test_List_Delete_DecrementsListSize(void)
{
    struct rg_List *list;

#define LIST_SIZE 3
    
    list = rg_List_Create(sizeof(int), LIST_SIZE);

    rg_List_Delete(list, 0);
    TEST_ASSERT_EQUAL_INT(LIST_SIZE - 1, list->_list_length);

    rg_List_Destroy(list);

#undef LIST_SIZE
#define LIST_SIZE 5

    list = rg_List_Create(sizeof(int), LIST_SIZE);

    rg_List_Delete(list, 0);
    TEST_ASSERT_EQUAL_INT(LIST_SIZE - 1, list->_list_length);

    rg_List_Destroy(list);

#undef LIST_SIZE
}

void test_List_Delete_ShiftsNextElementBackByOne(void)
{
    struct rg_List *list;

#define LIST_SIZE 4

    list = rg_List_Create(sizeof(int), LIST_SIZE);

    {
        int values[LIST_SIZE] = { 1, 2, 3, 4 };
        memcpy(list->_data, values, sizeof values);

        rg_List_Delete(list, 0);

        TEST_ASSERT_EQUAL_INT(2, *(int*)rg_List_Get(list, 0));
    }

    {
        int values[LIST_SIZE] = { 10, 12, 14 };
        memcpy(list->_data, values, sizeof values);

        rg_List_Delete(list, 1);

        TEST_ASSERT_EQUAL_INT(14, *(int*)rg_List_Get(list, 1));
    }

    {
        rg_List_Destroy(list);
        list = rg_List_Create(sizeof(int8_t), LIST_SIZE);

        int8_t values[LIST_SIZE] = { 100, 90, 80, 70 };
        memcpy(list->_data, values, sizeof values);

        rg_List_Delete(list, 1);

        TEST_ASSERT_EQUAL_INT8(80, *(int8_t*)rg_List_Get(list, 1));
    }

    rg_List_Destroy(list);

#undef LIST_SIZE
}

void test_List_Delete_DeletingLastElementDoesNotModifyContents(void)
{
#define LIST_SIZE 5

    struct rg_List *list = rg_List_Create(sizeof(int16_t), LIST_SIZE);

    int16_t values[LIST_SIZE] = { 5, 4, 3, 2, 1 };
    memcpy(list->_data, values, sizeof values);

    rg_List_Delete(list, LIST_SIZE - 1);

    for (int i = 0; i < LIST_SIZE - 1; ++i) {
        TEST_ASSERT_EQUAL_INT16(values[i], *(int16_t*)rg_List_Get(list, i));
    }

#undef LIST_SIZE
}

void test_List_Delete_ShiftsAllTrailingElementsBackByOne(void)
{
#define LIST_SIZE 5

    int16_t values[LIST_SIZE] = { 2, 4, 6, 8, 10 };
    struct rg_List *list = rg_List_Create(sizeof(int16_t), LIST_SIZE);

    memcpy(list->_data, values, sizeof values);

    rg_List_Delete(list, 2);
    int16_t expected_values[LIST_SIZE - 1] = { 2, 4, 8, 10 };

    TEST_ASSERT_EQUAL_INT16_ARRAY(expected_values, list->_data, LIST_SIZE - 1);

    rg_List_Destroy(list);

#undef LIST_SIZE
}

void test_List_Delete_ReturnsTrueForListWithElements(void)
{
#define LIST_SIZE 5

    int32_t values[LIST_SIZE] = { 0, 0, 0, 0, 0 };
    struct rg_List *list = rg_List_Create(sizeof(int32_t), LIST_SIZE);

    memcpy(list->_data, values, sizeof values);

    for (int i = 0; i < LIST_SIZE; ++i) {
        bool status;
        status = rg_List_Delete(list, 0);
        TEST_ASSERT_TRUE(status);
    }

#undef LIST_SIZE
}

void test_List_Delete_ReturnsFalseForListWithNoElements(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 0);

    bool status;
    status = rg_List_Delete(list, 0);
    TEST_ASSERT_FALSE(status);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_List_Create_DoesNotReturnNull);
    RUN_TEST(test_List_Create_AssignsBytesPerElementToValueProvided);
    RUN_TEST(test_List_Create_AssignsNumAllocatedElementsToInitialLength);
    RUN_TEST(test_List_Create_AssignsListLengthToInitialLength);
    RUN_TEST(test_List_Create_DataIsNotNull);
    RUN_TEST(test_List_Create_NumAllocatedElementsIsNeverZero);

    RUN_TEST(test_List_GetIndex_DoesNotReturnNullForIndexZero);
    RUN_TEST(test_List_GetIndex_ReturnsPointerToDataInStructForIndexZero);
    RUN_TEST(test_List_GetIndex_ReturnsPointerToItemInStructForAnyIndex);
    RUN_TEST(test_List_GetIndex_ReturnsNullForInvalidIndices);

    RUN_TEST(test_List_Insert_IncreasesListLength);
    RUN_TEST(test_List_Insert_GrowsMemoryIfNoSpaceLeft);
    RUN_TEST(test_List_Insert_OverwritesOldElements);
    RUN_TEST(test_List_Insert_ShiftsTrailingElementsForwards);

    RUN_TEST(test_List_Delete_DecrementsListSize);
    RUN_TEST(test_List_Delete_ShiftsNextElementBackByOne);
    RUN_TEST(test_List_Delete_DeletingLastElementDoesNotModifyContents);
    RUN_TEST(test_List_Delete_ShiftsAllTrailingElementsBackByOne);
    RUN_TEST(test_List_Delete_ReturnsTrueForListWithElements);
    RUN_TEST(test_List_Delete_ReturnsFalseForListWithNoElements);

    return UNITY_END();
}

