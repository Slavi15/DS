cmake_minimum_required(VERSION 3.10)
project(DS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DS
    main.cpp
    Vector/Vector.hpp
    Vector/Iterator.hpp
    ForwardLinkedList/Node.hpp
    ForwardLinkedList/ForwardLinkedList.hpp
    ForwardLinkedList/Iterator.hpp
    LinkedList/Node.hpp
    LinkedList/Iterator.hpp
    LinkedList/LinkedList.hpp
    Deque/Deque.hpp
    Queue/Queue.hpp
    PriorityQueue/PriorityQueue.hpp
    Stack/Stack.hpp
    HashMap/HashMap.hpp
    HashMap/HashSet.hpp
)

if(MSVC)
    target_compile_options(DS PRIVATE /W4 /WX)
else()
    target_compile_options(DS PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wconversion
        -Wnull-dereference
    )
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
