#ifndef _CACHE_LRU_REPLACER_H_
#define _CACHE_LRU_REPLACER_H_

#include "uthash/uthash.h"


/**
 * @brief LRU 链表节点。
 */
typedef struct LruNode
{
    uint32_t key;
    struct LruNode *prev;
    struct LruNode *next;
} LruNode;

/**
 * @brief 哈希表条目：key -> LruNode。
 * @details 因为需要 O(1) 的时间复杂度从 key 定位到链表节点，所以需要额外开辟一个哈希表存储 key 到 LRU 链表的映射关系。
 */
typedef struct LruEntry
{
    uint32_t key;  // 按指针比较。
    LruNode *node; // 指向链表节点。
    UT_hash_handle hh;
} LruEntry;

/**
 * @brief LRU 置换器。
 */
typedef struct CacheLruReplacer
{
    LruNode *head;   // 最久未访问。
    LruNode *tail;   // 最近访问。
    LruEntry *table; // uthash 表头。
    size_t size;     // 当前可置换数量。（在无 pin 版本的时候和链表的长度完全一致。）
} CacheLruReplacer;


/**
 * @brief 初始化 LRU replacer。
 */
void cacheLruReplacerInit(CacheLruReplacer *this);

/**
 * @brief 销毁 LRU replacer（不释放 key）。
 */
void cacheLruReplacerDestroy(CacheLruReplacer *this);

/**
 * @brief 添加 key（默认视为最近访问）。
 */
void cacheLruReplacerAdd(CacheLruReplacer *this, uint32_t key);

/**
 * @brief 访问 key（刷新 LRU 顺序）。
 */
void cacheLruReplacerAccess(CacheLruReplacer *this, uint32_t key);

/**
 * @brief 是否存在可被置换的 key。
 */
int cacheLruReplacerCanReplace(CacheLruReplacer *this);

/**
 * @brief 弹出一个最久未访问的 key。
 */
uint32_t cacheLruReplacerPop(CacheLruReplacer *this);

/**
 * @brief 手动移除指定 key。
 */
void cacheLruReplacerRemove(CacheLruReplacer *this, uint32_t key);


#endif
