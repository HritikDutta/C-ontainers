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

inline size_t da_size_impl(void* arr);
inline size_t da_cap_impl(void* arr);

#define da_make(arr)       da_make_impl((void**)&arr, DARRAY_START_CAP, sizeof(*arr))
#define da_copy(dest, src) da_copy_impl((void**)&dest, (void*)src, sizeof(*src))
#define da_move(dest, src) da_move_impl((void**)&dest, (void**)&src, sizeof(*src))
#define da_free(arr)       da_free_impl((void**)&arr)

#define da_make_with_cap(arr, cap) da_make_impl((void**)&arr, cap, sizeof(*arr))
#define da_resize(arr, cap)        da_resize_impl((void**)&arr, cap, sizeof(*arr))

#define da_size(arr) da_size_impl((void*)arr)
#define da_cap(arr)  da_cap_impl((void*)arr)

#define da_push_back(arr, value)     da_push_back_impl(arr, value)
#define da_pop_back(arr)             da_pop_back_impl(arr)
#define da_insert(arr, index, value) da_insert_impl(arr, index, value)
#define da_erase_at(arr, index)      da_erase_at_impl(arr, index)
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

    *arr = da + 1;
}

void da_copy_impl(void** dest, void* src, size_t type_size)
{
    hd_assert(src != NULL);
    DA_Internal* src_da = da_data(src);
    size_t byte_size = src_da->cap * type_size + sizeof(DA_Internal);
    
    DA_Internal* dest_da;
    if (*dest) dest_da = (DA_Internal*) realloc(da_data(*dest), byte_size);
    else       dest_da = (DA_Internal*) malloc(byte_size);

    hd_assert(dest_da != NULL);
    
    size_t filled_byte_size = src_da->size * type_size + sizeof(DA_Internal);
    memcpy(dest_da, src_da, filled_byte_size);

    *dest = dest_da + 1;
}

void da_move_impl(void** dest, void** src, size_t type_size)
{
    hd_assert(*src != NULL);
    DA_Internal* dest_da = da_data(*src);
    
    *src  = NULL;
    *dest = dest_da + 1;
}

void da_free_impl(void** arr)
{
    hd_assert(*arr != NULL);
    DA_Internal* da = da_data(arr);
    free(da);
    arr = NULL;
}


void da_resize_impl(void** arr, size_t new_cap, size_t type_size)
{
    DA_Internal* da = NULL;
    size_t size = 0;
    
    if (*arr)
    {
        da   = da_data(*arr);
        size = da->size;
    }

    size_t byte_size = new_cap * type_size + sizeof(DA_Internal);
    DA_Internal* new_da = (DA_Internal*) realloc(da, byte_size);
    
    new_da->size = size;
    new_da->cap  = new_cap;

    *arr = new_da + 1;
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
        if (cap == 0)    da_resize(arr, DARRAY_START_CAP);          \
        if (size >= cap) da_resize(arr, DARRAY_GROWTH_RATE * cap);  \
                                                                    \
        arr[da_data(arr)->size++] = value;                          \
    } while (0)

#define da_pop_back_impl(arr) \
    do {                                    \
        hd_assert(arr != NULL);             \
        DA_Internal* da = da_data(arr);     \
        size_t size = da->size;             \
                                            \
        if (size > 0) da->size--;           \
    } while(0)

#define da_insert_impl(arr, index, value) \
    do {                                                                \
        hd_assert(arr != NULL);                                         \
        DA_Internal* da = da_data(arr);                                 \
        size_t size = da->size, cap = da->cap;                          \
                                                                        \
        if (cap == 0)        da_resize(arr, DARRAY_START_CAP);          \
        if (size + 1 >= cap) da_resize(arr, DARRAY_GROWTH_RATE * cap);  \
                                                                        \
        for (int i = size; i > index; i--)                              \
            arr[i] = arr[i - 1];                                        \
                                                                        \
        arr[index] = value;                                             \
        da_data(arr)->size++;                                           \
    } while(0)

#define da_erase_at_impl(arr, index) \
    do {                                        \
        hd_assert(arr != NULL);                 \
        DA_Internal* da = da_data(arr);         \
        size_t size = da->size;                 \
        hd_assert(size > 0 && index < size);    \
                                                \
        for (int i = index; i < size - 1; i++)  \
            arr[i] = arr[i + 1];                \
                                                \
        da->size--;                             \
    } while(0)

#define da_erase_swap_impl(arr, index) \
    do {                                        \
        hd_assert(arr != NULL);                 \
        DA_Internal* da = da_data(arr);         \
        size_t size = da->size;                 \
        hd_assert(size > 0 && index < size);    \
                                                \
        arr[index] = arr[size - 1];             \
        da->size--;                             \
    } while(0)

#endif

#endif // DARRAY_H