#pragma once

#include <memory>
#include <utility>
#include <cstddef>

#include <vector>
#include <list>

template <typename K, typename Hasher = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashSet
{
private:
    static constexpr std::size_t MIN_BUCKETS = 16;
    static constexpr double LOAD_FACTOR = 0.8;

private:
    using entry = K;
    using chain = std::pair<typename std::list<entry>::iterator, std::size_t>;

    std::list<entry> data;
    std::vector<chain> table;

    Hasher hasher{};
    KeyEqual key_equal;

public:
    using size_type = std::size_t;
    using iterator = typename std::list<entry>::iterator;
    using const_iterator = typename std::list<entry>::const_iterator;

public:
    class HashSetIterator
    {
    private:
        friend class HashSet<K, Hasher, KeyEqual>;

        iterator currElement;

    public:
        HashSetIterator(iterator currElement) : currElement(currElement) {}

        HashSetIterator& operator++()
        {
            ++currElement;
            return *this;
        }

        HashSetIterator operator++(int)
        {
            HashSetIterator temp = *this;
            ++(*this);
            return temp;
        }

        entry& operator*()
        {
            return *currElement;
        }

        entry* operator->()
        {
            return &(*currElement);
        }

        bool operator==(const HashSetIterator& other) const
        {
            return currElement == other.currElement;
        }

        bool operator!=(const HashSetIterator& other) const
        {
            return !(*this  == other);
        }
    };

    class ConstHashSetIterator
    {
    private:
        friend class HashSet<K, Hasher, KeyEqual>;

        const_iterator currElement;

    public:
        ConstHashSetIterator(const_iterator currElement) : currElement(currElement) {}

        ConstHashSetIterator& operator++()
        {
            ++currElement;
            return *this;
        }

        ConstHashSetIterator operator++(int)
        {
            ConstHashSetIterator temp = *this;
            ++(*this);
            return temp;
        }

        const entry& operator*() const
        {
            return *currElement;
        }

        const entry* operator->() const
        {
            return &(*currElement);
        }

        bool operator==(const ConstHashSetIterator& other) const
        {
            return currElement == other.currElement;
        }

        bool operator!=(const ConstHashSetIterator& other) const
        {
            return !(*this  == other);
        }
    };

    HashSetIterator begin()
    {
        return HashSetIterator(data.begin());
    }

    HashSetIterator end()
    {
        return HashSetIterator(data.end());
    }

    ConstHashSetIterator cbegin()
    {
        return ConstHashSetIterator(data.cbegin());
    }

    ConstHashSetIterator cend()
    {
        return ConstHashSetIterator(data.cend());
    }

    explicit HashSet(size_type bucket_count = MIN_BUCKETS,
                     const Hasher& hasher = Hasher(),
                     const KeyEqual& equal = KeyEqual());

    size_type size() const noexcept;
    bool empty() const noexcept;

    std::pair<HashSetIterator, bool> insert(const K& key);
    std::pair<HashSetIterator, bool> insert(K&& key);

    HashSetIterator erase(const K& key);
    HashSetIterator erase(const HashSetIterator& iter);

    HashSetIterator find(const K& key);
    ConstHashSetIterator find(const K& key) const;

    bool contains(const K& key) const;

    size_type count(const K& key) const;

private:
    void rehash(size_type n);

    iterator getElementByChain(size_type chainIdx, const K& key);
    const_iterator getElementByChain(size_type chainIdx, const K& key) const;

    size_type hash(const K& key) const;
};

