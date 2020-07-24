#include "rigid.h"
#include "./rigid_array_list_utils.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct rg_ArrayList *rg_ArrayList_Create(size_t bytes_per_element,
                                         size_t initial_length)
{
    struct rg_ArrayList *array_list = malloc(sizeof *array_list);

    if (array_list == NULL) {
        return NULL;
    }

    size_t initial_num_allocated_elements = initial_length;

    if (initial_num_allocated_elements < 1) {
        initial_num_allocated_elements = 1;
    }

    array_list->_num_allocated_elements = initial_num_allocated_elements;

    size_t size_of_data = bytes_per_element * initial_num_allocated_elements;

    array_list->_data = calloc(1, size_of_data);

    array_list->_bytes_per_element = bytes_per_element;
    array_list->_list_length = initial_length;

    return array_list;
}

void *rg_ArrayList_GetItem(struct rg_ArrayList *array_list,
                           unsigned index)
{
    if (index >= array_list->_list_length) {
        return NULL;
    }

    const size_t bytes_per_element = array_list->_bytes_per_element;
    return &array_list->_data[index * bytes_per_element];
}

void rg_ArrayList_Insert(struct rg_ArrayList *array_list, unsigned index,
                         void *data)
{
    unsigned old_list_length = array_list->_list_length;
    ++array_list->_list_length;

    if (array_list->_list_length > array_list->_num_allocated_elements) {
        _rg_ArrayList_Resize(array_list);
    }

    unsigned num_elements_to_shift = old_list_length - index;

    if (num_elements_to_shift > 0) {
        const int shift_offset = 1;
        _rg_ArrayList_ShiftElements(array_list, index, num_elements_to_shift,
                shift_offset);
    }

    /* TODO: Maybe refactor this to its own function. */
    int8_t *array_data_pointer = rg_ArrayList_GetItem(array_list, index);
    memcpy(array_data_pointer, data, array_list->_bytes_per_element);
}

void rg_ArrayList_Clear(struct rg_ArrayList *array_list)
{
    array_list->_list_length = 0;
}

void rg_ArrayList_Destroy(struct rg_ArrayList *array_list)
{
    free(array_list->_data);
    free(array_list);
}

