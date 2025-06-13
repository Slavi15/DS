#pragma once

#include <iostream>

#include <utility>
#include <functional>
#include <algorithm>

#include <vector>

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class PriorityQueue
{
private:
    Container container;
    Compare compare;

public:
    std::size_t size() const;
    bool empty() const;

    PriorityQueue<T>& push(const T& val);
    PriorityQueue<T>& push(T&& val);

    PriorityQueue<T>& pop();

    const T& top() const;

    template <typename... Args>
    PriorityQueue<T>& emplace(Args&&... args);

private:
    void sift_up(int index);
    void sift_down(int index);
};

template <typename T, typename Container, typename Compare>
inline std::size_t PriorityQueue<T, Container, Compare>::size() const
{
    return container.size();
}

template <typename T, typename Container, typename Compare>
inline bool PriorityQueue<T, Container, Compare>::empty() const
{
    return size() == 0;
}

template <typename T, typename Container, typename Compare>
inline PriorityQueue<T>& PriorityQueue<T, Container, Compare>::push(const T& val)
{
    container.push_back(val);
    sift_up(static_cast<int>(container.size() - 1));
    return *this;
}

template <typename T, typename Container, typename Compare>
inline PriorityQueue<T>& PriorityQueue<T, Container, Compare>::push(T&& val)
{
    container.push_back(std::move(val));
    sift_up(static_cast<int>(container.size() - 1));
    return *this;
}

template <typename T, typename Container, typename Compare>
PriorityQueue<T>& PriorityQueue<T, Container, Compare>::pop()
{
    if (empty()) return *this;

    std::swap(container.front(), container.back());
    container.pop_back();

    if (!empty()) sift_down(0);

    return *this;
}

template <typename T, typename Container, typename Compare>
inline const T& PriorityQueue<T, Container, Compare>::top() const
{
    return container.front();
}

template <typename T, typename Container, typename Compare>
template <typename... Args>
inline PriorityQueue<T>& PriorityQueue<T, Container, Compare>::emplace(Args&&... args)
{
    container.push_back(std::forward<Args>(args)...);
    sift_up(static_cast<int>(container.size() - 1));
    return *this;
}

template <typename T, typename Container, typename Compare>
inline void PriorityQueue<T, Container, Compare>::sift_up(int index)
{
    if (index != 0)
    {
        int parentIndex = (index - 1) / 2;

        if (compare(container[parentIndex], container[index]))
        {
            std::swap(container[parentIndex], container[index]);
            sift_up(parentIndex);
        }
    }
}

template <typename T, typename Container, typename Compare>
inline void PriorityQueue<T, Container, Compare>::sift_down(int idx)
{
    int index = idx;

    int left = (2 * idx) + 1;
    int right = (2 * idx) + 2;

    int n = static_cast<int>(container.size());

    if (left < n && compare(container[index], container[left]))
    {
        index = left;
    }

    if (right < n && compare(container[index], container[right]))
    {
        index = right;
    }

    if (index != idx)
    {
        std::swap(container[index], container[idx]);
        sift_down(index);
    }
}
