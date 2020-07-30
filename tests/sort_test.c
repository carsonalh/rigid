#include "rigid.h"

#include <pest.h>

#include <string.h>

typedef void (*SortFunction)(void *, size_t, size_t, rg_CompareFunction);

void setUp(void) {}
void tearDown(void) {}

static int compareIntAscending(int *a, int *b)
{
    return *b - *a;
}

static int compareCharAscending(char *a, char *b)
{
    return (int)(*b - *a);
}

TEST(doesNotModifyEmptyArray, SortFunction sort)
{
#define ARRAY_SIZE 3

    int array[ARRAY_SIZE] = { 1, 2, 3 };

    int array_copy[ARRAY_SIZE];
    memcpy(array_copy, array, ARRAY_SIZE * sizeof *array);

    /* We're going to lie and tell it that this array is empty, so we have data
     * we know should not be modified. */

    const size_t purported_array_size = 0;
    sort(array, purported_array_size, sizeof *array,
            &compareIntAscending);

    ASSERT_EQUAL_MEMORY(array, array_copy, ARRAY_SIZE * sizeof array[0]);

#undef ARRAY_SIZE
}

TEST(swapsUnsortedArrayOfSizeTwo, SortFunction sort)
{
#define ARRAY_SIZE 2

    {
        int array[ARRAY_SIZE] = { 1, 0 };
        int expected_array_after[ARRAY_SIZE] = { 0, 1 };

        sort(array, ARRAY_SIZE, sizeof *array,
                (SortFunction)&compareIntAscending);

        ASSERT_EQUAL_MEMORY(expected_array_after, array,
                ARRAY_SIZE * sizeof array[0]);
    }

    {
        int8_t array[ARRAY_SIZE] = { 1, 0 };
        int8_t expected_array_after[ARRAY_SIZE] = { 0, 1 };

        sort(array, ARRAY_SIZE, sizeof *array,
                (SortFunction)&compareCharAscending);

        ASSERT_EQUAL_MEMORY(expected_array_after, array,
                ARRAY_SIZE * sizeof array[0]);
    }

#undef ARRAY_SIZE
}

TEST(doesNotModifySortedNonEmptyArray, SortFunction sort)
{
#define ARRAY_SIZE 4

    int values[ARRAY_SIZE] = { 1, 2, 3, 4 };
    int expected_array_after[ARRAY_SIZE] = { 1, 2, 3, 4 };

    sort(values, ARRAY_SIZE, sizeof *values,
            (SortFunction)&compareIntAscending);

    ASSERT_EQUAL_MEMORY(expected_array_after, values,
            ARRAY_SIZE * sizeof values[0]);

#undef ARRAY_SIZE
}

struct CustomStruct {
    int x, y, z;
    int real_data;
};

static int CustomStruct_Compare(struct CustomStruct *a, struct CustomStruct *b)
{
    return (int)(b->real_data - a->real_data);
}

TEST(correctlySwapsUnsortedArrayOfCustomStructs, SortFunction sort)
{
#define ARRAY_SIZE 2

    struct CustomStruct data[ARRAY_SIZE] = {
        { .real_data = 4 }, { .real_data = 2 }
    };

    struct CustomStruct expected_data[ARRAY_SIZE] = { data[1], data[0] };

    sort(data, ARRAY_SIZE, sizeof data[0],
            (SortFunction)&CustomStruct_Compare);

    ASSERT_EQUAL_MEMORY(data, expected_data, ARRAY_SIZE * sizeof data[0]);

#undef ARRAY_SIZE
}

TEST(correctlySortsCompleteArrayOfIntegers, SortFunction sort)
{
#define ARRAY_SIZE 10

    int array[ARRAY_SIZE] = { 8, 3, 7, 10, 1, 5, 4, 9, 6, 2 };
    int expected_array[ARRAY_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    sort(array, ARRAY_SIZE, sizeof array[0],
            (SortFunction)&compareIntAscending);

    ASSERT_EQUAL_MEMORY(array, expected_array, ARRAY_SIZE * sizeof array[0]);

#undef ARRAY_SIZE
}

int main(void)
{
    RUN_TEST(doesNotModifyEmptyArray, rg_BubbleSort);
    RUN_TEST(swapsUnsortedArrayOfSizeTwo, rg_BubbleSort);
    RUN_TEST(doesNotModifySortedNonEmptyArray, rg_BubbleSort);
    RUN_TEST(correctlySwapsUnsortedArrayOfCustomStructs, rg_BubbleSort);
    RUN_TEST(correctlySortsCompleteArrayOfIntegers, rg_BubbleSort);

    RUN_TEST(doesNotModifyEmptyArray, rg_SelectionSort);
    RUN_TEST(swapsUnsortedArrayOfSizeTwo, rg_SelectionSort);
    RUN_TEST(doesNotModifySortedNonEmptyArray, rg_SelectionSort);
    RUN_TEST(correctlySwapsUnsortedArrayOfCustomStructs, rg_SelectionSort);
    RUN_TEST(correctlySortsCompleteArrayOfIntegers, rg_SelectionSort);

    RUN_TEST(doesNotModifyEmptyArray, rg_InsertionSort);
    RUN_TEST(swapsUnsortedArrayOfSizeTwo, rg_InsertionSort);
    RUN_TEST(doesNotModifySortedNonEmptyArray, rg_InsertionSort);
    RUN_TEST(correctlySwapsUnsortedArrayOfCustomStructs, rg_InsertionSort);
    RUN_TEST(correctlySortsCompleteArrayOfIntegers, rg_InsertionSort);

    RUN_TEST(doesNotModifyEmptyArray, rg_QuickSort);
    RUN_TEST(swapsUnsortedArrayOfSizeTwo, rg_QuickSort);
    RUN_TEST(doesNotModifySortedNonEmptyArray, rg_QuickSort);
    RUN_TEST(correctlySwapsUnsortedArrayOfCustomStructs, rg_QuickSort);
    RUN_TEST(correctlySortsCompleteArrayOfIntegers, rg_QuickSort);

    return TEST_STATUS();
}

