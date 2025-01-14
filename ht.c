#include <stdlib.h>
#include <string.h>
#include "ht.h"

#define FNV_PRIME 0x01000193
#define FNV_OFFSET 0x811c9dc5
#define INITIAL_CAPACITY 0x10

// NOTE: this implemenation is not thread-safe

// 32-bit implementation of fnv-1a hash algorithm
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint32_t ht_hash_key(const char *key)
{
    uint32_t hash = FNV_OFFSET;

    for (const char* p = key; *p; p++) {
        hash ^= (uint32_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }

    return hash;
}

ht_t *ht_new(void)
{
    ht_t *ht = malloc(sizeof(ht_t));

    if(ht == NULL)
        return NULL;

    ht->capacity = INITIAL_CAPACITY;
    ht->length = 0;
    ht->entries = calloc(ht->capacity, sizeof(ht_entry_t));

    if(ht->entries == NULL)
        return NULL;
    
    return ht;
}

void ht_free(ht_t *ht)
{
    free(ht->entries);
    free(ht);
}

void *ht_get(ht_t *ht, const char *key)
{
    uint32_t hash = ht_hash_key(key);
    size_t index = (size_t)(hash & (uint32_t)(ht->capacity - 1));

    while(ht->entries[index].key != NULL) {
        if(strcmp(key, ht->entries[index].key) == 0)
            return ht->entries[index].value;

        index++;
    
        if(index >= ht->capacity)
            index = 0;
    }

    return NULL;
}

const char *ht_set(ht_t *ht, const char *key, void *value)
{
    if(ht->length >= ht->capacity / 2)
        if(!ht_grow(ht)) return NULL;

    uint32_t hash = ht_hash_key(key);
    size_t index = (size_t)(hash & (uint32_t)(ht->capacity - 1));

    while(ht->entries[index].key != NULL) {
        if(strcmp(key, ht->entries[index].key) == 0) {
            ht->entries[index].value = value;
            return ht->entries[index].key;
        }

        index++;
    
        if(index >= ht->capacity)
            index = 0;
    }

    ht->entries[index].key = strdup(key);
    ht->entries[index].value = value;
    ht->length++; 

    return ht->entries[index].key;
}

uint8_t ht_grow(ht_t *ht)
{
    ht_t *new_ht = malloc(sizeof(ht_t));
    if(new_ht == NULL)
        return 1;

    // just set capacity and entries, we don't care about length
    // in new_ht
    new_ht->capacity = ht->capacity * 2;
    new_ht->entries = calloc(new_ht->capacity, sizeof(ht_entry_t));
    if(new_ht->entries == NULL)
        return 1;

    for(size_t i = 0; i < ht->capacity; i++) {
        if(ht->entries[i].key != NULL)
            ht_set(new_ht, ht->entries[i].key, ht->entries[i].value);
    }

    free(ht->entries);
    ht->entries = new_ht->entries;
    ht->capacity = new_ht->capacity;

    return 0;
}
