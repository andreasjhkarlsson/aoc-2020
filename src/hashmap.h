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

void hashmap_init(struct hashmap* map, size_t size, size_t key_size, size_t value_size);

void hashmap_set(struct hashmap* map, void* key, void* value);

void hashmap_set_int(struct hashmap* map, void* key, int value);

int hashmap_get_int(struct hashmap* map, void* key);

void* hashmap_get(struct hashmap* map, void* key);

size_t hashmap_count(struct hashmap* map);


#endif

