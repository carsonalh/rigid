#ifndef RIGID_ARRAY_LIST_UTILS_H
#define RIGID_ARRAY_LIST_UTILS_H

#include "rigid.h"

#include <stdbool.h>

void _rg_ArrayList_ShiftElements(struct rg_ArrayList *array_list,
                                 unsigned index, unsigned length,
                                 int shift_offset);
bool _rg_ArrayList_Resize(struct rg_ArrayList *array_list);

#endif /* RIGID_ARRAY_LIST_UTILS_H */

