#pragma once

#include <memory>
#include <utility>
#include <cstddef>

#include <vector>
#include <list>

template <typename K, typename V, typename Hasher = std::hash<K>, typename KeyEqual = std::equal_to<K>>
class HashMap
{
private:
    static constexpr std::size_t MIN_BUCKETS = 16;
    static constexpr double LOAD_FACTOR = 0.8;

private:
    using entry = std::pair<K, V>;
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
    class HashMapIterator
    {
    private:
        friend class HashMap<K, V, Hasher, KeyEqual>;

        iterator currElement;

    public:
        HashMapIterator(iterator currElement) : currElement(currElement) {}

        HashMapIterator& operator++()
        {
            ++currElement;
            return *this;
        }

        HashMapIterator operator++(int)
        {
            HashMapIterator temp = *this;
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

        bool operator==(const HashMapIterator& other) const
        {
            return currElement == other.currElement;
        }

        bool operator!=(const HashMapIterator& other) const
        {
            return !(*this  == other);
        }
    };

    class ConstHashMapIterator
    {
    private:
        friend class HashMap<K, V, Hasher, KeyEqual>;

        const_iterator currElement;

    public:
        ConstHashMapIterator(const_iterator currElement) : currElement(currElement) {}

        ConstHashMapIterator& operator++()
        {
            ++currElement;
            return *this;
        }

        ConstHashMapIterator operator++(int)
        {
            ConstHashMapIterator temp = *this;
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

        bool operator==(const ConstHashMapIterator& other) const
        {
            return currElement == other.currElement;
        }

        bool operator!=(const ConstHashMapIterator& other) const
        {
            return !(*this  == other);
        }
    };

    HashMapIterator begin()
    {
        return HashMapIterator(data.begin());
    }

    HashMapIterator end()
    {
        return HashMapIterator(data.end());
    }

    ConstHashMapIterator cbegin()
    {
        return ConstHashMapIterator(data.cbegin());
    }

    ConstHashMapIterator cend()
    {
        return ConstHashMapIterator(data.cend());
    }

    explicit HashMap(size_type bucket_count = MIN_BUCKETS,
                     const Hasher& hasher = Hasher(),
                     const KeyEqual& equal = KeyEqual());

    size_type size() const noexcept;
    bool empty() const noexcept;

    template <typename U, typename T>
    std::pair<HashMapIterator, bool> insert(U&& key, T&& value);

    template <typename U>
    V& operator[](U&& key);

    HashMapIterator erase(const K& key);
    HashMapIterator erase(const HashMapIterator& iter);

    HashMapIterator find(const K& key);
    ConstHashMapIterator find(const K& key) const;

    bool contains(const K& key) const;

    size_type count(const K& key) const;

private:
    void rehash(size_type n);

    iterator getElementByChain(size_type chainIdx, const K& key);
    const_iterator getElementByChain(size_type chainIdx, const K& key) const;

