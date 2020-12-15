#include "hashmap.h"
#include "bitarray.h"
#include <string.h>
#include <stdio.h>

struct __attribute__((packed)) index_entry 
{
    bool in_use;
};

uint32_t ht_hash(void* x, size_t length) {
    // http://www.cse.yorku.ca/~oz/hash.html
    uint32_t hash = 5381;
    
    for(int i=0;i<length;i++)
        hash = ((hash << 5) + hash) + ((uint8_t*)x)[i]; /* hash * 33 + c */

    return hash;
}

void hashmap_init(struct hashmap* map, size_t size, size_t key_size, size_t value_size)
{
    memset(map, 0, size);    
    map->capacity = (size - sizeof(struct hashmap)) / (key_size + value_size + sizeof(struct index_entry));
    map->key_size = key_size;
    map->value_size = value_size;
    map->count = 0;
}

#define PAIR_SIZE(map) (map->key_size+map->value_size)

void hashmap_set(struct hashmap* map, void* key, void* value)
{
    int index = ht_hash(key, map->key_size) % map->capacity;

    
    for(int i=0;i<map->capacity;i++)
    {
        int offset = (index + i) % map->capacity;
        struct index_entry* index_entry = &((struct index_entry*)map->data)[offset];
        uint8_t* kv_start = &map->data[map->capacity * sizeof(struct index_entry)];
        uint8_t* key_entry = &kv_start[offset * PAIR_SIZE(map)]; 

        if (!index_entry->in_use || (index_entry->in_use && memcmp(key_entry, key, map->key_size)==0))
        {
            memcpy(key_entry + map->key_size, value, map->value_size);
            if (!index_entry->in_use)
            {
                memcpy(key_entry, key, map->key_size);
                map->count++;
            }
            
            index_entry->in_use = true;
            return;
        }

    }
    
    printf("Warning: no available slot found in hashmap for key");
}

void hashmap_set_int(struct hashmap* map, void* key, int value)
{
    hashmap_set(map, key, &value);
}

int hashmap_get_int(struct hashmap* map, void* key)
{
    int* a = hashmap_get(map, key);
    return *a;
}

void* hashmap_get(struct hashmap* map, void* key)
{
    int index = ht_hash(key, map->key_size) % map->capacity;
    
    for(int i=0;i<map->capacity;i++)
    {
        int offset = (index + i) % map->capacity;
        struct index_entry* index_entry = &((struct index_entry*)map->data)[offset];
        uint8_t* kv_start = &map->data[map->capacity * sizeof(struct index_entry)];
        uint8_t* key_entry = &kv_start[offset * PAIR_SIZE(map)]; 
        if (!index_entry->in_use)
        {
            return NULL;
        }
        if ((index_entry->in_use && memcmp(key_entry, key, map->key_size)==0))
        {
            return key_entry + map->key_size;
        }
    }

    return NULL;
}

size_t hashmap_count(struct hashmap* map)
{
    return map->count;
}