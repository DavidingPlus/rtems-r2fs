#include "cache_lru_replacer.h"

#include <stdlib.h>


static void detachNode(CacheLruReplacer *lru, LruNode *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        lru->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        lru->tail = node->prev;

    node->prev = node->next = NULL;
}

static void attachTail(CacheLruReplacer *lru, LruNode *node)
{
    node->prev = lru->tail;
    node->next = NULL;

    if (lru->tail)
        lru->tail->next = node;
    else
        lru->head = node;

    lru->tail = node;
}


void cacheLruReplacerInit(CacheLruReplacer *lru)
{
    assert(lru);
    lru->head = NULL;
    lru->tail = NULL;
    lru->table = NULL;
    lru->size = 0;
}

void cacheLruReplacerDestroy(CacheLruReplacer *lru)
{
    assert(lru);

    LruEntry *entry, *tmp;
    HASH_ITER(hh, lru->table, entry, tmp)
    {
        HASH_DEL(lru->table, entry);
        free(entry->node);
        free(entry);
    }

    lru->head = lru->tail = NULL;
    lru->table = NULL;
    lru->size = 0;
}

void cacheLruReplacerAdd(CacheLruReplacer *lru, void *key)
{
    assert(lru);

    LruEntry *entry = NULL;
    HASH_FIND_PTR(lru->table, &key, entry);
    assert(entry == NULL);

    LruNode *node = (LruNode *)malloc(sizeof(LruNode));
    assert(node);
    node->key = key;
    node->prev = node->next = NULL;

    attachTail(lru, node);

    entry = (LruEntry *)malloc(sizeof(LruEntry));
    assert(entry);
    entry->key = key;
    entry->node = node;

    HASH_ADD_PTR(lru->table, key, entry);
    ++lru->size;
}

void cacheLruReplacerAccess(CacheLruReplacer *lru, void *key)
{
    assert(lru);

    LruEntry *entry = NULL;
    HASH_FIND_PTR(lru->table, &key, entry);
    assert(entry);

    LruNode *node = entry->node;

    if (node == lru->tail) return;

    detachNode(lru, node);
    attachTail(lru, node);
}

int cacheLruReplacerCanReplace(CacheLruReplacer *lru)
{
    assert(lru);
    return lru->size > 0;
}

void *cacheLruReplacerPop(CacheLruReplacer *lru)
{
    assert(lru);
    assert(lru->size > 0);

    LruNode *node = lru->head;
    void *key = node->key;

    LruEntry *entry = NULL;
    HASH_FIND_PTR(lru->table, &key, entry);
    assert(entry);

    detachNode(lru, node);
    HASH_DEL(lru->table, entry);

    free(node);
    free(entry);

    --lru->size;
    return key;
}

void cacheLruReplacerRemove(CacheLruReplacer *lru, void *key)
{
    assert(lru);

    LruEntry *entry = NULL;
    HASH_FIND_PTR(lru->table, &key, entry);
    assert(entry);

    LruNode *node = entry->node;

    detachNode(lru, node);
    HASH_DEL(lru->table, entry);

    free(node);
    free(entry);

    --lru->size;
}
