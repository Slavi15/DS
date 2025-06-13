#pragma once

#include <cstddef>
#include <iterator>

#include "Node.hpp"

template <typename T>
class FLLIterator
{
private:
    using Ref = Node<T>&;
    using Ptr = Node<T>*;
    using Offset = std::ptrdiff_t;

    Ptr ptr;

public:
    using iterator_category = std::forward_iterator_tag;

    FLLIterator(Ptr ptr);

    FLLIterator<T>& operator++();
    FLLIterator<T> operator++(int);

    FLLIterator<T>& operator+=(Offset offset);
    FLLIterator<T> operator+(Offset offset);

    T& operator*();
    T* operator->();
    T& operator[](Offset offset);

    bool operator==(const FLLIterator<T>& other) const;
    bool operator!=(const FLLIterator<T>& other) const;
};

template <typename T>
FLLIterator<T>::FLLIterator(Ptr ptr) : ptr(ptr)
{
}

template <typename T>
inline FLLIterator<T>& FLLIterator<T>::operator++()
{
    if (ptr) ptr = ptr->next;
    return *this;
}

template <typename T>
inline FLLIterator<T> FLLIterator<T>::operator++(int)
{
    FLLIterator<T> temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
inline FLLIterator<T>& FLLIterator<T>::operator+=(Offset offset)
{
    while (offset--) ++(*this);
    return *this;
}

template <typename T>
inline FLLIterator<T> FLLIterator<T>::operator+(Offset offset)
{
    FLLIterator<T> temp = *this;
    return temp += offset;
}

template <typename T>
inline T& FLLIterator<T>::operator*()
{
    return ptr->val;
}

template <typename T>
inline T* FLLIterator<T>::operator->()
{
    return &ptr->val;
}

template <typename T>
inline T& FLLIterator<T>::operator[](Offset offset)
{
    return *(ptr + offset);
}

template <typename T>
inline bool FLLIterator<T>::operator==(const FLLIterator<T>& other) const
{
    return ptr == other.ptr;
}

template <typename T>
inline bool FLLIterator<T>::operator!=(const FLLIterator<T>& other) const
{
    return !(ptr == other.ptr);
}

template <typename T>
class ConstFLLIterator
{
private:
    using Ref = const Node<T>&;
    using Ptr = const Node<T>*;
    using Offset = std::ptrdiff_t;

    Ptr ptr;

public:
    using iterator_category = std::forward_iterator_tag;
    
    ConstFLLIterator(Ptr ptr);

    ConstFLLIterator<T>& operator++();
    ConstFLLIterator<T> operator++(int);

    ConstFLLIterator<T>& operator+=(Offset offset);
    ConstFLLIterator<T> operator+(Offset offset);

    const T& operator*() const;
    const T* operator->() const;
    const T& operator[](Offset offset) const;

    bool operator==(const ConstFLLIterator<T>& other) const;
    bool operator!=(const ConstFLLIterator<T>& other) const;
};

template <typename T>
ConstFLLIterator<T>::ConstFLLIterator(Ptr ptr) : ptr(ptr)
{
}

template <typename T>
inline ConstFLLIterator<T>& ConstFLLIterator<T>::operator++()
{
    if (ptr) ptr = ptr->next;
    return *this;
}

template <typename T>
inline ConstFLLIterator<T> ConstFLLIterator<T>::operator++(int)
{
    ConstFLLIterator<T> temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
inline ConstFLLIterator<T>& ConstFLLIterator<T>::operator+=(Offset offset)
{
    while (offset--) ++(*this);
    return *this;
}

template <typename T>
inline ConstFLLIterator<T> ConstFLLIterator<T>::operator+(Offset offset)
{
    ConstFLLIterator<T> temp = *this;
    return temp += offset;
}

template <typename T>
inline const T& ConstFLLIterator<T>::operator*() const
{
    return ptr->val;
}

template <typename T>
inline const T* ConstFLLIterator<T>::operator->() const
{
    return &ptr->val;
}

template <typename T>
inline const T& ConstFLLIterator<T>::operator[](Offset offset) const
{
    return *(ptr + offset);
}

template <typename T>
inline bool ConstFLLIterator<T>::operator==(const ConstFLLIterator<T>& other) const
{
    return ptr == other.ptr;
}

template <typename T>
inline bool ConstFLLIterator<T>::operator!=(const ConstFLLIterator<T>& other) const
{
    return !(ptr == other.ptr);
}