template <typename K, typename Hasher, typename KeyEqual>
HashSet<K, Hasher, KeyEqual>::HashSet(size_type bucket_count,
                                         const Hasher& hasher,
                                         const KeyEqual& equal)
    : data{}, 
    table(bucket_count, chain{ data.end(), 0 }), 
    hasher(hasher), 
    key_equal(equal)
{
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::size_type HashSet<K, Hasher, KeyEqual>::size() const noexcept
{
    return data.size();
}

template <typename K, typename Hasher, typename KeyEqual>
inline bool HashSet<K, Hasher, KeyEqual>::empty() const noexcept
{
    return size() == 0;
}

template <typename K, typename Hasher, typename KeyEqual>
inline std::pair<typename HashSet<K, Hasher, KeyEqual>::HashSetIterator, bool> HashSet<K, Hasher, KeyEqual>::insert(const K& key)
{
    size_type chainIdx = hash(key);
    
    auto foundIter = getElementByChain(chainIdx, key);
    if (foundIter != data.end()) return std::make_pair(HashSetIterator(foundIter), false);

    auto& chainInfo = table[chainIdx];
    if (chainInfo.second == 0)
    {
        data.push_front(key);
        chainInfo.first = data.begin();
    }
    else
    {
        auto it = data.insert(chainInfo.first, key);
        chainInfo.first = it;
    }

    chainInfo.second++;

    double factor = static_cast<double>(size()) / static_cast<double>(table.size());

    if (factor > LOAD_FACTOR)
    {
        rehash(table.size() * 2);
        size_type newChainIdx = hash(key);
        auto newIter = getElementByChain(newChainIdx, key);
        return std::make_pair(HashSetIterator(newIter), true);
    }

    return std::make_pair(HashSetIterator(chainInfo.first), true);
}

template <typename K, typename Hasher, typename KeyEqual>
inline std::pair<typename HashSet<K, Hasher, KeyEqual>::HashSetIterator, bool> HashSet<K, Hasher, KeyEqual>::insert(K&& key)
{
    size_type chainIdx = hash(std::move(key));
    
    auto foundIter = getElementByChain(chainIdx, std::move(key));
    if (foundIter != data.end()) return std::make_pair(HashSetIterator(foundIter), false);

    auto& chainInfo = table[chainIdx];
    if (chainInfo.second == 0)
    {
        data.push_front(std::move(key));
        chainInfo.first = data.begin();
    }
    else
    {
        auto it = data.insert(chainInfo.first, std::move(key));
        chainInfo.first = it;
    }

    chainInfo.second++;

    double factor = static_cast<double>(size()) / static_cast<double>(table.size());

    if (factor > LOAD_FACTOR)
    {
        rehash(table.size() * 2);
        size_type newChainIdx = hash(std::move(key));
        auto newIter = getElementByChain(newChainIdx, key);
        return std::make_pair(HashSetIterator(newIter), true);
    }

    return std::make_pair(HashSetIterator(chainInfo.first), true);
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::HashSetIterator HashSet<K, Hasher, KeyEqual>::erase(const K& key)
{
    if (table.empty()) return end();

    size_type chainIdx = hash(key);

    auto foundIt = getElementByChain(chainIdx, key);
    if (foundIt == data.end()) return end();

    auto& chainInfo = table[chainIdx];
    if (chainInfo.second != 0)
    {
        auto newIter = data.erase(foundIt);
        chainInfo.first = newIter;
    }
    chainInfo.second--;

    return HashSetIterator(chainInfo.first);
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::HashSetIterator HashSet<K, Hasher, KeyEqual>::erase(const HashSetIterator& iter)
{
    if (iter == data.end() || table.empty()) return end();

    const K& key = iter.currElement->first;
    return remove(key);
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::HashSetIterator HashSet<K, Hasher, KeyEqual>::find(const K& key)
{
    if (table.empty()) return end();

    size_type chainIdx = hash(key);
    auto foundIt = getElementByChain(chainIdx, key);
    return HashSetIterator(foundIt);
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::ConstHashSetIterator HashSet<K, Hasher, KeyEqual>::find(const K& key) const
{
    if (table.empty()) return cend();

    size_type chainIdx = hash(key);
    auto foundIt = getElementByChain(chainIdx, key);
    return ConstHashSetIterator(foundIt);
}

template <typename K, typename Hasher, typename KeyEqual>
inline bool HashSet<K, Hasher, KeyEqual>::contains(const K& key) const
{
    auto iter = find(key);
    return iter != end() || iter != cend();
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::size_type  HashSet<K, Hasher, KeyEqual>::count(const K& key) const
{
    size_type chainIdx = hash(key);
    auto foundIt = getElementByChain(chainIdx, key);
    return foundIt != data.end() ? 1 : 0;
}

template <typename K, typename Hasher, typename KeyEqual>
inline void HashSet<K, Hasher, KeyEqual>::rehash(size_type n)
{
    std::vector<entry> oldElements;
    oldElements.reserve(n);

    for (auto it = data.begin(); it != data.end(); it++)
    {
        oldElements.push_back(*it);
    }

    data.clear();
    table.clear();
    table.resize(n, std::make_pair(data.end(), 0));

    for (const auto& entry : oldElements)
    {
        insert(entry);
    }
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::iterator HashSet<K, Hasher, KeyEqual>::getElementByChain(size_type chainIdx, const K& key)
{
    auto iter = table[chainIdx].first;

    size_type chainSize = table[chainIdx].second;
    if (chainSize == 0) return data.end();

    for (std::size_t i = 0; i < chainSize; i++)
    {
        if (key_equal(*iter, key)) return iter;
        iter++;
    }

    return data.end();
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::const_iterator HashSet<K, Hasher, KeyEqual>::getElementByChain(size_type chainIdx, const K& key) const
{
    auto iter = table[chainIdx].first;
    
    size_type chainSize = table[chainIdx].second;
    if (chainSize == 0) return data.cend();

    for (std::size_t i = 0; i < chainSize; i++)
    {
        if (key_equal(iter->first, key)) return iter;
        iter++;
    }

    return data.cend();
}

template <typename K, typename Hasher, typename KeyEqual>
inline typename HashSet<K, Hasher, KeyEqual>::size_type HashSet<K, Hasher, KeyEqual>::hash(const K& key) const
{
    return hasher(key) % table.size();
}
