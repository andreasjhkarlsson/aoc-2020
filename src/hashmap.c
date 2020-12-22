#include "hashmap.h"
#include "bitarray.h"
#include <string.h>
#include <stdio.h>

struct __attribute__((packed)) index_entry 
{
    bool in_use: 1;
    bool tombstone: 1;
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

struct bucket
{
    struct index_entry* index;
    void* key;
    void* value;
};

struct bucket get_bucket(struct hashmap* map, int index)
{
    #define KV_SIZE(map) (map->key_size+map->value_size)

    struct bucket bucket;
    bucket.index = &((struct index_entry*)map->data)[index];
    uint8_t* kv_start = &map->data[map->capacity * sizeof(struct index_entry)];
    bucket.key = &kv_start[index * KV_SIZE(map)]; 
    bucket.value =  &kv_start[index * KV_SIZE(map) + map->key_size]; 
    return bucket;
}

void hashmap_set(struct hashmap* map, void* key, void* value)
{
    int h = ht_hash(key, map->key_size) % map->capacity;

    for(int i=0;i<map->capacity;i++)
    {
        struct bucket bucket = get_bucket(map, (h + i) % map->capacity);

        if (!bucket.index->in_use || bucket.index->tombstone || (bucket.index->in_use && memcmp(bucket.key, key, map->key_size)==0))
        {
            memcpy(bucket.value, value, map->value_size);
            if (!bucket.index->in_use || bucket.index->tombstone)
            {
                memcpy(bucket.key, key, map->key_size);
                map->count++;
                bucket.index->in_use = true;
            }
            
            bucket.index->tombstone = false;
            return;
        }
    }
    
    printf("Fatal warning: no available slot found in hashmap for key\n");
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
    int h = ht_hash(key, map->key_size) % map->capacity;
    
    for(int i=0;i<map->capacity;i++)
    {
        struct bucket bucket = get_bucket(map, (h + i) % map->capacity);
        if (!bucket.index->in_use)
            return NULL;

        if (bucket.index->tombstone)
            continue;
            
        if (memcmp(bucket.key, key, map->key_size)==0)
            return bucket.value;
    }

    return NULL;
}

size_t hashmap_count(struct hashmap* map)
{
    return map->count;
}

struct hashmap_iterator hashmap_iterate(struct hashmap* map, struct hashmap_iterator* it)
{
    struct hashmap_iterator next = {-1,0,0,0};
    if (it)
        next.index = it->index;
    for(int i=it->index+1;i<map->capacity;i++)
    {
        struct bucket bucket = get_bucket(map, i);
        if (!bucket.index->in_use || bucket.index->tombstone)
            continue;       
        next.index = i;
        next.key = bucket.key;
        next.value = bucket.value;
        return next;
    }

    next.ended = true;
    return next;
}

void hashmap_clear(struct hashmap* map)
{
    if (map->count > 0)
    {
        map->count = 0;
        memset(&map->data[0], 0, map->capacity);
    }
}

void hashmap_copy(struct hashmap* source, struct hashmap* destination)
{
    hashmap_clear(destination);

    struct hashmap_iterator it = hashmap_iterate(source, NULL);
    
    while (!it.ended)
    {
        hashmap_set(destination, it.key, it.value);
        it = hashmap_iterate(source, &it);
    }
}

void hashmap_erase(struct hashmap* map, void* key)
{
    int h = ht_hash(key, map->key_size) % map->capacity;
    
    for(int i=0;i<map->capacity;i++)
    {
        struct bucket bucket = get_bucket(map, (h + i) % map->capacity);
        if (!bucket.index->in_use)
            return;

        if (bucket.index->tombstone)
            continue;
            
        if (memcmp(bucket.key, key, map->key_size)==0)
        {
            bucket.index->tombstone = true;
            map->count--;
            return;
        }
    }

}