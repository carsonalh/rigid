#ifndef RIGID_H
#define RIGID_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct rg_List {
    size_t  _bytes_per_element;
    size_t  _list_length;
    size_t  _num_allocated_elements;
    uint8_t *_data;
};

void            _rg_List_Init(struct rg_List *list, size_t bytes_per_element,
                              size_t initial_length);
void            _rg_List_Uninit(struct rg_List *list);

struct rg_List *rg_List_Create(size_t bytes_per_element,
                               size_t initial_length);
void            rg_List_Destroy(struct rg_List *list);
void           *rg_List_Get(const struct rg_List *list, unsigned index);
size_t          rg_List_GetSize(const struct rg_List *list);
void            rg_List_Insert(struct rg_List *list, unsigned index,
                               const void *data);
void            rg_List_InsertStart(struct rg_List *list, const void *data);
void            rg_List_InsertEnd(struct rg_List *list, const void *data);
bool            rg_List_Delete(struct rg_List *list, unsigned index);
bool            rg_List_DeleteStart(struct rg_List *list);
bool            rg_List_DeleteEnd(struct rg_List *list);
void            rg_List_Clear(struct rg_List *list);

/* Function which returns a value greater than zero if the right input is
 * greater than the left; zero if they are equivalent; and a number less than
 * zero if the right is less than that of the left. */
typedef int (*rg_CompareFunction)(void *l, void *r);

void rg_BubbleSort(void *array, size_t element_count, size_t element_size,
                   rg_CompareFunction compare_function);
void rg_SelectionSort(void *array, size_t element_count, size_t element_size,
                      rg_CompareFunction compare_function);
void rg_InsertionSort(void *array, size_t element_count, size_t element_size,
                      rg_CompareFunction compare_function);
void rg_QuickSort(void *array, size_t element_count, size_t element_size,
                  rg_CompareFunction compare_function);

#endif /* RIGID_H */

