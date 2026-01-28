#include "cache_index_manager.h"

#include <assert.h>


void cacheIndexManagerInit(CacheIndexManager *cim)
{
    cim->index = NULL;
}

void cacheIndexManagerDestroy(CacheIndexManager *cim)
{
    CacheEntry *entry, *tmp;
    HASH_ITER(hh, cim->index, entry, tmp)
    {
        HASH_DEL(cim->index, entry);
        free(entry->value); // 释放 value
        free(entry);        // 释放条目
    }
    cim->index = NULL;
}

void cacheIndexManagerAdd(CacheIndexManager *cim, uint32_t key, void *value)
{
    assert(cacheIndexManagerGet(cim, key) == NULL); // key 不能重复

    CacheEntry *entry = (CacheEntry *)malloc(sizeof(CacheEntry));
    entry->key = key;
    entry->value = value;

    HASH_ADD_INT(cim->index, key, entry);
}

void *cacheIndexManagerGet(CacheIndexManager *cim, uint32_t key)
{
    CacheEntry *entry;

    HASH_FIND_INT(cim->index, &key, entry);


    return entry ? entry->value : NULL;
}

void *cacheIndexManagerRemove(CacheIndexManager *cim, uint32_t key)
{
    CacheEntry *entry;

    HASH_FIND_INT(cim->index, &key, entry);
    if (!entry) return NULL;

    void *value = entry->value;
    HASH_DEL(cim->index, entry);

    free(entry);


    return value;
}

void cacheIndexManagerErase(CacheIndexManager *cim, CacheEntry *cacheEntry)
{
    if (!cacheEntry) return;

    HASH_DEL(cim->index, cacheEntry);

    free(cacheEntry->value);
    free(cacheEntry);
}
