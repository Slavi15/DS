#pragma once

#include <iostream>
#include <memory>
#include <utility>

#include "Iterator.hpp"

template <typename T, typename Allocator = std::allocator<Node<T>>>
class ForwardLinkedList
{
private:
    Node<T>* head;
    Node<T>* tail;
    std::size_t size;

    Allocator allocator;
    using AllocatorT = std::allocator_traits<Allocator>;

public:
    using Iterator = FLLIterator<T>;
    using ConstIterator = ConstFLLIterator<T>;

    Iterator begin()
    {
        return Iterator(head);
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }

    ConstIterator cbegin()
    {
        return ConstIterator(head);
    }

    ConstIterator cend()
    {
        return ConstIterator(nullptr);
    }

public:
    ForwardLinkedList();

    std::size_t getSize() const;

    bool isEmpty() const;

    template <typename U>
    ForwardLinkedList<T, Allocator>& push_back(U&& val);
    ForwardLinkedList<T, Allocator>& pop_back();

    template <typename U>
    ForwardLinkedList<T, Allocator>& push_front(U&& val);
    ForwardLinkedList<T, Allocator>& pop_front();

    template <typename U>
    Iterator insert(const Iterator& it, U&& val);
    Iterator remove(const Iterator& it);

    void print() const;

    ForwardLinkedList(const ForwardLinkedList<T, Allocator>& other);
    ForwardLinkedList<T, Allocator>& operator=(const ForwardLinkedList<T, Allocator>& other);

    ForwardLinkedList(ForwardLinkedList<T, Allocator>&& other) noexcept;
    ForwardLinkedList<T, Allocator>& operator=(ForwardLinkedList<T, Allocator>&& other) noexcept;

    ~ForwardLinkedList() noexcept;

private:
    template <typename U>
    Node<T>* createNode(U&& val);

    void deleteNode(Node<T>* node);

    void copyFrom(const ForwardLinkedList<T, Allocator>& other);
    void moveFrom(ForwardLinkedList<T, Allocator>&& other) noexcept;
    void free() noexcept;
};

template <typename T, typename Allocator>
ForwardLinkedList<T, Allocator>::ForwardLinkedList() : head(nullptr), tail(nullptr), size(0)
{
}

template <typename T, typename Allocator>
inline std::size_t ForwardLinkedList<T, Allocator>::getSize() const
{
    return size;
}

template <typename T, typename Allocator>
inline bool ForwardLinkedList<T, Allocator>::isEmpty() const
{
    return size == 0;
}

