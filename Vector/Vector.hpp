#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

#include "Iterator.hpp"

namespace Constants
{
    constexpr std::size_t GROWTH_FACTOR = 2;
}

template <typename T, typename Allocator = std::allocator<T>>
class Vector
{
private:
    T* data;
    std::size_t size;
    std::size_t capacity;

    Allocator allocator;
    using AllocatorT = std::allocator_traits<Allocator>;

public:
    using Iterator = VectorIterator<T>;
    using ConstIterator = ConstVectorIterator<T>;

    Iterator begin()
    {
        return Iterator(data);
    }

    Iterator end()
    {
        return Iterator(data + size);
    }

    ConstIterator cbegin()
    {
        return ConstIterator(data);
    }

    ConstIterator cend()
    {
        return ConstIterator(data + size);
    }

public:
    Vector();
    explicit Vector(std::size_t n);
    Vector(std::size_t n, const T& val);

    std::size_t getSize() const;
    std::size_t getCapacity() const;

    bool isEmpty() const;

    const T& front() const;
    T& front();

    const T& back() const;
    T& back();

    const T& operator[](std::size_t idx) const;
    T& operator[](std::size_t idx);

    Vector<T, Allocator>& push_back(const T& val);
    Vector<T, Allocator>& push_back(T&& val);

    Vector<T, Allocator>& pop_back();

    template <typename... Args>
    Vector<T, Allocator>& emplace_back(Args&&... args);

    Vector<T, Allocator>& insert(const Iterator& pos, const T& val);
    Vector<T, Allocator>& insert(const Iterator& pos, T&& val);

    template <typename U>
    Vector<T, Allocator>& insert(const Iterator& pos, U&& val);

    Vector<T, Allocator>& erase(const Iterator& pos);
    Vector<T, Allocator>& erase(const Iterator& first, const Iterator& last);

    Vector(const Vector<T, Allocator>& other);
    Vector<T, Allocator>& operator=(const Vector<T, Allocator>& other);

    Vector(Vector<T, Allocator>&& other) noexcept;
    Vector<T, Allocator>& operator=(Vector<T, Allocator>&& other) noexcept;

    ~Vector() noexcept;

private:
    void resize(std::size_t n);
    std::size_t calculateCapacity(bool shouldEnlarge) const;

    void copyFrom(const Vector<T, Allocator>& other);
    void moveFrom(Vector<T, Allocator>&& other) noexcept;
    void free() noexcept;
};

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector() : data(nullptr), size(0), capacity(0)
{
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::size_t n) : data(allocator.allocate(n)), size(n), capacity(n)
{
    for (std::size_t i = 0; i < n; i++)
    {
        AllocatorT::construct(allocator, &data[i]);
    }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::size_t n, const T& val) : data(allocator.allocate(n)), size(n), capacity(n)
{
    for (std::size_t i = 0; i < n; i++)
    {
        AllocatorT::construct(allocator, &data[i], val);
    }
}

template <typename T, typename Allocator>
inline std::size_t Vector<T, Allocator>::getSize() const
{
    return size;
}

template <typename T, typename Allocator>
inline std::size_t Vector<T, Allocator>::getCapacity() const
{
    return size;
}

