#ifndef RIGID_LIST_UTILS_H
#define RIGID_LIST_UTILS_H

#include "rigid.h"

#include <stdbool.h>

void _rg_List_ShiftElements(struct rg_List *list, unsigned index,
                            size_t length, int shift_offset);
bool _rg_List_Resize(struct rg_List *list);

#endif /* RIGID_LIST_UTILS_H */

