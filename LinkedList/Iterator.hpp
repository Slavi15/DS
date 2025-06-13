#pragma once

#include <iterator>

#include "Node.hpp"

template <typename T, typename Allocator>
class LinkedList;

template <typename T>
class LLIterator
{
private:
    friend class LinkedList<T, std::allocator<Node<T>>>;

    using Ptr = Node<T>*;
    using Ref = Node<T>&;
    using Offset = std::ptrdiff_t;

    LinkedList<T, std::allocator<Node<T>>>& list;
    Ptr ptr;

public:
    using iterator_category = std::bidirectional_iterator_tag;

    LLIterator(LinkedList<T, std::allocator<Node<T>>>& list, Ptr ptr);

    LLIterator<T>& operator++();
    LLIterator<T> operator++(int);

    LLIterator<T>& operator--();
    LLIterator<T> operator--(int);

    LLIterator<T>& operator+=(Offset offset);
    LLIterator<T>& operator-=(Offset offset);

    LLIterator<T> operator+(Offset offset);
    LLIterator<T> operator-(Offset offset);

    T& operator*();
    T* operator->();
    T& operator[](Offset offset);

    bool operator==(const LLIterator<T>& other) const;
    bool operator!=(const LLIterator<T>& other) const;
};

template <typename T>
LLIterator<T>::LLIterator(LinkedList<T, std::allocator<Node<T>>>& list, Ptr ptr) : list(list), ptr(ptr)
{
}

template <typename T>
inline LLIterator<T>& LLIterator<T>::operator++()
{
    if (ptr) ptr = ptr->next;
    return *this;
}

template <typename T>
inline LLIterator<T> LLIterator<T>::operator++(int)
{
    LLIterator<T> temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
inline LLIterator<T>& LLIterator<T>::operator--()
{
    if (ptr)
    {
        ptr = ptr->prev;
    }
    else
    {
        ptr = list.tail;
    }

    return *this;
}

template <typename T>
inline LLIterator<T> LLIterator<T>::operator--(int)
{
    LLIterator<T> temp = *this;
    --(*this);
    return temp;
}

template <typename T>
inline LLIterator<T>& LLIterator<T>::operator+=(Offset offset)
{
    while (offset--) ++(*this);
    return *this;
}

template <typename T>
inline LLIterator<T>& LLIterator<T>::operator-=(Offset offset)
{
    while (offset--) --(*this);
    return *this;
}

template <typename T>
inline LLIterator<T> LLIterator<T>::operator+(Offset offset)
{
    LLIterator<T> temp = *this;
    temp += offset;
    return temp;
}

template <typename T>
inline LLIterator<T> LLIterator<T>::operator-(Offset offset)
{
    LLIterator<T> temp = *this;
    temp -= offset;
    return temp;
}

template <typename T>
inline T& LLIterator<T>::operator*()
{
    return ptr->val;
}

template <typename T>
inline T* LLIterator<T>::operator->()
{
    return &ptr->val;
}

template <typename T>
inline T& LLIterator<T>::operator[](Offset offset)
{
    return *(ptr + offset);
}

template <typename T>
inline bool LLIterator<T>::operator==(const LLIterator<T>& other) const
{
    return ptr == other.ptr;
}

template <typename T>
inline bool LLIterator<T>::operator!=(const LLIterator<T>& other) const
{
    return !(ptr == other.ptr);
}

template <typename T>
class ConstLLIterator
{
private:
    friend class LinkedList<T, std::allocator<Node<T>>>;

    using Ptr = const Node<T>*;
    using Ref = const Node<T>&;
    using Offset = std::ptrdiff_t;

    const LinkedList<T, std::allocator<Node<T>>>& list;
    Ptr ptr;

public:
    using iterator_category = std::bidirectional_iterator_tag;

    ConstLLIterator(const LinkedList<T, std::allocator<Node<T>>>& list, Ptr ptr);

    ConstLLIterator<T>& operator++();
    ConstLLIterator<T> operator++(int);

    ConstLLIterator<T>& operator--();
    ConstLLIterator<T> operator--(int);

    ConstLLIterator<T>& operator+=(Offset offset);
    ConstLLIterator<T>& operator-=(Offset offset);

    ConstLLIterator<T> operator+(Offset offset);
    ConstLLIterator<T> operator-(Offset offset);

    const T& operator*() const;
    const T* operator->() const;
    const T& operator[](Offset offset) const;

    bool operator==(const ConstLLIterator<T>& other) const;
    bool operator!=(const ConstLLIterator<T>& other) const;
};

template <typename T>
ConstLLIterator<T>::ConstLLIterator(const LinkedList<T, std::allocator<Node<T>>>& list, Ptr ptr) : list(list), ptr(ptr)
{
}

template <typename T>
inline ConstLLIterator<T>& ConstLLIterator<T>::operator++()
{
    if (ptr) ptr = ptr->next;
    return *this;
}

template <typename T>
inline ConstLLIterator<T> ConstLLIterator<T>::operator++(int)
{
    ConstLLIterator<T> temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
inline ConstLLIterator<T>& ConstLLIterator<T>::operator--()
{
    if (ptr)
    {
        ptr = ptr->prev;
    }
    else
    {
        ptr = list.tail;
    }

    return *this;
}

template <typename T>
inline ConstLLIterator<T> ConstLLIterator<T>::operator--(int)
{
    ConstLLIterator<T> temp = *this;
    --(*this);
    return temp;
}

template <typename T>
inline ConstLLIterator<T>& ConstLLIterator<T>::operator+=(Offset offset)
{
    while (offset--) ++(*this);
    return *this;
}

template <typename T>
inline ConstLLIterator<T>& ConstLLIterator<T>::operator-=(Offset offset)
{
    while (offset--) --(*this);
    return *this;
}

template <typename T>
inline ConstLLIterator<T> ConstLLIterator<T>::operator+(Offset offset)
{
    ConstLLIterator<T> temp = *this;
    temp += offset;
    return temp;
}

template <typename T>
inline ConstLLIterator<T> ConstLLIterator<T>::operator-(Offset offset)
{
    ConstLLIterator<T> temp = *this;
    temp -= offset;
    return temp;
}

template <typename T>
inline const T& ConstLLIterator<T>::operator*() const
{
    return ptr->val;
}

template <typename T>
inline const T* ConstLLIterator<T>::operator->() const
{
    return &ptr->val;
}

template <typename T>
inline const T& ConstLLIterator<T>::operator[](Offset offset) const
{
    return *(ptr + offset);
}

template <typename T>
inline bool ConstLLIterator<T>::operator==(const ConstLLIterator<T>& other) const
{
    return ptr == other.ptr;
}

template <typename T>
inline bool ConstLLIterator<T>::operator!=(const ConstLLIterator<T>& other) const
{
    return !(ptr == other.ptr);
}