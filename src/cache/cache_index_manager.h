#ifndef _CACHEIndexManagerINDEX_MANAGER_H_
#define _CACHEIndexManagerINDEX_MANAGER_H_

#include <stdlib.h>
#include <assert.h>

#include "uthash/uthash.h"


/**
 * @brief 缓存条目。
 */
typedef struct CacheEntry
{
    void *key;         // 任意类型 key（按指针比较）。
    void *value;       // 任意类型 value（类似 unique_ptr，index 拥有）。
    UT_hash_handle hh; // uthash 必须要求需要的字段。
} CacheEntry;

/**
 * @brief 缓存索引管理器。
 */
typedef struct CacheIndexManager
{
    CacheEntry *index; // 哈希表头指针。
} CacheIndexManager;


/**
 * @brief 初始化缓存索引。
 */
void cacheIndexManagerInit(CacheIndexManager *pCim);

/**
 * @brief 销毁缓存索引，释放所有 value 内存。
 */
void cacheIndexManagerDestroy(CacheIndexManager *pCim);

/**
 * @brief 添加缓存项。
 */
void cacheIndexManagerAdd(CacheIndexManager *pCim, void *key, void *value);

/**
 * @brief 获取缓存项。
 */
void *cacheIndexManagerGet(CacheIndexManager *pCim, void *key);

/**
 * @brief 移除缓存项，返回 value，由调用者决定是否释放。
 */
void *cacheIndexManagerRemove(CacheIndexManager *pCim, void *key);

/**
 * @brief 删除指定条目（迭代器风格）。
 */
void cacheIndexManagerErase(CacheIndexManager *pCim, CacheEntry *pCacheEntry);

/**
 * @brief 遍历缓存的宏。
 */
#define CACHE_INDEX_MANAGER_FOREACH(cache, entry_var) \
    CacheEntry *entry_var, *tmp_##entry_var;          \
    HASH_ITER(hh, (cache)->index, entry_var, tmp_##entry_var)


#endif
