#include "cache_index_manager.h"


void cacheIndexManagerInit(CacheIndexManager *pCim)
{
    pCim->index = NULL;
}

void cacheIndexManagerDestroy(CacheIndexManager *pCim)
{
    CacheEntry *entry, *tmp;
    HASH_ITER(hh, pCim->index, entry, tmp)
    {
        HASH_DEL(pCim->index, entry);
        free(entry->value); // 释放 value
        free(entry);        // 释放条目
    }
    pCim->index = NULL;
}

void cacheIndexManagerAdd(CacheIndexManager *pCim, void *key, void *value)
{
    assert(cacheIndexManagerGet(pCim, key) == NULL); // key 不能重复

    CacheEntry *entry = (CacheEntry *)malloc(sizeof(CacheEntry));
    entry->key = key;
    entry->value = value;
    HASH_ADD_PTR(pCim->index, key, entry); // 按指针比较 key
}

void *cacheIndexManagerGet(CacheIndexManager *pCim, void *key)
{
    CacheEntry *entry;
    HASH_FIND_PTR(pCim->index, &key, entry);
    return entry ? entry->value : NULL;
}

void *cacheIndexManagerRemove(CacheIndexManager *pCim, void *key)
{
    CacheEntry *entry;
    HASH_FIND_PTR(pCim->index, &key, entry);
    if (!entry) return NULL;

    void *value = entry->value;
    HASH_DEL(pCim->index, entry);
    free(entry);
    return value;
}

void cacheIndexManagerErase(CacheIndexManager *pCim, CacheEntry *pCacheEntry)
{
    if (!pCacheEntry) return;
    HASH_DEL(pCim->index, pCacheEntry);
    free(pCacheEntry->value);
    free(pCacheEntry);
}
