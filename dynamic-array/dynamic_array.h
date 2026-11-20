

/*a dynamic array is a block of memory that grows when it fills up,
same as python's list. 
it holds 3 things:
1) *ptr: pointer to heap buffer
2) length: how many elements are in use
3) cap: how many elements fit before the buffer has to grow

operations and costs:

operation                cost
---------                -----
get/set by index         O(1)
append/pop from end      O(1) average. worst case append that triggers resize O(n)
insert/delete            O(n) (memmove)

v1 will work only for int, v2 will be generic.

in order API implementation:
1) init: set up empty array
2) free: release the buffer, set the pointer to NULL, and cap/length to 0
3) append: add one element at the end, grow buffer if cap == length, buffer will grow by 2X
         to keep it O(1) on average. basically same as append in python
4) get/set: bounds-checked access
5) pop: remove from end and hand value back
6) insert_at/remove_at: shift elements with memmove


*/

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#include <stddef.h>


typedef struct DynArray {
    int *ptr;
    size_t cap;
    size_t length;
} DynArray;

typedef enum {
    DA_OK = 0,
    DA_ERR_ALLOC,        // malloc/realloc failed
    DA_ERR_OUT_OF_BOUNDS, // index out of range
    DA_ERR_EMPTY,         // example if pop on empty array
    DA_ERR_NULL_ARR      // null array
} da_status;


//init array with length = 0, cap = 2
da_status da_init(DynArray *arr);
void da_free(DynArray *arr);
//add element to the end of the array
da_status da_append(DynArray *arr, int val);
//get element by index
da_status da_get(const DynArray *arr, size_t index, int *out_val);
//set element by index
da_status da_set(DynArray *arr, size_t index, int val);
//delete last element and store it in out_val
da_status da_pop(DynArray *arr, int *out_val);
// index may equal length (same as append)
da_status da_insert_at(DynArray *arr, size_t index, int val);
// out_val may be NULL if the removed value isn't needed
da_status da_remove_at(DynArray *arr, size_t index, int *out_val);
size_t da_len(const DynArray *arr);
size_t da_cap(const DynArray *arr);
void da_print(const DynArray *arr);




#endif