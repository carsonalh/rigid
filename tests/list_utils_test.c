#include "src/rigid_list_utils.h"

#include <unity.h>

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_List_ShiftElements_CorrectlyShiftsOneElement(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 0);

    {
        int value;

        value = 3;
        rg_List_Insert(list, 0, &value);
        value = 2;
        rg_List_Insert(list, 0, &value);
        value = 1;
        rg_List_Insert(list, 0, &value);
    }

    /* At this point our list should be { 1, 2, 3 } . */
    _rg_List_ShiftElements(list, 0, 1, 1);

    TEST_ASSERT_EQUAL_INT(1, *(int*)&list->_data[1 * sizeof(int)]);

    rg_List_Clear(list);

    unsigned bytes_of_data_allocated = \
            list->_num_allocated_elements
                * list->_bytes_per_element;

    /* Do not do this under normal circumstances. */
    memset(list->_data, 0, bytes_of_data_allocated);

    {
        int value;

        value = 3;
        rg_List_Insert(list, 0, &value);
        value = 2;
        rg_List_Insert(list, 0, &value);
        value = 1;
        rg_List_Insert(list, 0, &value);
    }

    _rg_List_ShiftElements(list, 1, 1, 1);

    TEST_ASSERT_EQUAL_INT(2, *(int*)&list->_data[2 * sizeof(int)]);

    rg_List_Destroy(list);
}

void test_List_ShiftElements_CorrectlyShiftsMoreThanOneELement(void)
{
#define LIST_SIZE 4

    struct rg_List *list = \
            rg_List_Create(sizeof(int), LIST_SIZE);
    int values[LIST_SIZE] = { 10, 12, 14, 16 };

    for (int i = 0; i < LIST_SIZE; ++i) {
        *(int*)rg_List_GetIndex(list, i) = values[i];
    }

    /* Move the first three elements to be the last three. */

#define START_INDEX     0
#define LENGTH          3
#define SHIFT_OFFSET    1

    _rg_List_ShiftElements(list, START_INDEX, LENGTH, SHIFT_OFFSET);

    for (int i = 0; i < LIST_SIZE - 1; ++i) {
        TEST_ASSERT_EQUAL_INT(
                values[i],
                *(int*)rg_List_GetIndex(list, i + 1)
        );
    }

#undef START_INDEX
#undef LENGTH
#undef SHIFT_OFFSET

    rg_List_Destroy(list);

#undef LIST_SIZE
}

void test_List_Resize_SetsSizeToOneIfItIsZero(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);
    list->_num_allocated_elements = 0;

    _rg_List_Resize(list);

    TEST_ASSERT_EQUAL_UINT(1, list->_num_allocated_elements);

    rg_List_Destroy(list);
}

void test_List_Resize_DoublesSizeIfItIsNotZero(void)
{
    struct rg_List *list = rg_List_Create(sizeof(int), 1);

    list->_num_allocated_elements = 1;
    _rg_List_Resize(list);
    TEST_ASSERT_EQUAL_UINT(2, list->_num_allocated_elements);

    list->_num_allocated_elements = 4;
    _rg_List_Resize(list);
    TEST_ASSERT_EQUAL_UINT(8, list->_num_allocated_elements);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_List_ShiftElements_CorrectlyShiftsOneElement);
    RUN_TEST(test_List_ShiftElements_CorrectlyShiftsMoreThanOneELement);

    RUN_TEST(test_List_Resize_SetsSizeToOneIfItIsZero);
    RUN_TEST(test_List_Resize_DoublesSizeIfItIsNotZero);

    return UNITY_END();
}

