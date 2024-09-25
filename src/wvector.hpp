#ifndef WVECOTR_HPP__
#define WVECOTR_HPP__

/**
 * @file wvector.hpp
 * @brief A sequential storage container
 * @author wangqinghe
 * @date 9/24/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

#include "wallocator.hpp"
#include "algorithm.hpp"
#include "utils.hpp"
#include "uninitialized.hpp"

namespace wstl
{

template <class T>
class vector
{
public:
    typedef wstl::allocator<T>                      allocator_type;
    typedef wstl::allocator<T>                      data_allocator;
    typedef typename allocator_type::value_type     value_type;
    typedef typename allocator_type::size_type      size_type;
    
    typedef value_type*                             iterator;
    typedef const value_type*                       const_iterator;

private:
    iterator begin_;
    iterator end_;
    iterator cap_;

public:
    vector() noexcept {
        try_init();
    }
    explicit vector(size_type n) {
        fill_init(n, value_type());
        LOG(n, value_type());
    }

public:
    // iterator related operation
    iterator begin() noexcept {
        return begin_;
    }
    const_iterator begin() const noexcept {
        return begin_;
    }
    iterator end() noexcept {
        return end_;
    }
    const_iterator end() const noexcept {
        return end_;
    }

private:
    void try_init() noexcept;

    void fill_init(size_type n, const value_type& value);
    void init_space(size_type size, size_type c);
};

template <class T>
void vector<T>::try_init() noexcept
{
    try
    {
        begin_ = data_allocator::allocate(16);
        end_ = begin_;
        cap_ = begin_ + 16;
    }
    catch(...)
    {
        begin_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
    }
}

template <class T>
void vector<T>::init_space(size_type size, size_type capacity)
{
    try
    {
        begin_ = data_allocator::allocate(capacity);
        end_ = begin_;
        cap_ = begin_ + capacity;
    }
    catch(...)
    {
        begin_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
        throw;
    }    
}

template <class T>
void vector<T>::fill_init(size_type n, const value_type& value)
{
    const size_type init_size = wstl::max(static_cast<size_type>(16), n);
    init_space(n, init_size);
    wstl::uninitialized_fill_n(begin_, n, value);
}

}   // namespace wstl
#endif

/**
 * [day01]: implement a try_init() to verify that the allocator is valid
 * [day02]: add explicit vector(size_type n) and its dependency function
 */