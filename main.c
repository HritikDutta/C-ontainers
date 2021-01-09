#include <stdio.h>

#define DARRAY_IMPL
#include "containers/darray.h"

#define STRING_IMPL
#include "containers/string.h"

void da_test();
void string_test();

int main()
{
    da_test();
    printf("\n");
    string_test();
}

void da_test()
{
    printf("---DArray Test---\n");

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

    printf("\nFreed successfully!\n");
}

void string_test()
{
    printf("---String Test---\n");

    String str = string_make("Hello World!");
    printf("str :: %s\n", str);
    
    
    String cpy = NULL;
    string_copy(&cpy, str);

    printf("cpy :: %s\n", cpy);
    printf("cpy :: length: %zd\n", string_length(cpy));

    string_free(&str);
    string_free(&cpy);

    printf("\nFreed successfully!\n");
}