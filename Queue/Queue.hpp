#pragma once

#include <iostream>
#include <memory>
#include <utility>

namespace Constants
{
    constexpr std::size_t GROWTH_FACTOR = 2;
}

template <typename T, typename Allocator = std::allocator<T>>
class Queue
{
private:
    T* data;
    std::size_t size;
    std::size_t capacity;

    std::size_t get;
    std::size_t put;

    Allocator allocator;
    using AllocatorT = std::allocator_traits<Allocator>;

public:
    Queue();

    std::size_t getSize() const;
    std::size_t getCapacity() const;

    bool isEmpty() const;

    Queue<T, Allocator>& push(const T& val);
    Queue<T, Allocator>& push(T&& val);

    Queue<T, Allocator>& pop();

    const T& front() const;

    Queue(const Queue<T, Allocator>& other);
    Queue<T, Allocator>& operator=(const Queue<T, Allocator>& other);

    Queue(Queue<T, Allocator>&& other) noexcept;
    Queue<T, Allocator>& operator=(Queue<T, Allocator>&& other) noexcept;

    ~Queue() noexcept;

private:
    void resize(std::size_t n);
    std::size_t calculateCapacity() const;

    void copyFrom(const Queue<T, Allocator>& other);
    void moveFrom(Queue<T, Allocator>&& other) noexcept;
    void free() noexcept;
};

template <typename T, typename Allocator>
Queue<T, Allocator>::Queue() : data(nullptr), size(0), capacity(0), get(0), put(0)
{
}

template <typename T, typename Allocator>
inline std::size_t Queue<T, Allocator>::getSize() const
{
    return size;
}

template <typename T, typename Allocator>
inline std::size_t Queue<T, Allocator>::getCapacity() const
{
    return capacity;
}

template <typename T, typename Allocator>
bool Queue<T, Allocator>::isEmpty() const
{
    return size == 0;
}

template <typename T, typename Allocator>
inline Queue<T, Allocator>& Queue<T, Allocator>::push(const T& val)
{
    if (size >= capacity) resize(calculateCapacity());

    data[put] = val;
    (++put) %= capacity;

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline Queue<T, Allocator>& Queue<T, Allocator>::push(T&& val)
{
    if (size >= capacity) resize(calculateCapacity());

    data[put] = std::move(val);
    (++put) %= capacity;

    size++;
    return *this;
}

template <typename T, typename Allocator>
Queue<T, Allocator>& Queue<T, Allocator>::pop()
{
    if (isEmpty()) return *this;

    AllocatorT::destroy(allocator, &data[get]);
    (++get) %= capacity;

    size--;
    return *this;
}

template <typename T, typename Allocator>
const T& Queue<T, Allocator>::front() const
{
    return data[get];
}

template <typename T, typename Allocator>
Queue<T, Allocator>::Queue(const Queue<T, Allocator>& other)
{
    copyFrom(other);
}

template <typename T, typename Allocator>
Queue<T, Allocator>& Queue<T, Allocator>::operator=(const Queue<T, Allocator>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

template <typename T, typename Allocator>
Queue<T, Allocator>::Queue(Queue<T, Allocator>&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T, typename Allocator>
Queue<T, Allocator>& Queue<T, Allocator>::operator=(Queue<T, Allocator>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template <typename T, typename Allocator>
Queue<T, Allocator>::~Queue() noexcept
{
    free();
}

template <typename T, typename Allocator>
inline void Queue<T, Allocator>::resize(std::size_t n)
{
    T* newData = allocator.allocate(n);

    for (std::size_t i = 0; i < size; i++)
    {
        AllocatorT::construct(allocator, &newData[i], std::move(data[get]));
        (++get) %= capacity;
    }

    allocator.deallocate(data, capacity);
    
    data = newData;
    capacity = n;
    get = 0;
    put = size;
}

template <typename T, typename Allocator>
inline std::size_t Queue<T, Allocator>::calculateCapacity() const
{
    return capacity ? capacity * Constants::GROWTH_FACTOR : 1;
}

template <typename T, typename Allocator>
inline void Queue<T, Allocator>::copyFrom(const Queue<T, Allocator>& other)
{
    data = allocator.allocate(other.capacity);

    for (std::size_t i = other.get; i != other.put; (++i) %= other.capacity)
    {
        AllocatorT::construct(allocator, &data[i], other.data[i]);
    }

    size = other.size;
    capacity = other.capacity;

    get = other.get;
    put = other.put;
}

template <typename T, typename Allocator>
inline void Queue<T, Allocator>::moveFrom(Queue<T, Allocator>&& other) noexcept
{
    data = std::exchange(other.data, nullptr);
    size = std::exchange(other.size, 0);
    capacity = std::exchange(other.capacity, 0);
    get = std::exchange(other.get, 0);
    put = std::exchange(other.put, 0);
}

template <typename T, typename Allocator>
inline void Queue<T, Allocator>::free() noexcept
{
    for (std::size_t i = get; i != put; (++i) %= capacity)
    {
        AllocatorT::destroy(allocator, &data[i]);
    }

    allocator.deallocate(data, capacity);

    data = nullptr;
    size = capacity = get = put = 0;
}