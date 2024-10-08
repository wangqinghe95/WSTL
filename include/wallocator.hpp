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
#include "wconstruct.hpp"

namespace wstl
{

template<class T>
class allocator
{
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;
public:
    static T* allocate();
    static T* allocate(size_type n);

    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);

    static void construct(T* ptr);
    static void construct(T* ptr, const T&value);
    static void construct(T* ptr, T&& value);

    template <class ...Args>
    static void construct(T* ptr, Args&& ...args);

    static void destroy(T* ptr);
    static void destroy(T* first, T* last);
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

template <class T>
void allocator<T>::construct(T* ptr)
{
    wstl::construct(ptr);
}

template <class T>
void allocator<T>::construct(T* ptr, const T&value)
{
    wstl::construct(ptr, value);
}

template <class T>
void allocator<T>::construct(T* ptr, T&& value)
{
    wstl::construct(ptr, wstl::move(value));
}

template <class T>
template <class ...Args>
void allocator<T>::construct(T* ptr, Args&& ...args)
{
    wstl::construct(ptr, wstl::forward<Args>(args)...);
}

template <class T>
void allocator<T>::destroy(T* ptr)
{
    wstl::destroy(ptr);
}

template <class T>
void allocator<T>::destroy(T* first, T* last)
{
    wstl::destroy(first, last);
}


}

#endif

/**
 * [day03] add a series of functions of construct and destroy
 * [day05] add some types of T
 */