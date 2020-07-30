#include "rigid.h"
#include "./rigid_list_internal.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct rg_List *rg_List_Create(size_t bytes_per_element, size_t initial_length)
{
    struct rg_List *list = malloc(sizeof *list);

    if (list == NULL) {
        return NULL;
    }

    size_t initial_num_allocated_elements = initial_length;

    if (initial_num_allocated_elements < 1) {
        initial_num_allocated_elements = 1;
    }

    list->_num_allocated_elements = initial_num_allocated_elements;

    size_t size_of_data = bytes_per_element * initial_num_allocated_elements;

    list->_data = calloc(1, size_of_data);

    list->_bytes_per_element = bytes_per_element;
    list->_list_length = initial_length;

    return list;
}

void *rg_List_Get(const struct rg_List *list, unsigned index)
{
    if (index >= list->_list_length) {
        return NULL;
    }

    const size_t bytes_per_element = list->_bytes_per_element;
    return &list->_data[index * bytes_per_element];
}

size_t rg_List_GetSize(const struct rg_List *list)
{
    return list->_list_length;
}

void rg_List_Insert(struct rg_List *list, unsigned index, void *data)
{
    unsigned old_list_length = list->_list_length;
    ++list->_list_length;

    if (list->_list_length > list->_num_allocated_elements) {
        _rg_List_Resize(list);
    }

    unsigned num_elements_to_shift = old_list_length - index;

    if (num_elements_to_shift > 0) {
        const int shift_offset = 1;
        /* Shifts the elements forward by shift_offset. */
        _rg_List_ShiftElements(list, index, num_elements_to_shift,
                shift_offset);
    }

    /* Possibly refactor this to its own function that sets a value at an index in a list. */
    int8_t *array_data_pointer = rg_List_Get(list, index);
    memcpy(array_data_pointer, data, list->_bytes_per_element);
}

bool rg_List_Delete(struct rg_List *list, unsigned index)
{
    const size_t list_length = list->_list_length;
    bool at_least_one_trailing_element = index < list_length;

    if (at_least_one_trailing_element) {
        const int shift_back_by_one = -1;
        unsigned subsection_length = list_length - index - 1;

        _rg_List_ShiftElements(list, index + 1, subsection_length,
                shift_back_by_one);
    }

    if (list->_list_length > 0) {
        --list->_list_length;
        return true;
    }
    else {
        return false;
    }
}

void rg_List_Clear(struct rg_List *list)
{
    list->_list_length = 0;
}

void rg_List_Destroy(struct rg_List *list)
{
    free(list->_data);
    free(list);
}

