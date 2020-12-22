#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdbool.h>

struct hashmap
{
    size_t capacity;
    size_t key_size;
    size_t value_size;
    size_t count;
    uint8_t data[];
};

struct hashmap_iterator
{
    int index;
    void* key;
    void* value; 
    bool ended;
};

void hashmap_init(struct hashmap* map, size_t size, size_t key_size, size_t value_size);

void hashmap_set(struct hashmap* map, void* key, void* value);

void hashmap_set_int(struct hashmap* map, void* key, int value);

int hashmap_get_int(struct hashmap* map, void* key);

void* hashmap_get(struct hashmap* map, void* key);

size_t hashmap_count(struct hashmap* map);

struct hashmap_iterator hashmap_iterate(struct hashmap* map, struct hashmap_iterator* it);

void hashmap_clear(struct hashmap* map);

void hashmap_copy(struct hashmap* source, struct hashmap* destination);

void hashmap_erase(struct hashmap* map, void* key);

#endif

