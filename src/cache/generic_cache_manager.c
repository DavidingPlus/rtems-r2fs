#include "generic_cache_manager.h"

#include <assert.h>


void genericCacheManagerInit(GenericCacheManager *gcm)
{
    assert(gcm);

    cacheIndexManagerInit(&gcm->index);
    cacheLruReplacerInit(&gcm->replacer);
}

void genericCacheManagerDestroy(GenericCacheManager *gcm)
{
    assert(gcm);

    cacheLruReplacerDestroy(&gcm->replacer);
    cacheIndexManagerDestroy(&gcm->index);
}

void genericCacheManagerAdd(GenericCacheManager *gcm, uint32_t key, void *entry)
{
    assert(gcm);
    assert(entry);

    cacheIndexManagerAdd(&gcm->index, key, entry);
    cacheLruReplacerAdd(&gcm->replacer, key);
}

void *genericCacheManagerGet(GenericCacheManager *gcm, uint32_t key, bool is_access)
{
    assert(gcm);

    void *entry = cacheIndexManagerGet(&gcm->index, key);
    if (entry && is_access) cacheLruReplacerAccess(&gcm->replacer, key);


    return entry;
}

void *genericCacheManagerReplaceOne(GenericCacheManager *gcm)
{
    assert(gcm);

    if (!cacheLruReplacerCanReplace(&gcm->replacer)) return NULL;

    uint32_t key = cacheLruReplacerPop(&gcm->replacer);


    return cacheIndexManagerRemove(&gcm->index, key);
}

void *genericCacheManagerRemove(GenericCacheManager *gcm, uint32_t key)
{
    assert(gcm);

    cacheLruReplacerRemove(&gcm->replacer, key);


    return cacheIndexManagerRemove(&gcm->index, key);
}
