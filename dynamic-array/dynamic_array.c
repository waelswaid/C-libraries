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
    arr->ptr = malloc(INITIAL_CAP*sizeof(*arr->ptr));
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
    *out_val = arr->ptr[--arr->length];
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

    memmove(arr->ptr+index+1,arr->ptr+index,(arr->length-index)*sizeof(*arr->ptr));
    arr->ptr[index] = val;
    arr->length++;
    return DA_OK;

/*
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
*/
}
 
da_status da_remove_at(DynArray *arr, size_t index, int *out_val)
{
    //{0,1,2,3,4} --> da_remove_at(index=2) --> {0,1,3,4}
    //{0,1,2,3,4} --> loop(mv index+1->index), length--; --> {0,1,3,4}
    //memory illustration: {0,1,2,3,4} --> {0,1,3,3,4} --> {0,1,3,4,4} --> {0,1,3,4}
    if(index>=arr->length)
        return DA_ERR_OUT_OF_BOUNDS;
    
    if(out_val)
        *out_val = arr->ptr[index];

    memmove(arr->ptr+index, arr->ptr+index+1,(arr->length-index-1)*sizeof(*arr->ptr));
    arr->ptr[--arr->length] = 0;
    return DA_OK;
}

size_t da_len(const DynArray *arr)
{
    return arr->length;
}
size_t da_cap(const DynArray *arr)
{
    return arr->cap;
}

void da_print(const DynArray *arr)
{
    assert(arr->length>0);
        
    for(size_t i = 0; i < arr->length; i++)
        printf("%d  ",arr->ptr[i]);
    printf("\n");
}