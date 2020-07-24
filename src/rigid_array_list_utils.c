#include "./rigid_list_utils.h"

#include <string.h>

void _rg_ArrayList_ShiftElements(struct rg_ArrayList *array_list,
                                 unsigned index, unsigned length,
                                 int shift_offset)
{
    unsigned start_byte = index * array_list->_bytes_per_element;
    unsigned offset_in_bytes = shift_offset * array_list->_bytes_per_element;
    unsigned length_in_bytes = length * array_list->_bytes_per_element;

    /* Note that data is indexed per byte (it is an int8_t). */

    memmove(&array_list->_data[start_byte + offset_in_bytes],
            &array_list->_data[start_byte], length_in_bytes);
}

bool _rg_ArrayList_Resize(struct rg_ArrayList *array_list)
{
    if (array_list->_num_allocated_elements == 0) {
        array_list->_num_allocated_elements = 1;
    }
    else {
        array_list->_num_allocated_elements *= 2;
    }

    size_t new_num_bytes = \
            array_list->_num_allocated_elements
            * array_list->_bytes_per_element;

    array_list->_data = realloc(array_list->_data, new_num_bytes);

    return false;
}

