#ifndef STRING_STORE_H
#define STRING_STORE_H
#include <string.h>

extern char __string_storage[];

// pinned_string is guaranteed to have static lifetime and 
// can be compared directly (i.e. the same string always maps to the same memory)

static inline const char* string_storage_pin(const char *str)
{
    size_t len = strlen(str);

    if (*str=='\0')
        return "";

    char* ptr = __string_storage;
    size_t* size;
    while ( *(size=(size_t*)ptr)>0 )
    {
        ptr += sizeof(size_t);
        if (*size == len+1 && strcmp(str, ptr)==0)
            return ptr;
        ptr += *size;
    }

    *size = len + 1;
    ptr += sizeof(size_t);
    memcpy(ptr, str, *size);
    return ptr;
}

#define PIN_STRING(str) string_storage_pin(str)
#define STRING_STORAGE(N) char __string_storage[N] = { '\0' };

typedef const char* pinned_string;

#endif