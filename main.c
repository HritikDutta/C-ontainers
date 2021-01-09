#include <stdio.h>

#define DARRAY_IMPL
#include "containers/darray.h"

#define LOG_INT(x) printf("%s: %d\n", #x, x)

int main()
{
    DArray(int) arr = NULL;
    da_make(arr);

    printf("arr :: size: %zd, cap: %zd\n", da_size(arr), da_cap(arr));

    da_push_back(arr, 4);
    da_push_back(arr, 3);
    da_push_back(arr, 5);
    da_push_back(arr, 7);
    da_push_back(arr, 8);
    
    da_pop_back(arr);

    printf("arr :: size: %zd, cap: %zd\n", da_size(arr), da_cap(arr));

    for (int i = 0; i < da_size(arr); i++)
        printf("arr[%d] :: %d\n", i, arr[i]);

    DArray(int) dest = NULL;
    da_copy(dest, arr);

    printf("\ndest :: size: %zd, cap: %zd\n", da_size(dest), da_cap(dest));

    for (int i = 0; i < da_size(dest); i++)
        printf("dest[%d] :: %d\n", i, dest[i]);

    da_free(arr);
    da_free(dest);
}