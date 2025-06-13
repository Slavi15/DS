#pragma once

#include <cstddef>
#include <iterator>

template <typename T>
class VectorIterator
{
private:
    using Ptr = T*;
    using Ref = T&;
    using Offset = std::ptrdiff_t;

    Ptr ptr;

public:
    using iterator_category = std::random_access_iterator_tag;

    VectorIterator(Ptr ptr);

    VectorIterator<T>& operator++();
    VectorIterator<T> operator++(int);

    VectorIterator<T>& operator--();
    VectorIterator<T> operator--(int);

    VectorIterator<T>& operator+=(Offset offset);
    VectorIterator<T>& operator-=(Offset offset);

    VectorIterator<T> operator+(Offset offset) const;
    VectorIterator<T> operator-(Offset offset) const;

    Offset operator-(const VectorIterator<T>& other) const;

    Ref operator*() const noexcept;
    Ptr operator->() const noexcept;
    Ref operator[](Offset offset) const noexcept;

    bool operator==(const VectorIterator<T>& other) const;
    bool operator!=(const VectorIterator<T>& other) const;

    bool operator<(const VectorIterator<T>& other) const;
    bool operator>(const VectorIterator<T>& other) const;

    bool operator<=(const VectorIterator<T>& other) const;
    bool operator>=(const VectorIterator<T>& other) const;
};

template <typename T>
VectorIterator<T>::VectorIterator(Ptr ptr) : ptr(ptr)
{
}

template <typename T>
inline VectorIterator<T>& VectorIterator<T>::operator++()
{
    ++ptr;
    return *this;
}

