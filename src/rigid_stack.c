#include "rigid.h"

#include <stdlib.h>

inline void _rg_Stack_Init(struct rg_Stack *stack, size_t bytes_per_element,
                           size_t initial_length)
{
    _rg_List_Init(&stack->_list, bytes_per_element, initial_length);
}

inline void _rg_Stack_Uninit(struct rg_Stack *stack)
{
    _rg_List_Uninit(&stack->_list);
}

inline struct rg_Stack *rg_Stack_Create(size_t bytes_per_element,
                                        size_t initial_length)
{
    struct rg_Stack *stack = malloc(sizeof * stack);
    _rg_Stack_Init(stack, bytes_per_element, initial_length);
    return stack;
}

inline void rg_Stack_Destroy(struct rg_Stack *stack)
{
    _rg_Stack_Uninit(stack);
    free(stack);
}

inline void rg_Stack_Push(struct rg_Stack *stack, void *item)
{
    rg_List_InsertEnd(&stack->_list, item);
}

inline void rg_Stack_Pop(struct rg_Stack *stack)
{
    rg_List_DeleteEnd(&stack->_list);
}

inline void *rg_Stack_Peek(struct rg_Stack *stack)
{
    unsigned list_size = (unsigned)rg_List_GetSize(&stack->_list);
    return rg_List_Get(&stack->_list, list_size - 1);
}