    size_type hash(const K& key) const;
};

template <typename K, typename V, typename Hasher, typename KeyEqual>
HashMap<K, V, Hasher, KeyEqual>::HashMap(size_type bucket_count,
                                         const Hasher& hasher,
                                         const KeyEqual& equal)
    : data{}, 
    table(bucket_count, chain{ data.end(), 0 }), 
    hasher(hasher), 
    key_equal(equal)
{
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::size_type HashMap<K, V, Hasher, KeyEqual>::size() const noexcept
{
    return data.size();
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline bool HashMap<K, V, Hasher, KeyEqual>::empty() const noexcept
{
    return size() == 0;
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
template <typename U, typename T>
inline std::pair<typename HashMap<K, V, Hasher, KeyEqual>::HashMapIterator, bool> HashMap<K, V, Hasher, KeyEqual>::insert(U&& key, T&& value)
{
    size_type chainIdx = hash(std::forward<U>(key));
    
    auto foundIter = getElementByChain(chainIdx, std::forward<U>(key));
    if (foundIter != data.end()) return std::make_pair(HashMapIterator(foundIter), false);

    auto pairToAdd = std::make_pair(std::forward<U>(key), std::forward<T>(value));

    auto& chainInfo = table[chainIdx];
    if (chainInfo.second == 0)
    {
        data.push_front(pairToAdd);
        chainInfo.first = data.begin();
    }
    else
    {
        auto it = data.insert(chainInfo.first, pairToAdd);
        chainInfo.first = it;
    }

    chainInfo.second++;

    double factor = static_cast<double>(size()) / static_cast<double>(table.size());

    if (factor > LOAD_FACTOR)
    {
        rehash(table.size() * 2);
        size_type newChainIdx = hash(std::forward<U>(key));
        auto newIter = getElementByChain(newChainIdx, std::forward<U>(key));
        return std::make_pair(HashMapIterator(newIter), true);
    }

    return std::make_pair(HashMapIterator(chainInfo.first), true);
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
template <typename U>
inline V& HashMap<K, V, Hasher, KeyEqual>::operator[](U&& key)
{
    size_type chainIdx = hash(std::forward<U>(key));
    
    auto foundIt = getElementByChain(chainIdx, std::forward<U>(key));
    if (foundIt != data.end()) return foundIt->second;

    auto& chainInfo = table[chainIdx];
    if (chainInfo.second == 0)
    {
        data.push_front(std::make_pair(std::forward<U>(key), V{}));
        chainInfo.first = data.begin();
    }
    else
    {
        auto newIter = data.insert(foundIt, std::make_pair(std::forward<U>(key), V{}));
        chainInfo.first = newIter;
    }

    double factor = static_cast<double>(size()) / static_cast<double>(table.size());

    if (factor > LOAD_FACTOR)
    {
        rehash(table.size() * 2);
        size_type newIdx = hash(std::forward<U>(key));
        auto newIter = getElementByChain(newIdx, std::forward<U>(key));
        return newIter->second;
    }

    return chainInfo.first->second;
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::HashMapIterator HashMap<K, V, Hasher, KeyEqual>::erase(const K& key)
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

    return HashMapIterator(chainInfo.first);
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::HashMapIterator HashMap<K, V, Hasher, KeyEqual>::erase(const HashMapIterator& iter)
{
    if (iter == data.end() || table.empty()) return end();

    const K& key = iter.currElement->first;
    return remove(key);
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::HashMapIterator HashMap<K, V, Hasher, KeyEqual>::find(const K& key)
{
    if (table.empty()) return end();

    size_type chainIdx = hash(key);
    auto foundIt = getElementByChain(chainIdx, key);
    return HashMapIterator(foundIt);
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::ConstHashMapIterator HashMap<K, V, Hasher, KeyEqual>::find(const K& key) const
{
    if (table.empty()) return cend();

    size_type chainIdx = hash(key);
    auto foundIt = getElementByChain(chainIdx, key);
    return ConstHashMapIterator(foundIt);
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline bool HashMap<K, V, Hasher, KeyEqual>::contains(const K& key) const
{
    auto iter = find(key);
    return iter != end() || iter != cend();
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::size_type  HashMap<K, V, Hasher, KeyEqual>::count(const K& key) const
{
    size_type chainIdx = hash(key);
    auto foundIt = getElementByChain(chainIdx, key);
    return foundIt != data.end() ? 1 : 0;
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline void HashMap<K, V, Hasher, KeyEqual>::rehash(size_type n)
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

    for (const auto& p : oldElements)
    {
        insert(p.first, p.second);
    }
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::iterator HashMap<K, V, Hasher, KeyEqual>::getElementByChain(size_type chainIdx, const K& key)
{
    auto iter = table[chainIdx].first;

    size_type chainSize = table[chainIdx].second;
    if (chainSize == 0) return data.end();

    for (std::size_t i = 0; i < chainSize; i++)
    {
        if (key_equal(iter->first, key)) return iter;
        iter++;
    }

    return data.end();
}

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::const_iterator HashMap<K, V, Hasher, KeyEqual>::getElementByChain(size_type chainIdx, const K& key) const
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

template <typename K, typename V, typename Hasher, typename KeyEqual>
inline typename HashMap<K, V, Hasher, KeyEqual>::size_type HashMap<K, V, Hasher, KeyEqual>::hash(const K& key) const
{
    return hasher(key) % table.size();
}
