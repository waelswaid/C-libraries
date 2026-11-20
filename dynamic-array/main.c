#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "dynamic_array.h"

int main(void)
{
    DynArray arr;
    da_status ret;
    ret = da_init(&arr);
    if(ret!=DA_OK)
        return ret;
    //printf("cap:%zu\nlength:%zu\n",arr.cap,arr.length);
    da_append(&arr,5);
    da_append(&arr,9);
    da_append(&arr,12);
    da_print(&arr);
    int out_val;

    da_pop(&arr,&out_val);
    printf("popped value: %d\n",out_val);
    da_print(&arr);
    da_insert_at(&arr,2,26);
    da_print(&arr);
    da_remove_at(&arr,2,NULL);
    da_print(&arr);
    return 0;
}