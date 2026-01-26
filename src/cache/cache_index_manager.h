#ifndef _CACHE_INDEX_MANAGER_H_
#define _CACHE_INDEX_MANAGER_H_

#include <stdlib.h>
#include <assert.h>

#include "uthash/uthash.h"


/**
 * @brief 缓存条目。
 */
typedef struct CacheEntry
{
    /*
     * TODO CacheEntry 的 key 设计。
     *
     * 当前实现：
     *  - key 类型为 void*，在哈希表中存储的是指针值（地址），不是 key 指向对象的内容。
     *  - 管理器不托管 key，仅存储指针；value 托管，由管理器负责释放。
     *  - 调用者必须保证 key 在缓存存在期间有效：
     *        a) 使用堆分配对象（malloc）作为 key
     *        b) 或使用静态/全局对象
     *  - 栈对象不能直接作为 key，否则会导致未定义行为（崩溃）。
     *
     * 潜在问题：
     *  1. 栈对象作为 key 会 UB，尤其在 destroy 或 LRU 替换时。
     *  2. key 指针失效（被 free 或超出作用域）会导致缓存查找失败或访问非法内存。
     *  3. 对于复杂对象 key，调用者必须管理内存，否则可能泄漏或重复 free。
     *
     * 可选方案对比：
     *  - 方案 A：当前实现（存 pointer，不托管 key）
     *       优点：简单、效率高、灵活，可支持任意对象
     *       缺点：调用者必须保证 key 生命周期，栈对象不能用
     *
     *  - 方案 B：托管 key（内部 malloc + memcpy）
     *       优点：调用者可使用栈对象，管理器统一管理 key 内存
     *       缺点：增加接口复杂度，需要提供 key 大小，性能稍低
     *
     *  - 方案 C：仅支持固定类型 key（如 int / struct），按值存储
     *       优点：安全，不依赖堆，支持栈变量
     *       缺点：不够泛型，无法支持任意对象类型
     *
     */
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
void cacheIndexManagerInit(CacheIndexManager *cim);

/**
 * @brief 销毁缓存索引，释放所有 value 内存。
 */
void cacheIndexManagerDestroy(CacheIndexManager *cim);

/**
 * @brief 添加缓存项。
 */
void cacheIndexManagerAdd(CacheIndexManager *cim, void *key, void *value);

/**
 * @brief 获取缓存项。
 */
void *cacheIndexManagerGet(CacheIndexManager *cim, void *key);

/**
 * @brief 移除缓存项，返回 value，由调用者决定是否释放。
 */
void *cacheIndexManagerRemove(CacheIndexManager *cim, void *key);

/**
 * @brief 删除指定条目（迭代器风格）。
 */
void cacheIndexManagerErase(CacheIndexManager *cim, CacheEntry *cacheEntry);


#endif
