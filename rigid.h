#ifndef RIGID_H
#define RIGID_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct rg_List {
    size_t  _bytes_per_element;
    size_t  _list_length;
    size_t  _num_allocated_elements;
    uint8_t *_data;
} rg_List;

void            _rg_List_Init(rg_List *list, size_t bytes_per_element,
                              size_t initial_length);
void            _rg_List_Uninit(rg_List *list);

rg_List        *rg_List_Create(size_t bytes_per_element,
                               size_t initial_length);
void            rg_List_Destroy(rg_List *list);
void           *rg_List_Get(const rg_List *list, unsigned index);
size_t          rg_List_GetSize(const rg_List *list);
void            rg_List_Insert(rg_List *list, unsigned index,
                               const void *data);
void            rg_List_InsertStart(rg_List *list, const void *data);
void            rg_List_InsertEnd(rg_List *list, const void *data);
bool            rg_List_Delete(rg_List *list, unsigned index);
bool            rg_List_DeleteStart(rg_List *list);
bool            rg_List_DeleteEnd(rg_List *list);
void            rg_List_Clear(rg_List *list);

typedef struct rg_Stack {
    struct rg_List _list;
} rg_Stack;

inline void             _rg_Stack_Init(rg_Stack *stack,
                                       size_t bytes_per_element,
                                       size_t initial_length);
inline void             _rg_Stack_Uninit(rg_Stack *stack);
inline rg_Stack        *rg_Stack_Create(size_t bytes_per_element,
                                        size_t initial_length);
inline void             rg_Stack_Destroy(rg_Stack *stack);
inline void             rg_Stack_Push(rg_Stack *stack, void *item);
inline void             rg_Stack_Pop(rg_Stack *stack);
inline void            *rg_Stack_Peek(rg_Stack *stack);

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

