#include "src/rigid_array_list_utils.h"

#include <unity.h>

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_ArrayList_ShiftElements_CorrectlyShiftsOneElement(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 0);

    {
        int value;

        value = 3;
        rg_ArrayList_Insert(array_list, 0, &value);
        value = 2;
        rg_ArrayList_Insert(array_list, 0, &value);
        value = 1;
        rg_ArrayList_Insert(array_list, 0, &value);
    }

    /* At this point our list should be { 1, 2, 3 } . */
    _rg_ArrayList_ShiftElements(array_list, 0, 1, 1);

    TEST_ASSERT_EQUAL_INT(1, *(int*)&array_list->_data[1 * sizeof(int)]);

    rg_ArrayList_Clear(array_list);

    unsigned bytes_of_data_allocated = \
            array_list->_num_allocated_elements
                * array_list->_bytes_per_element;

    /* Do not do this under normal circumstances. */
    memset(array_list->_data, 0, bytes_of_data_allocated);

    {
        int value;

        value = 3;
        rg_ArrayList_Insert(array_list, 0, &value);
        value = 2;
        rg_ArrayList_Insert(array_list, 0, &value);
        value = 1;
        rg_ArrayList_Insert(array_list, 0, &value);
    }

    _rg_ArrayList_ShiftElements(array_list, 1, 1, 1);

    TEST_ASSERT_EQUAL_INT(2, *(int*)&array_list->_data[2 * sizeof(int)]);

    rg_ArrayList_Destroy(array_list);
}

void test_ArrayList_ShiftElements_CorrectlyShiftsMoreThanOneELement(void)
{
#define ARRAY_SIZE 4

    struct rg_ArrayList *array_list = \
            rg_ArrayList_Create(sizeof(int), ARRAY_SIZE);
    int values[ARRAY_SIZE] = { 10, 12, 14, 16 };

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        *(int*)rg_ArrayList_GetItem(array_list, i) = values[i];
    }

    /* Move the first three elements to be the last three. */

#define START_INDEX     0
#define LENGTH          3
#define SHIFT_OFFSET    1

    _rg_ArrayList_ShiftElements(array_list, START_INDEX, LENGTH, SHIFT_OFFSET);

    for (int i = 0; i < ARRAY_SIZE - 1; ++i) {
        TEST_ASSERT_EQUAL_INT(
                values[i],
                *(int*)rg_ArrayList_GetItem(array_list, i + 1)
        );
    }

#undef START_INDEX
#undef LENGTH
#undef SHIFT_OFFSET

    rg_ArrayList_Destroy(array_list);

#undef ARRAY_SIZE
}

void test_ArrayList_Resize_SetsSizeToOneIfItIsZero(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);
    array_list->_num_allocated_elements = 0;

    _rg_ArrayList_Resize(array_list);

    TEST_ASSERT_EQUAL_UINT(1, array_list->_num_allocated_elements);

    rg_ArrayList_Destroy(array_list);
}

void test_ArrayList_Resize_DoublesSizeIfItIsNotZero(void)
{
    struct rg_ArrayList *array_list = rg_ArrayList_Create(sizeof(int), 1);

    array_list->_num_allocated_elements = 1;
    _rg_ArrayList_Resize(array_list);
    TEST_ASSERT_EQUAL_UINT(2, array_list->_num_allocated_elements);

    array_list->_num_allocated_elements = 4;
    _rg_ArrayList_Resize(array_list);
    TEST_ASSERT_EQUAL_UINT(8, array_list->_num_allocated_elements);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ArrayList_ShiftElements_CorrectlyShiftsOneElement);
    RUN_TEST(test_ArrayList_ShiftElements_CorrectlyShiftsMoreThanOneELement);

    RUN_TEST(test_ArrayList_Resize_SetsSizeToOneIfItIsZero);
    RUN_TEST(test_ArrayList_Resize_DoublesSizeIfItIsNotZero);

    return UNITY_END();
}

