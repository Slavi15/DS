#pragma once

template <typename T>
struct Node
{
    T val;
    Node* next = nullptr;

    Node() = default;
    Node(const T& val) : val(val) {}
    Node(const T& val, Node* next) : val(val), next(next) {}
};