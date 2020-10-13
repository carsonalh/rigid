#include "./rigid_list_internal.h"

#include <string.h>

void _rg_List_ShiftElements(struct rg_List *list,
                                 unsigned index, size_t length,
                                 int shift_offset)
{
    unsigned start_byte = index * list->_bytes_per_element;
    unsigned offset_in_bytes = shift_offset * list->_bytes_per_element;
    size_t length_in_bytes = length * list->_bytes_per_element;

    /* Note that data is indexed per byte (it is an int8_t). */

    memmove(&list->_data[start_byte + offset_in_bytes],
            &list->_data[start_byte], length_in_bytes);
}

bool _rg_List_Resize(struct rg_List *list)
{
    if (list->_num_allocated_elements == 0) {
        list->_num_allocated_elements = 1;
    }
    else {
        list->_num_allocated_elements *= 2;
    }

    size_t new_num_bytes = \
            list->_num_allocated_elements * list->_bytes_per_element;

    list->_data = realloc(list->_data, new_num_bytes);

    return false;
}

