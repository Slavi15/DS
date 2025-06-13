#pragma once

#include <iostream>
#include <memory>
#include <utility>

template <typename T>
class DequeIterator;

template <typename T>
class ConstDequeIterator;

namespace Constants
{
    constexpr std::size_t GROWTH_FACTOR = 2;
}

template <typename T, typename Allocator = std::allocator<T>>
class Deque
{
private:
    T* data;
    std::size_t size;
    std::size_t capacity;

    std::size_t head;
    std::size_t tail;

    Allocator allocator;
    using AllocatorT = std::allocator_traits<Allocator>;

public:
    using Iterator = DequeIterator<T>;
    using ConstIterator = ConstDequeIterator<T>;
    
    Iterator begin()
    {
        return Iterator(0, *this);
    }

    Iterator end()
    {
        return Iterator(size, *this);
    }

    ConstIterator cbegin()
    {
        return ConstIterator(0, *this);
    }

    ConstIterator cend()
    {
        return ConstIterator(size, *this);
    }

public:
    Deque();
    explicit Deque(std::size_t n);
    Deque(std::size_t n, const T& val);

    std::size_t getSize() const;
    std::size_t getCapacity() const;

    bool isEmpty() const;

    const T& front() const;
    T& front();

    const T& back() const;
    T& back();

    const T& operator[](std::size_t idx) const;
    T& operator[](std::size_t idx);

    Deque<T, Allocator>& push_back(const T& val);
    Deque<T, Allocator>& push_back(T&& val);

    Deque<T, Allocator>& pop_back();

    Deque<T, Allocator>& push_front(const T& val);
    Deque<T, Allocator>& push_front(T&& val);

    Deque<T, Allocator>& pop_front();

    template <typename... Args>
    Deque<T, Allocator>& emplace_back(Args&&... args);

    template <typename... Args>
    Deque<T, Allocator>& emplace_front(Args&&... args);

    Deque(const Deque<T, Allocator>& other);
    Deque<T, Allocator>& operator=(const Deque<T, Allocator>& other);

    Deque(Deque<T, Allocator>&& other) noexcept;
    Deque<T, Allocator>& operator=(Deque<T, Allocator>&& other) noexcept;

    ~Deque() noexcept;

private:
    void moveIndex(std::size_t& index, bool forward) const;
    std::size_t calculateCapacity() const;

    void resize(std::size_t n);

    void copyFrom(const Deque<T, Allocator>& other);
    void moveFrom(Deque<T, Allocator>&& other) noexcept;
    void free() noexcept;
};

template <typename T>
class DequeIterator
{
private:
    friend class Deque<T>;

    std::size_t index;
    Deque<T>& deque;

public:
    DequeIterator(std::size_t index, Deque<T>& deque) : index(index), deque(deque) {}

    DequeIterator<T>& operator++()
    {
        ++index;
        return *this;
    }

    DequeIterator<T> operator++(int)
    {
        DequeIterator<T> temp = *this;
        ++(*this);
        return temp;
    }

    DequeIterator<T>& operator--()
    {
        --index;
        return *this;
    }

    DequeIterator<T> operator--(int)
    {
        DequeIterator<T> temp = *this;
        --(*this);
        return temp;
    }

    T& operator*()
    {
        return deque[index];
    }

    T* operator->()
    {
        return &operator*();
    }

    bool operator==(const DequeIterator& other) const
    {
        return index == other.index && &deque == &other.deque;
    }

    bool operator!=(const DequeIterator& other) const
    {
        return !(*this == other);
    }
};

template <typename T>
class ConstDequeIterator
{
private:
    friend class Deque<T>;

    std::size_t index;
    const Deque<T>& deque;

public:
    ConstDequeIterator(std::size_t index, const Deque<T>& deque) : index(index), deque(deque) {}

    ConstDequeIterator<T>& operator++()
    {
        ++index;
        return *this;
    }

