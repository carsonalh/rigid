#ifndef RIGID_H
#define RIGID_H

#include <stdlib.h>
#include <stdint.h>

struct rg_ArrayList {
    size_t  _bytes_per_element;
    size_t  _list_length;
    size_t  _num_allocated_elements;
    uint8_t *_data;
};

struct rg_ArrayList*rg_ArrayList_Create(size_t bytes_per_element,
                                        size_t initial_length);
void               *rg_ArrayList_GetItem(struct rg_ArrayList *array_list,
                                         unsigned index);
void                rg_ArrayList_Insert(struct rg_ArrayList *array_list,
                                        unsigned index, void *data);
void                rg_ArrayList_Clear(struct rg_ArrayList *array_list);
void                rg_ArrayList_Destroy(struct rg_ArrayList *array_list);

#endif /* RIGID_H */

