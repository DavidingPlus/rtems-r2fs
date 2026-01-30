#include "cache_lru_replacer.h"

#include <stdlib.h>
#include <assert.h>


static void detachNode(CacheLruReplacer *this, LruNode *node);

static void attachTail(CacheLruReplacer *this, LruNode *node);


void cacheLruReplacerInit(CacheLruReplacer *this)
{
    assert(this);
    this->head = NULL;
    this->tail = NULL;
    this->table = NULL;
    this->size = 0;
}

void cacheLruReplacerDestroy(CacheLruReplacer *this)
{
    assert(this);

    LruEntry *entry, *tmp;
    HASH_ITER(hh, this->table, entry, tmp)
    {
        HASH_DEL(this->table, entry);
        free(entry->node);
        free(entry);
    }

    this->head = this->tail = NULL;
    this->table = NULL;
    this->size = 0;
}

void cacheLruReplacerAdd(CacheLruReplacer *this, uint32_t key)
{
    assert(this);

    LruEntry *entry = NULL;
    HASH_FIND_INT(this->table, &key, entry);
    assert(entry == NULL);

    LruNode *node = (LruNode *)malloc(sizeof(LruNode));
    assert(node);
    node->key = key;
    node->prev = node->next = NULL;

    attachTail(this, node);

    entry = (LruEntry *)malloc(sizeof(LruEntry));
    assert(entry);
    entry->key = key;
    entry->node = node;

    HASH_ADD_INT(this->table, key, entry);
    ++this->size;
}

void cacheLruReplacerAccess(CacheLruReplacer *this, uint32_t key)
{
    assert(this);

    LruEntry *entry = NULL;
    HASH_FIND_INT(this->table, &key, entry);
    assert(entry);

    LruNode *node = entry->node;
    if (node == this->tail) return;

    detachNode(this, node);
    attachTail(this, node);
}

int cacheLruReplacerCanReplace(CacheLruReplacer *this)
{
    assert(this);


    return this->size > 0;
}

uint32_t cacheLruReplacerPop(CacheLruReplacer *this)
{
    assert(this);
    assert(this->size > 0);

    LruNode *node = this->head;
    uint32_t key = node->key;

    LruEntry *entry = NULL;
    HASH_FIND_INT(this->table, &key, entry);
    assert(entry);

    detachNode(this, node);
    HASH_DEL(this->table, entry);

    free(node);
    free(entry);

    --this->size;


    return key;
}

void cacheLruReplacerRemove(CacheLruReplacer *this, uint32_t key)
{
    assert(this);

    LruEntry *entry = NULL;
    HASH_FIND_INT(this->table, &key, entry);
    assert(entry);

    LruNode *node = entry->node;

    detachNode(this, node);
    HASH_DEL(this->table, entry);

    free(node);
    free(entry);

    --this->size;
}


void detachNode(CacheLruReplacer *this, LruNode *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        this->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        this->tail = node->prev;

    node->prev = node->next = NULL;
}

void attachTail(CacheLruReplacer *this, LruNode *node)
{
    node->prev = this->tail;
    node->next = NULL;

    if (this->tail)
        this->tail->next = node;
    else
        this->head = node;

    this->tail = node;
}
