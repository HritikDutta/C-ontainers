#include <stdio.h>

#define DARRAY_IMPL
#include "containers/darray.h"

int main()
{
    DArray(int) a;
    da_make_with_size(a, 5);

    DArray(int) arr = make_darray(5);
    da_insert(arr, 0, 4);
    da_push_back(arr, 3);
    
    DArray(int) dest = NULL;
    da_copy(dest, arr);

    for (int i = 0; i < da_size(dest); i++)
    {
        printf("%d\n", dest[i]);
    }

    da_free(arr);
}