template <typename T>
inline VectorIterator<T> VectorIterator<T>::operator++(int)
{
    VectorIterator<T> temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
inline VectorIterator<T>& VectorIterator<T>::operator--()
{
    --ptr;
    return *this;
}

template <typename T>
inline VectorIterator<T> VectorIterator<T>::operator--(int)
{
    VectorIterator<T> temp = *this;
    --(*this);
    return temp;
}

template <typename T>
inline VectorIterator<T>& VectorIterator<T>::operator+=(Offset offset)
{
    ptr += offset;
    return *this;
}

template <typename T>
inline VectorIterator<T>& VectorIterator<T>::operator-=(Offset offset)
{
    ptr -= offset;
    return *this;
}

template <typename T>
inline VectorIterator<T> VectorIterator<T>::operator+(Offset offset) const
{
    VectorIterator<T> temp = *this;
    temp += offset;
    return temp;
}

template <typename T>
inline VectorIterator<T> VectorIterator<T>::operator-(Offset offset) const
{
    VectorIterator<T> temp = *this;
    temp -= offset;
    return temp;
}

template <typename T>
inline typename VectorIterator<T>::Offset VectorIterator<T>::operator-(const VectorIterator<T>& other) const
{
    return ptr - other.ptr;
}

template <typename T>
inline typename VectorIterator<T>::Ref VectorIterator<T>::operator*() const noexcept
{
    return *ptr;
}

template <typename T>
inline typename VectorIterator<T>::Ptr VectorIterator<T>::operator->() const noexcept
{
    return ptr;
}

template <typename T>
inline typename VectorIterator<T>::Ref VectorIterator<T>::operator[](Offset offset) const noexcept
{
    return *(ptr + offset);
}

template <typename T>
inline bool VectorIterator<T>::operator==(const VectorIterator<T>& other) const
{
    return ptr == other.ptr;
}

template <typename T>
inline bool VectorIterator<T>::operator!=(const VectorIterator<T>& other) const
{
    return ptr != other.ptr;
}

template <typename T>
inline bool VectorIterator<T>::operator<(const VectorIterator<T>& other) const
{
    return ptr < other.ptr;
}

template <typename T>
inline bool VectorIterator<T>::operator>(const VectorIterator<T>& other) const
{
    return ptr > other.ptr;
}

template <typename T>
inline bool VectorIterator<T>::operator<=(const VectorIterator<T>& other) const
{
    return ptr <= other.ptr;
}

template <typename T>
inline bool VectorIterator<T>::operator>=(const VectorIterator<T>& other) const
{
    return ptr >= other.ptr;
}

template <typename T>
class ConstVectorIterator
{
private:
    using Ptr = const T*;
    using Ref = const T&;
    using Offset = std::ptrdiff_t;

    Ptr ptr;

public:
    using iterator_category = std::random_access_iterator_tag;

    ConstVectorIterator(Ptr ptr);
    ConstVectorIterator(const VectorIterator<T>& other);

    ConstVectorIterator<T>& operator++();
    ConstVectorIterator<T> operator++(int);

    ConstVectorIterator<T>& operator--();
    ConstVectorIterator<T> operator--(int);

    ConstVectorIterator<T>& operator+=(Offset offset);
    ConstVectorIterator<T>& operator-=(Offset offset);

    ConstVectorIterator<T> operator+(Offset offset) const;
    ConstVectorIterator<T> operator-(Offset offset) const;

    Offset operator-(const ConstVectorIterator<T>& other) const;

    Ref operator*() const;
    Ptr operator->() const;
    Ref operator[](Offset offset) const;

    bool operator==(const ConstVectorIterator<T>& other) const;
    bool operator!=(const ConstVectorIterator<T>& other) const;

    bool operator<(const ConstVectorIterator<T>& other) const;
    bool operator>(const ConstVectorIterator<T>& other) const;

    bool operator<=(const ConstVectorIterator<T>& other) const;
    bool operator>=(const ConstVectorIterator<T>& other) const;
};

template <typename T>
ConstVectorIterator<T>::ConstVectorIterator(Ptr ptr) : ptr(ptr)
{
}

template <typename T>
ConstVectorIterator<T>::ConstVectorIterator(const VectorIterator<T>& other) : ptr(other.operator->())
{
}

template <typename T>
inline ConstVectorIterator<T>& ConstVectorIterator<T>::operator++()
{
    ++ptr;
    return *this;
}

template <typename T>
inline ConstVectorIterator<T> ConstVectorIterator<T>::operator++(int)
{
    ConstVectorIterator<T> temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
inline ConstVectorIterator<T>& ConstVectorIterator<T>::operator--()
{
    --ptr;
    return *this;
}

template <typename T>
inline ConstVectorIterator<T> ConstVectorIterator<T>::operator--(int)
{
    ConstVectorIterator<T> temp = *this;
    --(*this);
    return temp;
}

template <typename T>
inline ConstVectorIterator<T>& ConstVectorIterator<T>::operator+=(Offset offset)
{
    ptr += offset;
    return *this;
}

template <typename T>
inline ConstVectorIterator<T>& ConstVectorIterator<T>::operator-=(Offset offset)
{
    ptr -= offset;
    return *this;
}

template <typename T>
inline ConstVectorIterator<T> ConstVectorIterator<T>::operator+(Offset offset) const
{
    ConstVectorIterator<T> temp = *this;
    temp += offset;
    return temp;
}

template <typename T>
inline ConstVectorIterator<T> ConstVectorIterator<T>::operator-(Offset offset) const
{
    ConstVectorIterator<T> temp = *this;
    temp -= offset;
    return temp;
}

template <typename T>
inline typename ConstVectorIterator<T>::Offset ConstVectorIterator<T>::operator-(const ConstVectorIterator<T>& other) const
{
    return ptr - other.ptr;
}

template <typename T>
inline typename ConstVectorIterator<T>::Ref ConstVectorIterator<T>::operator*() const
{
    return *ptr;
}

template <typename T>
inline typename ConstVectorIterator<T>::Ptr ConstVectorIterator<T>::operator->() const
{
    return ptr;
}

template <typename T>
inline typename ConstVectorIterator<T>::Ref ConstVectorIterator<T>::operator[](Offset offset) const
{
    return *(ptr + offset);
}

template <typename T>
inline bool ConstVectorIterator<T>::operator==(const ConstVectorIterator<T>& other) const
{
    return ptr == other.ptr;
}

template <typename T>
inline bool ConstVectorIterator<T>::operator!=(const ConstVectorIterator<T>& other) const
{
    return ptr != other.ptr;
}

template <typename T>
inline bool ConstVectorIterator<T>::operator<(const ConstVectorIterator<T>& other) const
{
    return ptr < other.ptr;
}

template <typename T>
inline bool ConstVectorIterator<T>::operator>(const ConstVectorIterator<T>& other) const
{
    return ptr > other.ptr;
}

template <typename T>
inline bool ConstVectorIterator<T>::operator<=(const ConstVectorIterator<T>& other) const
{
    return ptr <= other.ptr;
}

template <typename T>
inline bool ConstVectorIterator<T>::operator>=(const ConstVectorIterator<T>& other) const
{
    return ptr >= other.ptr;
}