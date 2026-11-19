#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "dynamic_array.h"
#define INITIAL_CAP 2

da_status da_init(DynArray *arr)
{
    //caller passes da_init(NULL)
    if(!arr)
        return DA_ERR_NULL_ARR;  
    arr->ptr = malloc(INITIAL_CAP*sizeof(arr->ptr));
    if(!(arr->ptr))
        return DA_ERR_ALLOC;
    arr->cap = INITIAL_CAP;
    arr->length = 0;
    return DA_OK;
}

void da_free(DynArray *arr)
{
    arr->cap = 0;
    arr->length = 0;
    free(arr->ptr);
    arr->ptr = NULL;
    
}

da_status da_append(DynArray *arr, int val)
{
    if(arr->length == arr->cap)
    {
        size_t new_cap = arr->cap * 2;
        int *tmp = realloc(arr->ptr, new_cap*sizeof(*(arr->ptr)));
        if(!tmp)
            return DA_ERR_ALLOC;
        arr->cap = new_cap;
        arr->ptr = tmp;
    }
    arr->ptr[arr->length++] = val;
    return DA_OK;

}


