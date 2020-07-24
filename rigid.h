#ifndef RIGID_H
#define RIGID_H

#include <stdlib.h>
#include <stdint.h>

struct rg_List {
    size_t  _bytes_per_element;
    size_t  _list_length;
    size_t  _num_allocated_elements;
    uint8_t *_data;
};

struct rg_List     *rg_List_Create(size_t bytes_per_element,
                                   size_t initial_length);
void               *rg_List_GetIndex(struct rg_List *list, unsigned index);
void                rg_List_Insert(struct rg_List *list, unsigned index,
                                   void *data);
void                rg_List_Clear(struct rg_List *list);
void                rg_List_Destroy(struct rg_List *list);

#endif /* RIGID_H */