template <typename T, typename Allocator>
inline bool Vector<T, Allocator>::isEmpty() const
{
    return size == 0;
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::front() const
{
    return data[0];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::front() 
{
    return data[0];
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::back() const
{
    return data[size - 1];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::back()
{
    return data[size - 1];
}

template <typename T, typename Allocator>
const T& Vector<T, Allocator>::operator[](std::size_t idx) const
{
    return data[idx];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](std::size_t idx)
{
    return data[idx];
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::push_back(const T& val)
{
    if (size >= capacity) reserve(calculateCapacity(true));
    AllocatorT::construct(allocator, &data[size++], val);
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::push_back(T&& val)
{
    if (size >= capacity) reserve(calculateCapacity(true));
    AllocatorT::construct(allocator, &data[size++], std::move(val));
    return *this;
}

template <typename T, typename Allocator>
inline Vector<T, Allocator>& Vector<T, Allocator>::pop_back()
{
    if (isEmpty()) return *this;

    AllocatorT::destroy(allocator, &data[--size]);

    if (size * 4 >= capacity && capacity > 1) resize(calculateCapacity(false));

    return *this;
}

template <typename T, typename Allocator>
template <typename... Args>
inline Vector<T, Allocator>& Vector<T, Allocator>::emplace_back(Args&&... args)
{
    if (size >= capacity) resize(calculateCapacity(true));
    AllocatorT::construct(allocator, &data[size++], std::forward<Args>(args)...);
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::insert(const Iterator& pos, const T& val)
{
    if (size >= capacity) resize(calculateCapacity(true));

    std::ptrdiff_t idx = pos - begin();

    if (pos != end())
    {
        std::move_backward(data + idx, data + size - 1, data + size);
    }

    AllocatorT::destroy(allocator, &data[idx]);
    AllocatorT::construct(allocator, &data[idx], val);

    size++;
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::insert(const Iterator& pos, T&& val)
{
    if (size >= capacity) resize(calculateCapacity(true));

    std::ptrdiff_t idx = pos - begin();

    if (pos != end())
    {
        std::move_backward(data + idx, data + size - 1, data + size);
    }

    AllocatorT::destroy(allocator, &data[idx]);
    AllocatorT::construct(allocator, &data[idx], std::move(val));

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline Vector<T, Allocator>& Vector<T, Allocator>::erase(const Iterator& pos)
{
    return erase(pos, pos + 1);
}

template <typename T, typename Allocator>
inline Vector<T, Allocator>& Vector<T, Allocator>::erase(const Iterator& first, const Iterator& last)
{
    std::ptrdiff_t range = last - first;

    std::ptrdiff_t startIdx = first - begin();
    std::ptrdiff_t endIdx = last - begin();

    if (range <= 0) return *this;

    if (last != end())
    {
        std::move(data + endIdx, data + size, data + startIdx);
    }

    for (std::ptrdiff_t i = size - range; i < size; i++)
    {
        AllocatorT::destroy(allocator, &data[i]);
    }

    size -= range;
    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector<T, Allocator>& other)
{
    copyFrom(other);
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector<T, Allocator>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector<T, Allocator>&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector<T, Allocator>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector() noexcept
{
    free();
}

template <typename T, typename Allocator>
inline void Vector<T, Allocator>::resize(std::size_t n)
{
    if (n == 0) n = 1;
    T* newData = allocator.allocate(n);

    for (std::size_t i = 0; i < size; i++)
    {
        AllocatorT::construct(allocator, &newData[i], std::move(data[i]));
        AllocatorT::destroy(allocator, &data[i]);
    }

    allocator.deallocate(data, capacity);
    data = newData;
    capacity = n;
}

template <typename T, typename Allocator>
inline std::size_t Vector<T, Allocator>::calculateCapacity(bool shouldEnlarge) const
{
    return shouldEnlarge
        ? (capacity ? capacity * Constants::GROWTH_FACTOR : 1)
        : (capacity ? capacity / Constants::GROWTH_FACTOR : 1);
}

template <typename T, typename Allocator>
inline void Vector<T, Allocator>::copyFrom(const Vector<T, Allocator>& other)
{
    data = allocator.allocate(other.capacity);

    for (std::size_t i = 0; i < other.size; i++)
    {
        AllocatorT::construct(allocator, &data[i], other.data[i]);
    }

    size = other.size;
    capacity = other.capacity;
}

template <typename T, typename Allocator>
inline void Vector<T, Allocator>::moveFrom(Vector<T, Allocator>&& other) noexcept
{
    data = std::exchange(other.data, nullptr);
    size = std::exchange(other.size, 0);
    capacity = std::exchange(other.capacity, 0);
}

template <typename T, typename Allocator>
inline void Vector<T, Allocator>::free() noexcept
{
    for (std::size_t i = 0; i < size; i++)
    {
        AllocatorT::destroy(allocator, &data[i]);
    }

    allocator.deallocate(data, capacity);

    data = nullptr;
    size = capacity = 0;
}