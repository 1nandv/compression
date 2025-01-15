#ifndef HT_H
#define HT_H

#include <stdint.h>

typedef struct {
    char *key;
    void *value;
} ht_entry_t;

typedef struct {
    size_t capacity;
    size_t length;
    ht_entry_t* entries;
} ht_t;

// todo: implement iterator
typedef struct {
    ht_t *ht;
    size_t index;
} ht_iter_t;

static uint32_t ht_hash_key(const char *key);

ht_t *ht_new(void);
void ht_free(ht_t *ht);

void *ht_get(ht_t *ht, const char *key);
const char *ht_set(ht_t *ht, const char *key, void *value);
uint8_t ht_grow(ht_t *ht);

ht_iter_t *ht_iter_new(ht_t *ht);
void ht_iter_free(ht_iter_t *ht_iter);
ht_entry_t *ht_iter_next(ht_iter_t *ht_iter);

#endif