template <typename T, typename Allocator>
template <typename U>
inline ForwardLinkedList<T, Allocator>& ForwardLinkedList<T, Allocator>::push_back(U&& val)
{
    Node<T>* newNode = createNode(std::forward<U>(val));

    if (isEmpty())
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline ForwardLinkedList<T, Allocator>& ForwardLinkedList<T, Allocator>::pop_back()
{
    if (isEmpty()) return *this;

    if (head == tail)
    {
        deleteNode(tail);
        head = tail = nullptr;
    }
    else
    {
        Node<T>* curr = head;
        while (curr->next != tail) curr = curr->next;

        deleteNode(tail);

        tail = curr;
        tail->next = nullptr;
    }

    size--;
    return *this;    
}

template <typename T, typename Allocator>
template <typename U>
inline ForwardLinkedList<T, Allocator>& ForwardLinkedList<T, Allocator>::push_front(U&& val)
{
    Node<T>* newNode = createNode(std::forward<U>(val));

    if (isEmpty())
    {
        head = tail = newNode;
    }
    else
    {
        newNode->next = head;
        head = newNode;
    }

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline ForwardLinkedList<T, Allocator>& ForwardLinkedList<T, Allocator>::pop_front()
{
    if (!head) return *this;

    if (head == tail)
    {
        deleteNode(head);
        head = tail = nullptr;
    }
    else
    {
        Node<T>* toDelete = head;
        head = head->next;
        deleteNode(toDelete);
    }

    size--;
    return *this;
}

template <typename T, typename Allocator>
inline void ForwardLinkedList<T, Allocator>::print() const
{
    for (Node<T>* curr = head; curr != nullptr; curr = curr->next)
    {
        std::cout << curr->val << " ";
    }

    std::cout << std::endl;
}

template <typename T, typename Allocator>
template <typename U>
inline typename ForwardLinkedList<T, Allocator>::Iterator ForwardLinkedList<T, Allocator>::insert(const Iterator& it, U&& val)
{
    if (it == begin())
    {
        push_front(std::forward<U>(val));
        return begin();
    }
    else if (it == end())
    {
        push_back(std::forward<U>(val));
        return end();
    }
    else
    {
        Node<T>* newNode = createNode(std::forward<U>(val));
        Node<T>* currNode = it.ptr;
        
        Node<T>* temp = head;
        while (temp->next != currNode) temp = temp->next;

        temp->next = newNode;
        newNode->next = currNode;

        size++;
        return Iterator(*this, newNode);
    }
}

template <typename T, typename Allocator>
inline typename ForwardLinkedList<T, Allocator>::Iterator ForwardLinkedList<T, Allocator>::remove(const Iterator& it)
{
    Node<T>* toDelete = it.ptr;

    if (toDelete == head)
    {
        pop_front();
        return begin();
    }
    else if (toDelete == tail)
    {
        pop_back();
        return end();
    }
    else
    {
        Node<T>* temp = head;
        while (temp->next != toDelete) temp = temp->next;
        temp->next = toDelete->next;

        deleteNode(toDelete);

        size--;
        return Iterator(*this, temp->next);
    }
}

template <typename T, typename Allocator>
template <typename U>
inline Node<T>* ForwardLinkedList<T, Allocator>::createNode(U&& val)
{
    Node<T>* newNode = allocator.allocate(1);
    AllocatorT::construct(allocator, newNode, std::forward<U>(val));
    return newNode;
}

template <typename T, typename Allocator>
inline void ForwardLinkedList<T, Allocator>::deleteNode(Node<T>* node)
{
    AllocatorT::destroy(allocator, node);
    allocator.deallocate(node, 1);
}

template <typename T, typename Allocator>
ForwardLinkedList<T, Allocator>::ForwardLinkedList(const ForwardLinkedList<T, Allocator>& other)
{
    copyFrom(other);
}

template <typename T, typename Allocator>
ForwardLinkedList<T, Allocator>& ForwardLinkedList<T, Allocator>::operator=(const ForwardLinkedList<T, Allocator>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

template <typename T, typename Allocator>
ForwardLinkedList<T, Allocator>::ForwardLinkedList(ForwardLinkedList<T, Allocator>&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T, typename Allocator>
ForwardLinkedList<T, Allocator>& ForwardLinkedList<T, Allocator>::operator=(ForwardLinkedList<T, Allocator>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template <typename T, typename Allocator>
ForwardLinkedList<T, Allocator>::~ForwardLinkedList() noexcept
{
    free();
}

template <typename T, typename Allocator>
inline void ForwardLinkedList<T, Allocator>::copyFrom(const ForwardLinkedList<T, Allocator>& other)
{
    for (Node<T>* curr = other.head; curr != nullptr; curr = curr->next)
    {
        push_back(other.head->val);
    }
}

template <typename T, typename Allocator>
inline void ForwardLinkedList<T, Allocator>::moveFrom(ForwardLinkedList<T, Allocator>&& other) noexcept
{
    head = std::exchange(other.head, nullptr);
    tail = std::exchange(other.tail, nullptr);
    size = std::exchange(other.size, 0);
}

template <typename T, typename Allocator>
inline void ForwardLinkedList<T, Allocator>::free() noexcept
{
    Node<T>* curr = head;

    while (curr != nullptr)
    {
        Node<T>* toDelete = curr;
        curr = curr->next;
        AllocatorT::destroy(allocator, toDelete);
        allocator.deallocate(toDelete, 1);
    }
}