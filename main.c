#include <stdio.h>

#define DARRAY_IMPL
#include "containers/darray.h"

#define STRING_IMPL
#include "containers/string.h"

#include "containers/dictionary.h"

void da_test();
void string_test();
void dict_test();

int main()
{
    da_test();
    printf("\n");
    string_test();
    printf("\n");
    dict_test();
}

void da_test()
{
    printf("--- DArray Test ---\n");

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

    printf("\nTest Successful!\n");
}

void string_test()
{
    printf("--- String Test ---\n");

    String str = string_make("Hello World!");
    printf("str :: %s\n", str);
    
    
    String cpy = NULL;
    string_copy(&cpy, str);

    printf("cpy :: %s\n", cpy);
    printf("cpy :: length: %zd\n", string_length(cpy));

    string_free(&str);
    string_free(&cpy);

    printf("\nTest Successful!\n");
}

void dict_test()
{
    printf("--- Dictionary Test ---\n");

    Dict(int) dict;
    dict_make(dict);

    dict_put(dict, "One", 1);
    dict_put(dict, "Two", 2);
    dict_put(dict, "Three", 3);
    dict_put(dict, "Four", 4);

    printf("dict.cap : %zd\n", dict_cap(dict));

    printf("dict[\"One\"]   :: %d\n", dict_get(dict, "One"));
    printf("dict[\"Two\"]   :: %d\n", dict_get(dict, "Two"));
    printf("dict[\"Three\"] :: %d\n", dict_get(dict, "Three"));
    printf("dict[\"Four\"]  :: %d\n", dict_get(dict, "Four"));

    dict_remove(dict, "Three");
    if (dict_find(dict, "Three") == dict_end(dict))
        printf("\nRemoved dict[\"Three\"]\n\n");
    else
        printf("\ndict[\"Three\"] not removed.\n\n");

    dict_foreach(int, it, dict)
        printf("dict[\"%s\"] :: %d\n", it->key, it->value);

    dict_free(dict);
 
    printf("\nTest Successful!\n");
}