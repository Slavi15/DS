#pragma once

#include <iostream>
#include <memory>
#include <utility>

namespace Constants
{
    constexpr std::size_t GROWTH_FACTOR = 2;
}

template <typename T, typename Allocator = std::allocator<T>>
class Stack
{
private:
    T* data;
    std::size_t size;
    std::size_t capacity;

    Allocator allocator;
    using AllocatorT = std::allocator_traits<Allocator>;

public:
    Stack();

    std::size_t getSize() const;
    std::size_t getCapacity() const;

    bool isEmpty() const;

    Stack<T, Allocator>& push(const T& val);
    Stack<T, Allocator>& push(T&& val);

    Stack<T, Allocator>& pop();

    const T& top() const;

    Stack(const Stack<T, Allocator>& other);
    Stack<T, Allocator>& operator=(const Stack<T, Allocator>& other);

    Stack(Stack<T, Allocator>&& other) noexcept;
    Stack<T, Allocator>& operator=(Stack<T, Allocator>&& other) noexcept;

    ~Stack() noexcept;

private:
    void resize(std::size_t n);
    std::size_t calculateCapacity() const;

    void copyFrom(const Stack<T, Allocator>& other);
    void moveFrom(Stack<T, Allocator>&& other) noexcept;
    void free() noexcept;
};

template <typename T, typename Allocator>
Stack<T, Allocator>::Stack() : data(nullptr), size(0), capacity(0)
{
}

template <typename T, typename Allocator>
inline std::size_t Stack<T, Allocator>::getSize() const
{
    return size;
}

template <typename T, typename Allocator>
inline std::size_t Stack<T, Allocator>::getCapacity() const
{
    return capacity;
}

template <typename T, typename Allocator>
bool Stack<T, Allocator>::isEmpty() const
{
    return size == 0;
}

template <typename T, typename Allocator>
inline Stack<T, Allocator>& Stack<T, Allocator>::push(const T& val)
{
    if (size >= capacity) resize(calculateCapacity());
    data[size++] = val;
    return *this;
}

template <typename T, typename Allocator>
inline Stack<T, Allocator>& Stack<T, Allocator>::push(T&& val)
{
    if (size >= capacity) resize(calculateCapacity());
    data[size++] = std::move(val);
    return *this;
}

template <typename T, typename Allocator>
Stack<T, Allocator>& Stack<T, Allocator>::pop()
{
    if (isEmpty()) return *this;
    AllocatorT::destroy(allocator, &data[--size]);
    return *this;
}

template <typename T, typename Allocator>
const T& Stack<T, Allocator>::top() const
{
    return data[size - 1];
}

template <typename T, typename Allocator>
Stack<T, Allocator>::Stack(const Stack<T, Allocator>& other)
{
    copyFrom(other);
}

template <typename T, typename Allocator>
Stack<T, Allocator>& Stack<T, Allocator>::operator=(const Stack<T, Allocator>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

template <typename T, typename Allocator>
Stack<T, Allocator>::Stack(Stack<T, Allocator>&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T, typename Allocator>
Stack<T, Allocator>& Stack<T, Allocator>::operator=(Stack<T, Allocator>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template <typename T, typename Allocator>
Stack<T, Allocator>::~Stack() noexcept
{
    free();
}

template <typename T, typename Allocator>
inline void Stack<T, Allocator>::resize(std::size_t n)
{
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
inline std::size_t Stack<T, Allocator>::calculateCapacity() const
{
    return capacity ? capacity * Constants::GROWTH_FACTOR : 1;
}

template <typename T, typename Allocator>
inline void Stack<T, Allocator>::copyFrom(const Stack<T, Allocator>& other)
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
inline void Stack<T, Allocator>::moveFrom(Stack<T, Allocator>&& other) noexcept
{
    data = std::exchange(other.data, nullptr);
    size = std::exchange(other.size, 0);
    capacity = std::exchange(other.capacity, 0);
}

template <typename T, typename Allocator>
inline void Stack<T, Allocator>::free() noexcept
{
    for (std::size_t i = 0; i < size; i++)
    {
        AllocatorT::destroy(allocator, &data[i]);
    }

    allocator.deallocate(data, capacity);

    data = nullptr;
    size = capacity = 0;
}