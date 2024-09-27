#ifndef W_ALLOCATOR_HPP__
#define W_ALLOCATOR_HPP__

/**
 * @file wvector.hpp
 * @brief responsible for allocate and deallocate memory
 * @author wangqinghe
 * @date 9/24/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

#include <cstddef>
#include "utils.hpp"

namespace wstl
{

template<class T>
class allocator
{
public:
    typedef T               value_type;
    typedef T&              reference;
    typedef size_t          size_type;
public:
    static T* allocate();
    static T* allocate(size_type n);

    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);
};

template <class T>
T* allocator<T>::allocate()
{
    return static_cast<T*>(::operator new(sizeof(T)));
}

template <class T>
T* allocator<T>::allocate(size_type n)
{
    if(0 == n) return nullptr;
    return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <class T>
void allocator<T>::deallocate(T* ptr)
{
    if(nullptr == ptr) return;
    ::operator delete(ptr);
}

template <class T>
void allocator<T>::deallocate(T* ptr, size_type n)
{
    if(nullptr == ptr) return;
    ::operator delete(ptr);
}

}

#endif