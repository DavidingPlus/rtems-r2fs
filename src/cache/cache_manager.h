#ifndef _CACHE_MANAGER_H_
#define _CACHE_MANAGER_H_

#include <unordered_map>
#include <list>
#include <utility>
#include <tuple>
#include <memory>
#include <system_error>
#include <cassert>


/**
 * @class CacheIndexManager
 * @brief 缓存索引管理器。当缓存项添加到索引后，由索引获得其所有权。
 * @tparam key_t 缓存项具有的唯一标识 key_t 类型。
 * @tparam entry_t 缓存项类型。
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


/**
 * @class CacheLruReplacer
 * @brief 缓存 LRU 替换器。使用 LRU 算法进行置换。
 * @tparam key_t 使用缓存项的 key 用来代表缓存项。
 */
template <typename key_t>
class CacheLruReplacer
{

public:

    // 将 key 加入置换器。默认 key 可进行置换。
    void add(const key_t &key)
    {
        assert(0 == key_states.count(key));
        key_states[key].first = false;
        add_to_list_tail(lru_list, key);
    }

    // 返回可以被置换的缓存项数目。
    size_t get_num_can_replace() { return lru_list.size(); }

    // 进行置换，返回应当被置换的 key。
    key_t pop_replaced()
    {
        assert(lru_list.empty() == false);
        key_t key = lru_list.front();
        assert(key_states.at(key).first == false);
        assert(key_states.at(key).second == lru_list.begin());
        lru_list.pop_front();
        key_states.erase(key);
        return key;
    }

    // 锁住 key，让其不能被置换。如果已经被锁住，什么也不做。
    void pin(const key_t &key)
    {
        auto &element = key_states.at(key);
        if (element.first == true) return;

        element.first = true;
        lru_list.erase(element.second);
        add_to_list_tail(pinned_list, key);
    }

    // 解锁 key，使其可以被置换。unpin 被视为一次访问，会将其放置到 lru 链表尾。如果没有被锁住，什么都不做。
    void unpin(const key_t &key)
    {
        auto &element = key_states.at(key);
        if (element.first == false) return;

        element.first = false;
        pinned_list.erase(element.second);
        add_to_list_tail(lru_list, key);
    }

    // 告知 replacer 对 key 进行了一次访问。
    void access(const key_t &key)
    {
        assert(key_states.count(key) == 1);
        auto &element = key_states[key];

        // 访问被 pin 住的缓存，不用管。
        if (element.first == true) return;

        // 否则移到 lru 链表尾。
        lru_list.erase(element.second);
        add_to_list_tail(lru_list, key);
    }

    // 手动移除 key，无论是否被 pin 住。
    void remove(const key_t &key)
    {
        assert(key_states.count(key) == 1);
        bool pinned;
        list_iterator_t itr;
        std::tie(pinned, itr) = key_states.at(key);

        if (pinned)
            pinned_list.erase(itr);
        else
            lru_list.erase(itr);

        key_states.erase(key);
    }


private:

    using list_iterator_t = typename std::list<key_t>::iterator;


    void add_to_list_tail(std::list<key_t> &ls, const key_t &key)
    {
        ls.emplace_back(key);
        key_states[key].second = --ls.end();
    }


    /*
     * lru 链表 lru_list，不能置换的链表 pinned_list。
     * lru 链表中，首元素为最近最少访问的元素。
     */
    std::list<key_t> lru_list, pinned_list;

    /*
     * map 的 value: first 为当前该缓存项是否被 pin 住(true 则被 pin 住)，second 为 list 的 iterator。
     * 同一时刻，根据缓存项是否被 pin 住，该 iterator 指向的元素位于 lru_list(unpin) 或 pinned_list(pin) 二者之一。
     */
    std::unordered_map<key_t, std::pair<bool, list_iterator_t>> key_states;
};


#endif
