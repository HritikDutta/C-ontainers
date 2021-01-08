#ifndef DARRAY_H
#define DARRAY_H

#ifndef DARRAY_GROWTH_RATE
#define DARRAY_GROWTH_RATE 2
#endif // DARRAY_GROWTH_RATE

#ifndef DARRAY_START_CAP
#define DARRAY_START_CAP 2
#endif // DARRAY_START_CAP

#define DArray(type) type*

typedef struct
{
    size_t cap;
    size_t size;
    char buffer[];
} DA_Internal;

void da_make_impl(void** arr, size_t cap, size_t type_size);
void da_copy_impl(void** dest, void* src, size_t type_size);
void da_move_impl(void** dest, void** src, size_t type_size);
void da_free_impl(void** arr);

void da_resize_impl(void** arr, size_t new_cap, size_t type_size);

// void da_insert_impl(void** arr, size_t index, void* val, size_t type_size);
// void da_erase_at_impl(void** arr, size_t index, size_t type_size);
// void da_erase_swap_impl(void** arr, size_t index, size_t type_size);

inline size_t da_size_impl(void* arr);
inline size_t da_cap_impl(void* arr);

#define da_make(arr)       da_make_impl((void**)&arr, 0, sizeof(*arr))
#define da_copy(dest, src) da_copy_impl((void**)&dest, (void*)src, sizeof(*src))
#define da_move(dest, src) da_move_impl((void**)&dest, (void**)&src, sizeof(*src))
#define da_free(arr)       da_free_impl((void**)&arr)

#define da_resize(arr, cap) da_resize_impl((void**)&arr, cap, sizeof(*arr))

#define da_size(arr) da_size_impl((void*)arr)
#define da_cap(arr)  da_cap_impl((void*)arr)

#define da_push_back(arr, value)     da_push_back_impl(arr, value)
#define da_pop_back(arr)             da_pop_back_impl(arr)
#define da_insert(arr, index, value) da_insert_impl(arr, index, value)
#define da_erase_at(arr, index)      da_erase_at(arr, index)
#define da_erase_swap(arr, index)    da_erase_swap_impl(arr, index)

#ifdef DARRAY_IMPL

#include <stdlib.h>
#include <string.h>
#include "hd_assert.h"

#define da_data(arr) ((DA_Internal*)(arr) - 1)

void da_make_impl(void** arr, size_t cap, size_t type_size)
{
    size_t byte_size = cap * type_size + sizeof(DA_Internal);
    DA_Internal* da = (DA_Internal*) malloc(byte_size);
    hd_assert(da != NULL);

    da->cap  = cap;
    da->size = 0;

    *arr = da + sizeof(DA_Internal);
}

void da_copy_impl(void** dest, void* src, size_t type_size)
{
    DA_Internal* src_da = da_data(src);
    size_t byte_size = src_da->cap * type_size + sizeof(DA_Internal);
    DA_Internal* new_da = (DA_Internal*) realloc(da_data(*dest), byte_size);
    hd_assert(new_da != NULL);
    
    new_da->cap  = src_da->cap;
    new_da->size = src_da->size;

    *dest = new_da + sizeof(DA_Internal);
    memcpy(*dest, src, src_da->size * type_size);   
}

void da_move_impl(void** dest, void** src, size_t type_size)
{
    DA_Internal* dest_da = da_data(*src);
    
    *src  = NULL;
    *dest = dest_da + sizeof(DA_Internal);
}

void da_free_impl(void** arr)
{
    DA_Internal* da = da_data(arr);
    free(da);
    arr = NULL;
}


void da_resize_impl(void** arr, size_t new_cap, size_t type_size)
{
    DA_Internal* da;
    size_t size = 0, cap = new_cap;
    
    if (*arr)
    {
        da   = da_data(arr);
        size = da->size;
    }

    size_t byte_size = new_cap * type_size + sizeof(DA_Internal);
    DA_Internal* new_da = (DA_Internal*) realloc(da, byte_size);
    
    new_da->size = size;
    new_da->cap  = cap;
}

inline size_t da_size_impl(void* arr)
{
    if (!arr) return 0;
 
    return da_data(arr)->size;
}

inline size_t da_cap_impl(void* arr)
{
    if (!arr) return 0;
 
    return da_data(arr)->cap;
}


#define da_push_back_impl(arr, value) \
    do {                                                            \
        hd_assert(arr != NULL);                                     \
        DA_Internal* da = da_data(arr);                             \
        size_t size = da->size, cap = da->cap;                      \
                                                                    \
        if (cap == 0) da_resize(arr, DARRAY_START_CAP);             \
        if (size >= cap) da_resize(arr, DARRAY_GROWTH_RATE * cap);  \
                                                                    \
        arr[size++] = value;                                        \
    } while (0)

#define da_pop_back_impl(arr) \
    do {                                \
        hd_assert(arr != NULL);         \
        DA_Internal* da = da_data(arr); \
        size_t size = da->size;         \
                                        \
        if (size > 0) size--;           \
    } while(0)

#define da_insert_impl(arr, index, value) \
    do {\
        \
    } while(0)

#define da_erase_at_impl(arr, index) \
    do {\
        \
    } while(0)

#define da_erase_swap_impl(arr, index) \
    do {\
        \
    } while(0)

#endif

#endif // DARRAY_H