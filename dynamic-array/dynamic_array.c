#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "dynamic_array.h"
#include <assert.h>
#define INITIAL_CAP 2

da_status da_init(DynArray *arr,size_t elem_size)
{
    //caller passes da_init(NULL)
    if(!arr)
        return DA_ERR_NULL_ARR;
    arr->elem_size = elem_size;  
    if(arr->elem_size == 0)
        return DA_ERR_INVALID_SIZE;
    arr->ptr = malloc(INITIAL_CAP*arr->elem_size);
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
    void *tmp = realloc(arr->ptr, new_cap*(arr->elem_size));
    if(!tmp)
        return DA_ERR_ALLOC;
    arr->cap = new_cap;
    arr->ptr = tmp;
    return DA_OK;
}

static void* da_at(const DynArray *arr, size_t index)
{
/*
why cast arr->ptr to char*? because of how pointer arithmetic work, for a pointer p of type T*
p+n means: address of p + n*sizeof(T), p in our case is a void* pointer and sizeof(void) doesn't exist
so we cast it to char* pointer because sizeof(char) = 1 byte therefore it takes 1 byte steps in memory
and is guaranteed to reach the exact address of p+n.
why only char*? because if this array contained elements of size 6 bytes and we cast it to int* (T=4)
it will not be able to reach the exact address of p+n. char* is the standard one byte-stepping pointer
*/
    return (char*)arr->ptr+index*arr->elem_size;
}

da_status da_append(DynArray *arr, const void* val)
{
    if(arr->length == arr->cap)
    {
        da_status ret = da_grow_cap(arr);
        if(ret != DA_OK)
            return ret;
    }
        /*
        why did we use memcpy instead of doing: (char*)arr->ptr+arr->length*arr->elem_size = *val?
        because when we do something like x=*p the compiler emits machine instructions like “load 4 bytes from 
        this address,store 4 bytes at that address”. with void*, *val has type void which has no size.
        */
    memcpy(da_at(arr,arr->length),val,arr->elem_size);
    arr->length++;
    return DA_OK;

}

da_status da_get(const DynArray *arr, size_t index, void *out_val)
{
    assert(out_val!=NULL);
    if (index >= arr->length)
        return DA_ERR_OUT_OF_BOUNDS;
    memcpy(out_val, da_at(arr,index), arr->elem_size);
    return DA_OK;

}



da_status da_set(DynArray *arr, size_t index, const void* val)
{
    if(index>=arr->length)
        return DA_ERR_OUT_OF_BOUNDS;
    memcpy(da_at(arr,index),val,arr->elem_size);
    return DA_OK;
}

da_status da_pop(DynArray *arr, void *out_val)
{
    if(arr->length == 0)
        return DA_ERR_EMPTY;

    assert(out_val!=NULL);
    memcpy(out_val,da_at(arr,arr->length-1),arr->elem_size);
    arr->length--;
    return DA_OK;

}

da_status da_insert_at(DynArray *arr, size_t index, const void* val)
{
    if(index>arr->length)
        return DA_ERR_OUT_OF_BOUNDS;

    if(arr->length == arr->cap)
    {
        da_status ret = da_grow_cap(arr);
        if(ret!=DA_OK)
            return ret;
    }

    memmove(da_at(arr,index+1),da_at(arr,index),(arr->length-index)*arr->elem_size);
    memcpy(da_at(arr,index),val,arr->elem_size);
    arr->length++;
    return DA_OK;

}
 
da_status da_remove_at(DynArray *arr, size_t index, void *out_val)
{

    if(index>=arr->length)
        return DA_ERR_OUT_OF_BOUNDS;
    
    if(out_val)
        memcpy(out_val,da_at(arr,index),arr->elem_size);
        

    memmove(da_at(arr,index), da_at(arr, index+1),(arr->length-index-1)*arr->elem_size);
    memset(da_at(arr,--arr->length), 0, arr->elem_size);
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

void da_print(const DynArray *arr, void (*print_elem)(const void *elem))
{
    if (!arr || !print_elem)
        return;

    for (size_t i = 0; i < arr->length; i++)
        print_elem(da_at(arr, i));
    printf("\n");
}