    ConstDequeIterator<T> operator++(int)
    {
        ConstDequeIterator<T> temp = *this;
        ++(*this);
        return temp;
    }

    ConstDequeIterator<T>& operator--()
    {
        --index;
        return *this;
    }

    ConstDequeIterator<T> operator--(int)
    {
        ConstDequeIterator<T> temp = *this;
        --(*this);
        return temp;
    }

    const T& operator*() const
    {
        return deque[index];
    }

    const T* operator->() const
    {
        return &operator*();
    }

    bool operator==(const ConstDequeIterator& other) const
    {
        return index == other.index && &deque == &other.deque;
    }

    bool operator!=(const ConstDequeIterator& other) const
    {
        return !(*this == other);
    }
};

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque()
    :   data(nullptr), 
        size(0), 
        capacity(0), 
        head(0), 
        tail(0)
{
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(std::size_t n)
    :   data(allocator.allocate(n)), 
        size(n), 
        capacity(n),
        head(0),
        tail(n == 0 ? 0 : n - 1)
{
    for (std::size_t i = 0; i < size; i++)
    {
        AllocatorT::construct(allocator, &data[i]);
    }
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(std::size_t n, const T& val)
    :   data(allocator.allocate(n)), 
        size(n), 
        capacity(n),
        head(0),
        tail(n == 0 ? 0 : n - 1)
{
    for (std::size_t i = 0; i < size; i++)
    {
        AllocatorT::construct(allocator, &data[i], val);
    }
}

template <typename T, typename Allocator>
inline std::size_t Deque<T, Allocator>::getSize() const
{
    return size;
}

template <typename T, typename Allocator>
inline std::size_t Deque<T, Allocator>::getCapacity() const
{
    return capacity;
}

template <typename T, typename Allocator>
inline bool Deque<T, Allocator>::isEmpty() const
{
    return size == 0;
}

template <typename T, typename Allocator>
inline const T& Deque<T, Allocator>::operator[](std::size_t idx) const
{
    return data[(head + idx) % capacity];
}

template <typename T, typename Allocator>
inline T& Deque<T, Allocator>::operator[](std::size_t idx)
{
    return data[(head + idx) % capacity];
}

template <typename T, typename Allocator>
const T& Deque<T, Allocator>::front() const
{
    return data[head];
}

template <typename T, typename Allocator>
T& Deque<T, Allocator>::front()
{
    return data[head];
}

template <typename T, typename Allocator>
const T& Deque<T, Allocator>::back() const
{
    return data[(tail == 0 ? capacity : tail) - 1];
}

template <typename T, typename Allocator>
T& Deque<T, Allocator>::back()
{
    return data[(tail == 0 ? capacity : tail) - 1];
}

template <typename T, typename Allocator>
inline Deque<T, Allocator>& Deque<T, Allocator>::push_back(const T& val)
{
    if (size >= capacity) resize(calculateCapacity());

    AllocatorT::construct(allocator, &data[tail], val);
    moveIndex(tail, true);

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline Deque<T, Allocator>& Deque<T, Allocator>::push_back(T&& val)
{
    if (size >= capacity) resize(calculateCapacity());

    AllocatorT::construct(allocator, &data[tail], std::move(val));
    moveIndex(tail, true);

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline Deque<T, Allocator>& Deque<T, Allocator>::pop_back()
{
    if (isEmpty()) return *this;

    moveIndex(tail, false);
    AllocatorT::destroy(allocator, &data[tail]);

    size--;
    return *this;
}

template <typename T, typename Allocator>
inline Deque<T, Allocator>& Deque<T, Allocator>::push_front(const T& val)
{
    if (size >= capacity) resize(calculateCapacity());

    moveIndex(head, false);
    AllocatorT::construct(allocator, &data[head], val);

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline Deque<T, Allocator>& Deque<T, Allocator>::push_front(T&& val)
{
    if (size >= capacity) resize(calculateCapacity());

    moveIndex(head, false);
    AllocatorT::construct(allocator, &data[head], std::move(val));

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline Deque<T, Allocator>& Deque<T, Allocator>::pop_front()
{
    if (isEmpty()) return *this;

    AllocatorT::destroy(allocator, &data[head]);
    moveIndex(head, true);

    size--;
    return *this;
}

template <typename T, typename Allocator>
template <typename... Args>
inline Deque<T, Allocator>& Deque<T, Allocator>::emplace_back(Args&&... args)
{
    if (size >= capacity) resize(calculateCapacity());

    AllocatorT::construct(allocator, &data[tail], std::forward<Args>(args)...);
    moveIndex(tail, true);

    size++;
    return *this;
}

template <typename T, typename Allocator>
template <typename... Args>
inline Deque<T, Allocator>& Deque<T, Allocator>::emplace_front(Args&&... args)
{
    if (size >= capacity) resize(calculateCapacity());

    moveIndex(head, false);
    AllocatorT::construct(allocator, &data[head], std::forward<Args>(args)...);

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline void Deque<T, Allocator>::resize(std::size_t n)
{
    if (n == 0) n = calculateCapacity();

    T* newData = allocator.allocate(n);

    std::size_t elements = std::min(n, size);
    for (std::size_t i = 0; i < elements; i++)
    {
        std::size_t idx = (head + i) % capacity;
        AllocatorT::construct(allocator, &newData[i], std::move(data[idx]));
        AllocatorT::destroy(allocator, &data[idx]);
    }

    allocator.deallocate(data, capacity);

    data = newData;
    head = 0;
    tail = size;
    capacity = n;
}

template <typename T, typename Allocator>
void Deque<T, Allocator>::moveIndex(std::size_t& index, bool forward) const
{
    if (capacity == 0) return;

    if (forward)
	{
		(++index) %= capacity;
	}
	else
	{
		index = (index == 0) ? capacity - 1 : index - 1;
	}
}

template <typename T, typename Allocator>
inline std::size_t Deque<T, Allocator>::calculateCapacity() const
{
    return capacity ? capacity * Constants::GROWTH_FACTOR : 1;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(const Deque<T, Allocator>& other)
{
    copyFrom(other);
}

template <typename T, typename Allocator>
Deque<T, Allocator>& Deque<T, Allocator>::operator=(const Deque<T, Allocator>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::Deque(Deque<T, Allocator>&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T, typename Allocator>
Deque<T, Allocator>& Deque<T, Allocator>::operator=(Deque<T, Allocator>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template <typename T, typename Allocator>
Deque<T, Allocator>::~Deque() noexcept
{
    free();
}

template <typename T, typename Allocator>
inline void Deque<T, Allocator>::copyFrom(const Deque<T, Allocator>& other)
{
    allocator.allocate(other.capacity);

    for (std::size_t i = 0; i < other.size; i++)
    {
        std::size_t idx = (head + i) % capacity;
        AllocatorT::construct(allocator, &data[idx], other[i]);
    }

    size = other.size;
    capacity = other.capacity;
    head = 0;
    tail = size > 0 ? size - 1 : 0;
}

template <typename T, typename Allocator>
inline void Deque<T, Allocator>::moveFrom(Deque<T, Allocator>&& other) noexcept
{
    data = std::exchange(other.data, nullptr);
    size = std::exchange(other.size, 0);
    capacity = std::exchange(other.capacity, 0);
    head = std::exchange(other.head, 0);
    tail = std::exchange(other.tail, 0);
}

template <typename T, typename Allocator>
inline void Deque<T, Allocator>::free() noexcept
{
    for (std::size_t i = 0; i < size; i++)
    {
        std::size_t idx = (head + i) % capacity;
        AllocatorT::destroy(allocator, &data[idx]);
    }

    allocator.deallocate(data, capacity);
    data = nullptr;
    size = 0;
    capacity = 0;
    head = 0;
    tail = 0;
}
