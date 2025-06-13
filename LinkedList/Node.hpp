#pragma once

template <typename T>
struct Node
{
    T val;
    Node<T>* prev;
    Node<T>* next;

    Node() : prev(nullptr), next(nullptr) {}
    Node(const T& val, Node* prev = nullptr, Node* next = nullptr)
        : val(val), prev(prev), next(next) {}
    Node(T&& val, Node* prev = nullptr, Node* next = nullptr)
        : val(std::move(val)), prev(prev), next(next) {}
};