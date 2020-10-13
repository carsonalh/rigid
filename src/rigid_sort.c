#include "rigid.h"

#include <string.h>

/* We want to be able to use alloca on all platforms; probably put this sort of
 * functionality into a lbirary of its own at some point. */
#ifdef _WIN32
#   include <malloc.h>
#   ifndef alloca
#       define alloca(_size) _alloca(_size)
#   endif
#elif defined(unix) || defined(__unix__) || defined(__unix)
#   include <alloca.h>
#else
#   error "Platform does not have an alloca header!"
#endif

static void _rg_Swap(void *first, void *second, size_t size)
{
    void *tmp = alloca(size);
    memcpy(tmp, second, size);
    memcpy(second, first, size);
    memcpy(first, tmp, size);
}

void rg_BubbleSort(void *array, size_t element_count, size_t element_size,
                   rg_CompareFunction compare_function)
{
    for (int i = element_count - 1; i >= 0; --i) {
        for (int j = 0; j < i; ++j) {
            /* Note that j will ALWAYS be less than i... */
            void *ith_element = (int8_t *)array + i * element_size;
            void *jth_element = (int8_t *)array + j * element_size;

            bool latter_is_smaller = \
                    compare_function(jth_element, ith_element) < 0;

            if (latter_is_smaller) {
                _rg_Swap(ith_element, jth_element, element_size);
            }
        }
    }
}

void rg_SelectionSort(void *array, size_t element_count, size_t element_size,
                      rg_CompareFunction compare_function)
{
    void *minimum_value = alloca(element_size);

    for (int i = 0; i < element_count; ++i) {
        unsigned minimum_index = i;

        memcpy(minimum_value, (int8_t *)array + i * element_size,
                element_size);

        for (int j = i + 1; j < element_count; ++j) {
            /* Note that j will ALWAYS be greater than i. */

            void *jth_element = (int8_t *)array + j * element_size;

            bool j_less_than_minimum = \
                    compare_function(minimum_value, jth_element) < 0;

            if (j_less_than_minimum) {
                minimum_index = j;
                memcpy(minimum_value, jth_element, element_size);
            }
        }

        if (minimum_index != i) {
            void *ith_element = (int8_t *)array + i * element_size;
            void *smallest_element_thereafter = \
                    (int8_t *)array + minimum_index * element_size;

            _rg_Swap(ith_element, smallest_element_thereafter, element_size);
        }
    }
}

void rg_InsertionSort(void *array, size_t element_count, size_t element_size,
                      rg_CompareFunction compare_function)
{
    void *insert_value = alloca(element_size);

    for (int i = 1; i < element_count; ++i) {
        int insert_index = i;
        void *ith_element = (int8_t *)array + i * element_size;

        for (int j = i - 1; j >= 0; --j) {
            void *jth_element = (int8_t *)array + j * element_size;

            bool i_less_than_j = \
                    compare_function(jth_element, ith_element) < 0;

            if (i_less_than_j) {
                insert_index = j;
            }
            else {
                break;
            }
        }

        if (insert_index < i) {
            memcpy(insert_value, ith_element, element_size);

            void *block_start = (int8_t *)array + insert_index * element_size;

            /* Should always be moving the block one element to the right. */
            void *block_end = (int8_t *)block_start + element_size;

            size_t amount_of_bytes_to_move = (i - insert_index) * element_size;

            memmove(block_end, block_start, amount_of_bytes_to_move);
            memcpy(block_start, insert_value, element_size);
        }
    }
}

static int _rg_LomutoPartition(void *array, size_t element_count,
        size_t element_size, rg_CompareFunction compare_function)
{
    int pivot_index = element_count - 1;
    int i = -1;
    int j = 0;

    while (j < pivot_index) {
        void *pivot_element = (int8_t *)array + pivot_index * element_size;
        void *jth_element = (int8_t *)array + j * element_size;

        bool j_less_than_pivot = \
                compare_function(jth_element, pivot_element) > 0;

        if (j_less_than_pivot) {
            ++i;
            void *ith_element = (int8_t *)array + i * element_size;
            _rg_Swap(ith_element, jth_element, element_size);
        }

        ++j;
    }

    void *pivot_data = alloca(element_size);
    memcpy(pivot_data, (int8_t *)array + pivot_index * element_size,
            element_size);

    int insert_index = i + 1;

    size_t num_blocks_to_move = pivot_index - insert_index;
    size_t num_bytes_to_move = num_blocks_to_move * element_size;

    void *insert_location = (int8_t *)array + insert_index * element_size;

    memmove((int8_t *)insert_location + element_size, insert_location,
            num_bytes_to_move);

    memcpy(insert_location, pivot_data, element_size);

    return insert_index;
}

void rg_QuickSort(void *array, size_t element_count, size_t element_size,
                  rg_CompareFunction compare_function)
{
    if (element_count <= 1) {
        return;
    }

    int partition_index = \
            _rg_LomutoPartition(array, element_count, element_size,
                    compare_function);

    void *array_first_half = array;
    void *array_second_half = \
            (int8_t *)array + (partition_index + 1) * element_size;

    size_t array_first_half_length = partition_index;
    size_t array_second_half_length = element_count - partition_index - 1;

    rg_QuickSort(array_first_half, array_first_half_length, element_size,
            compare_function);
    rg_QuickSort(array_second_half, array_second_half_length, element_size,
            compare_function);
}

