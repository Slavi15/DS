#pragma once

#include <iostream>
#include <memory>
#include <utility>

#include "Iterator.hpp"

template <typename T, typename Allocator = std::allocator<Node<T>>>
class LinkedList
{
private:
    friend class LLIterator<T>;
    friend class ConstLLIterator<T>;

private:
    Node<T>* head;
    Node<T>* tail;
    std::size_t size;

    Allocator allocator;
    using AllocatorT = std::allocator_traits<Allocator>;

public:
    using Iterator = LLIterator<T>;
    using ConstIterator = ConstLLIterator<T>;

    Iterator begin()
    {
        return Iterator(*this, head);
    }

    Iterator end()
    {
        return Iterator(*this, nullptr);
    }

    ConstIterator cbegin()
    {
        return ConstIterator(*this, head);
    }

    ConstIterator cend()
    {
        return ConstIterator(*this, nullptr);
    }

public:
    LinkedList();

    std::size_t getSize() const;
    bool isEmpty() const;

    template <typename U>
    LinkedList<T, Allocator>& push_back(U&& val);
    LinkedList<T, Allocator>& pop_back();

    template <typename U>
    LinkedList<T, Allocator>& push_front(U&& val);
    LinkedList<T, Allocator>& pop_front();

    template <typename U>
    Iterator insert(const Iterator& it, U&& val);
    Iterator remove(const Iterator& it);

    LinkedList(const LinkedList<T, Allocator>& other);
    LinkedList<T, Allocator>& operator=(const LinkedList<T, Allocator>& other);

    LinkedList(LinkedList<T, Allocator>&& other) noexcept;
    LinkedList<T, Allocator>& operator=(LinkedList<T, Allocator>&& other) noexcept;

    ~LinkedList() noexcept;

private:
    template <typename U>
    Node<T>* createNode(U&& val);

    void deleteNode(Node<T>* node);

    void copyFrom(const LinkedList<T, Allocator>& other);
    void moveFrom(LinkedList<T, Allocator>&& other) noexcept;
    void free() noexcept;
};

template <typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList() : head(nullptr), tail(nullptr), size(0)
{
}

template <typename T, typename Allocator>
inline std::size_t LinkedList<T, Allocator>::getSize() const
{
    return size;
}

template <typename T, typename Allocator>
inline bool LinkedList<T, Allocator>::isEmpty() const
{
    return size == 0;
}

template <typename T, typename Allocator>
template <typename U>
inline LinkedList<T, Allocator>& LinkedList<T, Allocator>::push_back(U&& val)
{
    Node<T>* newNode = createNode(std::forward<U>(val));

    newNode->prev = tail;
    newNode->next = nullptr;

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
inline LinkedList<T, Allocator>& LinkedList<T, Allocator>::pop_back()
{
    if (!tail) return *this;

    if (head == tail)
    {
        deleteNode(tail);
        head = tail = nullptr;
    }
    else
    {
        Node<T>* toDelete = tail;

        tail = tail->prev;
        tail->next = nullptr;

        deleteNode(toDelete);
    }

    size--;
    return *this;
}

template <typename T, typename Allocator>
template <typename U>
inline LinkedList<T, Allocator>& LinkedList<T, Allocator>::push_front(U&& val)
{
    Node<T>* newNode = createNode(std::forward<U>(val));

    newNode->next = head;
    newNode->prev = nullptr;

    if (isEmpty())
    {
        head = tail = newNode;
    }
    else
    {
        head->prev = newNode;
        head = newNode;
    }

    size++;
    return *this;
}

template <typename T, typename Allocator>
inline LinkedList<T, Allocator>& LinkedList<T, Allocator>::pop_front()
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
        head->prev = nullptr;

        deleteNode(toDelete);
    }

    size--;
    return *this;
}

template <typename T, typename Allocator>
template <typename U>
inline typename LinkedList<T, Allocator>::Iterator LinkedList<T, Allocator>::insert(const Iterator& it, U&& val)
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

        newNode->next = currNode;
        newNode->prev = currNode->prev;
        if (newNode->prev) newNode->prev->next = newNode;
        currNode->prev = newNode;

        size++;
        return Iterator(*this, newNode);
    }
}

template <typename T, typename Allocator>
inline typename LinkedList<T, Allocator>::Iterator LinkedList<T, Allocator>::remove(const Iterator& it)
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
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
        Node<T>* nextNode = toDelete->next;

        deleteNode(toDelete);

        size--;
        return Iterator(*this, nextNode);
    }
}

template <typename T, typename Allocator>
template <typename U>
inline Node<T>* LinkedList<T, Allocator>::createNode(U&& val)
{
    Node<T>* newNode = allocator.allocate(1);
    AllocatorT::construct(allocator, newNode, std::forward<U>(val));
    return newNode;
}

template <typename T, typename Allocator>
inline void LinkedList<T, Allocator>::deleteNode(Node<T>* node)
{
    AllocatorT::destroy(allocator, node);
    allocator.deallocate(node, 1);
}

template <typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(const LinkedList<T, Allocator>& other)
{
    copyFrom(other);
}

template <typename T, typename Allocator>
LinkedList<T, Allocator>& LinkedList<T, Allocator>::operator=(const LinkedList<T, Allocator>& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }

    return *this;
}

template <typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(LinkedList<T, Allocator>&& other) noexcept
{
    moveFrom(std::move(other));
}

template <typename T, typename Allocator>
LinkedList<T, Allocator>& LinkedList<T, Allocator>::operator=(LinkedList<T, Allocator>&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }

    return *this;
}

template <typename T, typename Allocator>
LinkedList<T, Allocator>::~LinkedList() noexcept
{
    free();
}

template <typename T, typename Allocator>
inline void LinkedList<T, Allocator>::copyFrom(const LinkedList<T, Allocator>& other)
{
    for (Node<T>* curr = other.head; curr != nullptr; curr = curr->next)
    {
        push_back(curr->val);
    }
}

template <typename T, typename Allocator>
inline void LinkedList<T, Allocator>::moveFrom(LinkedList<T, Allocator>&& other) noexcept
{
    head = std::exchange(other.head, nullptr);
    tail = std::exchange(other.tail, nullptr);
    size = std::exchange(other.size, 0);
}

template <typename T, typename Allocator>
inline void LinkedList<T, Allocator>::free() noexcept
{
    Node<T>* curr = head;

    while (curr != nullptr)
    {
        Node<T>* toDelete = curr;
        curr = curr->next;
        deleteNode(toDelete);
    }
}