#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "dynamic_array.h"
#include <assert.h>
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

static da_status da_grow_cap(DynArray *arr)
{
    size_t new_cap = arr->cap * 2;
    int *tmp = realloc(arr->ptr, new_cap*sizeof(*(arr->ptr)));
    if(!tmp)
        return DA_ERR_ALLOC;
    arr->cap = new_cap;
    arr->ptr = tmp;
    return DA_OK;
}

da_status da_append(DynArray *arr, int val)
{
    if(arr->length == arr->cap)
    {
        da_status ret = da_grow_cap(arr);
        if(ret != DA_OK)
            return ret;
    }
    arr->ptr[arr->length++] = val;
    return DA_OK;

}

da_status da_get(const DynArray *arr, size_t index, int *out_val)
{
    assert(out_val!=NULL);
    if (index >= arr->length)
        return DA_ERR_OUT_OF_BOUNDS;
    *out_val = arr->ptr[index];
    return DA_OK;

}

da_status da_set(DynArray *arr, size_t index, int val)
{
    if(index>=arr->length)
        return DA_ERR_OUT_OF_BOUNDS;
    arr->ptr[index] = val;
    return DA_OK;
}

da_status da_pop(DynArray *arr, int *out_val)
{
    if(arr->length == 0)
        return DA_ERR_EMPTY;

    assert(out_val!=NULL);
    *out_val = arr->ptr[arr->length--];
    return DA_OK;

}

da_status da_insert_at(DynArray *arr, size_t index, int val)
{
    if(index>arr->length)
        return DA_ERR_OUT_OF_BOUNDS;

    if(arr->length == arr->cap)
    {
        da_status ret = da_grow_cap(arr);
        if(ret!=DA_OK)
            return ret;
    }
    if (index==arr->length)
        return da_append(arr,val);
    int *tmp = malloc(arr->cap*sizeof(*arr->ptr));
    if(!tmp)
        return DA_ERR_ALLOC;
    //{0,1,2,3,4} insert_at(index=2,val=x) --> {0,1,x,2,3,4}
    memcpy(tmp, arr->ptr, index*sizeof(*arr->ptr));
    tmp[index] = val;
    memcpy(tmp+index+1, arr->ptr+index, (arr->length-index)*sizeof(*arr->ptr));
    arr->length++;
    free(arr->ptr);
    arr->ptr = tmp;
    return DA_OK;

}
 

