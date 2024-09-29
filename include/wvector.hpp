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
#include "walgorithm.hpp"
#include "utils.hpp"
#include "uninitialized.hpp"
#include "wexcepdef.hpp"
#include "wconstruct.hpp"

namespace wstl
{

template <class T>
class vector
{
public:
    typedef wstl::allocator<T>                      allocator_type;
    typedef wstl::allocator<T>                      data_allocator;
    typedef typename allocator_type::value_type     value_type;
    typedef typename allocator_type::reference      reference;
    typedef typename allocator_type::size_type      size_type;

    
    typedef value_type*                             iterator;
    typedef const value_type*                       const_iterator;
    typedef wstl::reverse_iterator<iterator>        reverse_iterator;
    typedef wstl::reverse_iterator<const_iterator>  const_reverse_iterator;

private:
    iterator begin_;
    iterator end_;
    iterator cap_;

public:
    vector() noexcept {
        try_init();
    }
    explicit vector(size_type n) {
        /**
         * if value_type is basic data type, such as char, int, float
         * then value_type() is the default value for char, int, float
         * or value is a self-define data type, it will call construct of value
         */
        fill_init(n, value_type());
    }

    vector(size_type n, const value_type& value) {
        fill_init(n, value);
    }

    template <class Iter, typename std::enable_if<
            wstl::is_input_iterator<Iter>::value,int>::type = 0>
    vector(Iter first, Iter last) {
        WSTL_DEBUG(!(first > last));
        range_init(first, last);
    }

    vector(const vector& rhs) {
        range_init(rhs.begin_, rhs.end_);
    }

    vector(vector&& rhs) noexcept : begin_(rhs.begin_)
                                 , end_(rhs.end_)
                                 , cap_(rhs.cap_)
    {
        rhs.begin_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
    }

    vector(std::initializer_list<value_type> _list) {
        range_init(_list.begin(), _list.end());
    }

    vector& operator=(const vector& rhs);

    vector& operator=(vector&& rhs) noexcept;

    vector& operator=(std::initializer_list<value_type> _list) {
        vector tmp(_list.begin(), _list.end());
        swap(tmp);
        return *this;        
    }

    ~vector() {
        destroy_and_recovery(begin_, end_, cap_ - begin_);
        begin_ = end_ = cap_ = nullptr;
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

    reverse_iterator rbegin()   noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const   noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend()   noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const   noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }
    const_iterator cend() const noexcept {
        return end();
    }
    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }
    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    // capacity related function
    bool empty() const noexcept {
        return begin_ == end_;
    }

    size_type size() const noexcept {
        return static_cast<size_type>(end_ - begin_);
    }

    size_type max_size() const noexcept {
        return static_cast<size_type>(-1) / sizeof(T);
    }

    size_type capacity() const noexcept {
        return static_cast<size_type>(cap_ - begin_); 
    }

    void reverse(size_type n);
    void shrink_to_fit();

    // visit element related function
    reference operator[](size_type n) {
        WSTL_DEBUG(n < size());
        return *(begin_ + n);
    }

    const reference operator[](size_type n) const {
        WSTL_DEBUG(n < size());
        return *(begin_ + n);
    }

    reference at(size_type n) {
        THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
        return (*this)[n];
    }

    const reference at(size_type n) const {
        THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
        return (*this)[n];
    }

    reference front() {
        WSTL_DEBUG(!empty());
        return *begin_;
    }

    const_reference front() const {
        WSTL_DEBUG(!empty());
        return *begin_;
    }

    reference back() {
        WSTL_DEBUG(!empty());
        return *(end_ - 1);
    }

    const reference back() const {
        WSTL_DEBUG(!empty());
        return *(end_ - 1);
    }

    pointer data() noexcept {
        return begin_;
    }

    const_pointer data() const noexcept {
        return begin_;
    }

    void swap(vector& rhs) noexcept;

private:
    void    try_init() noexcept;

    void    fill_init(size_type n, const value_type& value);
    void    init_space(size_type size, size_type c);

    template <class Iter>
    void    range_init(Iter first, Iter last);
    void    destroy_and_recovery(iterator first, iterator last, size_type n);
    void    reinsert(size_type size);
};

template <class T>
vector<T>& vector<T>::operator=(const vector& rhs)
{
    LOG("....");
    if(this != &rhs) {
        const auto len = rhs.size();
        if(len > capacity()) {
            vector tmp(rhs.begin(),rhs.end());
            swap(tmp);
        }
        else if(size() >= len) {
            auto i = wstl::copy(rhs.begin(), rhs.end(), begin());
            data_allocator::destroy(i, end_);
            end_ = begin_ + len;
        }
        else {
            wstl::copy(rhs.begin(),rhs.begin() + size(), begin_);
            wstl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end_);
            cap_ = end_ = begin_ + len;
        }
    }
    return *this;
}

template <class T>
vector<T>& vector<T>::operator=(vector&& rhs) noexcept
{
    destroy_and_recovery(begin_, end_, cap_ - begin_);
    begin_ = rhs.begin_;
    end_ = rhs.end_;
    cap_ = rhs.cap_;
    rhs.begin_ = nullptr;
    rhs.end_ = nullptr;
    rhs.cap_ = nullptr;
    return *this;
}

template <class T>
void vector<T>::swap(vector<T>& rhs) noexcept
{
    if(this != &rhs) {
        wstl::swap(begin_, rhs.begin_);
        wstl::swap(end_, rhs.end_);
        wstl::swap(cap_, rhs.cap_);
    }
}

template <class T>
void swap(vector<T>& lhs, vector<T>& rhs)
{
    lhs.swap(rhs);
}

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
        end_ = begin_ + size;
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

template <class T>
template <class Iter>
void vector<T>::
range_init(Iter first, Iter last)
{
    const size_type len = wstl::distance(first, last);
    const size_type init_size = wstl::max(len, static_cast<size_type>(16));
    init_space(len, init_size);
    wstl::uninitialized_copy(first, last, begin_);
}

template <class T>
void vector<T>::destroy_and_recovery(iterator first, iterator last, size_type n)
{
    data_allocator::destroy(first, last);
    data_allocator::deallocate(first, n);
}

template <class T>
void vector<T>::reinsert(size_type size)
{
    auto new_begin = data_allocator::allocate(size);
    try
    {
        wstl::uninitialized_move(begin_, end_, new_begin);
    }
    catch(...)
    {
        data_allocator::deallocate(new_begin, size);
        throw;
    }

    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = new_begin;
    end_ = begin_ + size;
    cap_ = begin_ + size;    
}

template <class T>
void vector<T>::reverse(size_type n)
{
    if(capacity() < n) {
        THROW_LENGTH_ERROR_IF(n > max_size(), 
            "n can't larger than max_size() in vector<T>::reverse(n)");
        const auto old_size = size();
        auto tmp = data_allocator::allocate(n);
        wstl::uninitialized_move(begin_, end_, tmp);
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = tmp;
        end_ = tmp + old_size;
        cap_ = begin_ + n;
    }
}

template <class T>
void vector<T>::shrink_to_fit()
{
    if(end_ < cap_) {
        reinsert(size());
    }
}

}   // namespace wstl
#endif

/**
 * [day01]: implement a try_init() to verify that the allocator is valid
 * [day02]: add explicit vector(size_type n) and its dependency function
 * [day03]: add copy/move constrcutor and move constructor
 *          add copy/move the assignment opeator function
 * [day04]: add fucntion related with iterator and capacity
 */