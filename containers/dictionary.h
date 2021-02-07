#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>

#include "hd_assert.h"
#include "string.h"

#ifndef DICT_GROWTH_RATE
#define DICT_GROWTH_RATE 2
#endif // DICT_GROWTH_RATE

#ifndef DICT_START_CAP
#define DICT_START_CAP 16
#endif // DICT_START_CAP

#ifndef DICT_MAX_LOAD
#define DICT_MAX_LOAD 0.75
#endif // DICT_MAX_LOAD

inline size_t dict_string_hasher(String key)
{
    return 0;
}

#define Dict(type) \
    struct              \
    {                   \
        size_t cap;     \
        size_t filled;  \
                        \
        struct          \
        {               \
            String key; \
            type value; \
        }* buckets;     \
    }

void* dict_allocate_buckets(size_t cap, size_t bkt_size)
{
    return calloc(cap, bkt_size);
}

typedef struct
{
    String key;
    char value[];
} Dict_Bucket_Internal;

typedef struct
{
    size_t index;
    void* ptr;
} Dict_Itr;

#define dict_bucket_at(buckets, index, bkt_size) (void*)((char*) buckets + index * bkt_size)

Dict_Itr dict_find_bucket(void* buckets, size_t cap, size_t bkt_size, String key)
{
    size_t index = dict_string_hasher(key) % cap;
    size_t start = index;

    do
    {
        Dict_Bucket_Internal* bkt = dict_bucket_at(buckets, index, bkt_size);

        if (bkt->key == NULL)
            break;
        
        if (string_cmp(bkt->key, key))
            return (Dict_Itr){ index, bkt };

        index = (index + 1) % cap;
    } while (index != start);

    return (Dict_Itr){ cap, dict_bucket_at(buckets, cap, bkt_size) };
}

#define dict_make(dict) \
    do                                                                \
    {                                                                 \
        dict.cap = DICT_START_CAP;                                    \
        dict.filled = 0;                                              \
        dict.buckets = calloc(DICT_START_CAP, sizeof(*dict.buckets)); \
    } while (0)
    

// @Todo: Rehash based on load factor
#define dict_put(dict, _key, _value) \
    do {                                                     \
        size_t index = dict_string_hasher(_key) % dict.cap;  \
        size_t start = index;                                \
                                                             \
        do                                                   \
        {                                                    \
            if (dict.buckets[index].key == NULL)             \
            {                                                \
                dict.buckets[index].key = string_make(_key); \
                dict.buckets[index].value = _value;          \
                break;                                       \
            }                                                \
                                                             \
            if (string_cmp(dict.buckets[index].key, _key))   \
            {                                                \
                dict.buckets[index].value = _value;          \
                break;                                       \
            }                                                \
                                                             \
            index++;                                         \
        } while (index != start);                            \
                                                             \
    } while (0)

#define dict_free(dict) \
    do                                        \
    {                                         \
        if (dict.buckets) free(dict.buckets); \
        dict.cap = dict.filled = 0;           \
        dict.buckets = NULL;                  \
    } while (0)

#define dict_find(dict, _key) ((dict.buckets) ? (dict_find_bucket(dict.buckets, dict.cap, sizeof(*dict.buckets), _key)).ptr : NULL)
#define dict_begin(dict) ((dict.buckets) ? dict_bucket_at(dict.buckets, 0, sizeof(*dict.buckets)) : NULL)
#define dict_end(dict)   ((dict.buckets) ? dict_bucket_at(dict.buckets, dict.cap, sizeof(*dict.buckets)) : NULL)

#define dict_get(dict, _key) ((dict.buckets) ? dict.buckets[(dict_find_bucket(dict.buckets, dict.cap, sizeof(*dict.buckets), _key)).index].value : 0)

/*

Dict(int) dict;
dict_make(dict);

dict_put(dict, "Hello", something);

if (dict_find(dict, "Hello") != dict_end(dict))
{
    int hello = dict_get(dict, "Hello");
    ...
}

dict_free(dict);

*/

#endif // DICTIONARY_H