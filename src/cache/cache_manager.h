#ifndef _CACHE_MANAGER_H_
#define _CACHE_MANAGER_H_

#include <unordered_map>
#include <list>
#include <utility>
#include <tuple>
#include <memory>
#include <system_error>
#include <cassert>


/*
 * @class CacheIndexManager
 * @brief 缓存索引管理器。
 * 缓存项类型为 entry_t，要求缓存项具有唯一标识 key_t 类型。
 * 当缓存项添加到索引后，由索引获得其所有权。
 */
template <typename key_t, typename entry_t>
class CacheIndexManager
{

public:

    use iterator_t = typename std::unordered_map<key_t, std::unique_ptr<entry_t>>::iterator;


    void add(const key_t &key, std::unique_ptr<entry_t> &p_entry)
    {
        assert(0 == index.count(key));
        index.emplace(key, p_entry.release());
    }

    void add(const key_t &key, std::unique_ptr<entry_t> &&p_entry)
    {
        assert(0 == index.count(key));
        index.emplace(key, p_entry);
    }

    std::unique_ptr<entry_t> remove(const key_t &key)
    {
        assert(1 == index.count(key));
        std::unique_ptr<entry_t> ret = std::move(index[key]);
        index.erase(key);
        return ret;
    }

    entry_t *get(const key_t &key) { return 0 == index.count(key) ? nullptr : index[key].get(); }

    iterator_t begin() { return index.begin(); }

    iterator_t end() { return index.end(); }

    iterator_t erase(const iterator_t pos) { return index.erase(pos); }


private:

    std::unordered_map<key_t, std::unique_ptr<entry_t>> index;
};


#endif
