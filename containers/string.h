#ifndef CONTAINER_STRING_H
#define CONTAINER_STRING_H

typedef char* String;

String string_make(char* cstr);
void   string_copy(String* dest, String src);
void   string_free(String* str);

size_t string_length(String str);

#endif // CONTAINER_STRING_H

#include <stdlib.h>
#include <string.h>

#include "hd_assert.h"

typedef struct
{
    size_t length;
    char   buffer[];
} String_Internal;

#define string_data(str) ((String_Internal*)(str) - 1)

String string_make(char* cstr)
{
    size_t len = strlen(cstr);
    String_Internal* s = (String_Internal*) malloc(len * sizeof(char) + sizeof(String));
    hd_assert(s != NULL);
    
    s->length = len;
    strcpy(s->buffer, cstr);
    return s->buffer;
}

void string_copy(String* dest, String src)
{
    hd_assert(src != NULL);
    String_Internal* src_str = string_data(src);
    size_t byte_size = src_str->length * sizeof(char) + sizeof(String_Internal);
    
    String_Internal* dest_str;
    if (*dest) dest_str = (String_Internal*) realloc(string_data(*dest), byte_size);
    else       dest_str = (String_Internal*) malloc(byte_size);

    hd_assert(dest_str != NULL);
    dest_str->length = src_str->length;
    strcpy(dest_str->buffer, src_str->buffer);

    *dest = dest_str->buffer;
}

void string_free(String* str)
{
    hd_assert(*str);
    String_Internal* s = string_data(*str);
    free(s);
    *str = NULL;
}

size_t string_length(String str)
{
    if (!str) return 0;
    return string_data(str)->length;
}