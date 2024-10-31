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
#include "wconstruct.hpp"
#include "wmemory.hpp"
#include "witerator.hpp"

namespace wstl
{

template <class T>
class vector
{
public:
    typedef wstl::allocator<T>                          allocator_type;
    typedef wstl::allocator<T>                          data_allocator;
    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;

    
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
        LOGD("vector()");
        try_init();
    }
    explicit vector(size_type n) {
        /**
         * if value_type is basic data type, such as char, int, float
         * then value_type() is the default value for char, int, float
         * or value is a self-define data type, it will call construct of value
         */
        LOGD("vector(size_type n)");
        fill_init(n, value_type());
    }

    vector(size_type n, const value_type& value) {
        LOGD("vector(size_type n, const value_type& value)");
        fill_init(n, value);
    }

    template <class Iter, typename std::enable_if<
            wstl::is_input_iterator<Iter>::value,int>::type = 0>
    vector(Iter first, Iter last) {
        LOGD("vector(Iter first, Iter last)");
        WSTL_DEBUG(!(first > last));
        range_init(first, last);
    }

    vector(const vector& rhs) {
        LOGD("vector(const vector& rhs)");
        range_init(rhs.begin_, rhs.end_);
    }

    vector(vector&& rhs) noexcept : begin_(rhs.begin_)
                                 , end_(rhs.end_)
                                 , cap_(rhs.cap_)
    {
        LOGD("vector(vector&& rhs)");
        rhs.begin_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
    }

    vector(std::initializer_list<value_type> _list) {
        LOGD("vector(std::initializer_list<value_type> _list)");
        range_init(_list.begin(), _list.end());
    }

    vector& operator=(const vector& rhs);

    vector& operator=(vector&& rhs) noexcept;

    vector& operator=(std::initializer_list<value_type> _list) {
        LOGD("operator=(std::initializer_list<value_type> _list)");
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
        // LOGI("begin");
        return begin_;
    }
    const_iterator begin() const noexcept {
        // LOGI("const_iterator begin");
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

    void reserve(size_type n);

    // visit element related function
    reference operator[](size_type n) {
        WSTL_DEBUG(n < size());
        return *(begin_ + n);
    }

    const reference operator[](size_type n) const {
        LOGD("const operator[]");
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

    // modify container-related operations
    void assign(size_type n, const value_type &value) {
        fill_assign(n, value);
    }

    template <class Iter, typename std::enable_if<
            wstl::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last) {
        WSTL_DEBUG(!(last < first));
        copy_assign(first, last, iterator_category(first));
    }

    void assign(std::initializer_list<value_type> il) {
        copy_assign(il.begin(), il.end(), wstl::forward_iterator_tag{});
    }

    // emplace / emplace_back
    template <class... Args>
    iterator emplace(const_iterator pos, Args&& ...args);

    template <class... Args>
    void emplace_back(Args&& ...args);

    // push_back / pop_back
    void push_back(const value_type& value);
    void push_back(value_type& value) {
        emplace_back(wstl::move(value));
    }

    void pop_back();

    // insert
    iterator insert(const_iterator pos, const value_type& value);
    iterator insert(const_iterator pos, value_type&& value) {
        return emplace(pos, wstl::move(value));
    }
    iterator insert(const_iterator pos, size_type n, const value_type& value) {
        WSTL_DEBUG(pos >= begin() && pos <= end());
        return fill_insert(const_cast<iterator>(pos), n, value);
    }
    template <class Iter, typename std::enable_if<
            wstl::is_input_iterator<Iter>::value, int>::type = 0>
    void insert(const_iterator pos, Iter first, Iter last)
    {
        WSTL_DEBUG(pos >= begin() && pos <= end() && !(last < first));
        copy_insert(const_cast<iterator>(pos), first, last);
    }

    // erase / clear
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void clear() {
        erase(begin(), end());
    }

    void resize(size_type new_size) {
        return resize(new_size, value_type());
    }

    void resize(size_type new_size, const value_type& value);

    void swap(vector& rhs) noexcept;

private:
    void    try_init() noexcept;

    void    fill_init(size_type n, const value_type& value);
    void    init_space(size_type size, size_type c);

    template <class Iter>
    void    range_init(Iter first, Iter last);
    void    destroy_and_recovery(iterator first, iterator last, size_type n);
    void    fill_assign(size_type n, const value_type& value);

    // calculate the growth size
    size_type get_new_cap(size_type add_size);

    template <class IIter>
    void    copy_assign(IIter first, IIter last, input_iterator_tag);
    template <class FIter>
    void    copy_assign(FIter first, FIter last, forward_iterator_tag);

    // reallocate
    template <class... Args>
    void    reallocate_emplace(iterator pos, Args&& ...args);
    void    reallocate_insert(iterator pos, const value_type& value);

    // insert
    iterator    fill_insert(iterator pos, size_type n, const value_type& value);
    template <class IIter>
    void        copy_insert(iterator pos, IIter first, IIter last);

    void    shrink_to_fit();
    void    reinsert(size_type size);

};

template <class T>
vector<T>& vector<T>::operator=(const vector& rhs)
{
    LOGD("operator=");
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
    LOGD("operator=(vector&& rhs)");
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
void vector<T>::resize(size_type new_size, const value_type& value)
{
    if(new_size < size()) {
        erase(begin() + new_size, end());
    }
    else {
        insert(end(), new_size - size(), value);
    }
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
void vector<T>::reserve(size_type n)
{
    if(capacity() < n) {
        THROW_LENGTH_ERROR_IF(n > max_size(), 
            "n can't larger than max_size() in vector<T>::reserve(n)");
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

template <class T>
void vector<T>::
fill_assign(size_type n, const value_type& value)
{
    if(n > capacity()) {
        vector tmp(n, value);
        swap(tmp);
    }
    else if(n > size()) {
        wstl::fill(begin(), end(), value);
        end_ = wstl::uninitialized_fill_n(end_, n-size(), value);
    }
    else {
        erase(wstl::fill_n(begin_, n, value), end_);
    }
}

template <class T>
template <class... Args>
typename vector<T>::iterator
vector<T>::emplace(const_iterator pos, Args&& ...args)
{
    WSTL_DEBUG(pos >= begin() && pos <= end());
    iterator xpos = const_cast<iterator>(pos);
    const size_type n = xpos - begin_;
    if(end_ != cap_ && xpos == end_) {
        data_allocator::construct(wstl::address_of(*end_), wstl::forward<Args>(args)...);
        ++end_;
    }
    else if(end_ != cap_) {
        auto new_end = end_;
        data_allocator::construct(wstl::address_of(*end_), *(end_-1));
        ++new_end;
        wstl::copy_backward(xpos, end_ - 1, end_);
        *xpos = value_type(wstl::forward<Args>(args)...);
        end_ = new_end;
    }
    else {
        reallocate_emplace(xpos, wstl::forward<Args>(args)...);
    }
    return begin() + n;
}

template <class T>
template <class... Args>
void vector<T>::emplace_back(Args&& ...args)
{
    if(end_ < cap_) {
        data_allocator::construct(wstl::address_of(*end_), wstl::forward<Args>(args)...);
        ++end_;
    }
    else {
        reallocate_emplace(end_, wstl::forward<Args>(args)...);
    }
}

template <class T>
void vector<T>::push_back(const value_type& value)
{
    if(end_ != cap_) {
        data_allocator::construct(wstl::address_of(*end_), value);
        ++end_;
    }
    else {
        reallocate_insert(end_, value);
    }
}

template <class T>
void vector<T>::pop_back()
{
    WSTL_DEBUG(!empty());
    data_allocator::destroy(end_ - 1);
    --end_;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value)
{
    WSTL_DEBUG(pos >= begin() && pos <= end());
    iterator xpos = const_cast<iterator>(pos);
    const size_type n = pos - begin_;
    if(end_ != cap_ && xpos == end_) {
        data_allocator::construct(wstl::address_of(*end_), value);
        ++end_;
    }
    else if(end_ != cap_) {
        auto new_end = end_;
        data_allocator::costruct(wstl::address_of(*end_), *(end_-1));
        ++new_end;
        auto value_copy = value;
        wstl::copy_backward(xpos, end_-1, end_);
        *xpos = wstl::move(value_copy);
        end_ = new_end;
    }
    else {
        reallocate_insert(xpos, value);
    }
    return begin_ + n;
}

template <class T>
typename vector<T>::iterator
vector<T>::erase(const_iterator pos) {
    WSTL_DEBUG(pos >= begin() && pos < end());
    iterator xpos = begin_ + (pos - begin());
    wstl::move(xpos + 1, end_, xpos);
    data_allocator::destroy(end_ - 1);
    --end_;
    return xpos;
}

template <class T>
typename vector<T>::iterator
vector<T>::erase(const_iterator first, const_iterator last) {
    WSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
    const auto n = first - begin();
    iterator it = begin_ + (first - begin_);
    data_allocator::destroy(wstl::move(it+(last - first), end_, it), end_);
    end_ = end_ - (last - first);
    return begin_ + n;
}

template <class T>
typename vector<T>::size_type vector<T>::get_new_cap(size_type add_size)
{
    const auto old_size = capacity();
    THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size,
                            "vector<T>'s size too big");
    if(old_size > max_size() - old_size / 2) {
        return old_size + add_size > max_size() - 16 ?
                old_size + add_size : old_size + add_size + 16;
    }
    const size_type new_size = old_size == 0 ?
                    wstl::max(add_size, static_cast<size_type>(16)) :
                    wstl::max(old_size + old_size / 2, old_size + add_size);
    return new_size;                    
}


template <class T>
template <class IIter>
void vector<T>::copy_assign(IIter first, IIter last, input_iterator_tag)
{
    auto cur = begin_;
    for(; first != last && cur != end_; ++first, ++cur) {
        *cur = *first;
    }

    if(first == last) {
        erase(cur, end_);
    }
    else {
        insert(end_, first, last);
    }
}

template <class T>
template <class FIter>
void vector<T>::copy_assign(FIter first, FIter last, forward_iterator_tag)
{
    const size_type len = wstl::distance(first, last);
    if(len > capacity()) {
        vector tmp(first, last);
        swap(tmp);
    }
    else if(size() >= len) {
        auto new_end = wstl::copy(first, last, begin_);
        data_allocator::destroy(new_end, end_);
        end_ = new_end;
    }
    else {
        auto mid = first;
        wstl::advance(mid, size());
        wstl::copy(first, mid, begin_);
        auto new_end = wstl::uninitialized_copy(mid, last, end_);
        end_ = new_end;
    }
}

template <class T>
template <class... Args>
void vector<T>::reallocate_emplace(iterator pos, Args&& ...args)
{
    const auto new_size = get_new_cap(1);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    try
    {
        new_end = wstl::uninitialized_move(begin_, pos, new_begin);
        data_allocator::construct(wstl::address_of(*new_end), wstl::forward<Args>(args)...);
        ++new_end;
        new_end = wstl::uninitialized_move(pos, end_, new_end);
    }
    catch(...)
    {
        data_allocator::deallocate(new_begin, new_size);
        throw;
    }

    destroy_and_recovery(begin_, end_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = new_begin + new_size;    
}

template <class T>
void vector<T>::reallocate_insert(iterator pos, const value_type& value)
{
    const auto new_size = get_new_cap(1);
    auto new_begin = data_allocator::allocate(new_size);
    auto new_end = new_begin;
    const value_type& value_copy = value;
    try
    {
        new_end = wstl::uninitialized_move(begin_, pos, new_begin);
        data_allocator::construct(wstl::address_of(*new_end), value_copy);
        ++new_end;
        new_end = wstl::uninitialized_move(pos, end_, new_end);
    }
    catch(...)
    {
        data_allocator::deallocate(new_begin, new_size);
        throw;
    }
    
    destroy_and_recovery(begin_, end_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = new_begin + new_size;
}

template <class T>
typename vector<T>::iterator 
vector<T>::fill_insert(iterator pos, size_type n, const value_type& value)
{
    if(0 == n) return pos;
    const size_type xpos = pos - begin_;
    const value_type value_copy = value;
    if(static_cast<size_type>(cap_ - end_) >= n) {
        const size_type after_elems = end_ - pos;
        auto old_end = end_;
        if(after_elems > n) {
            wstl::uninitialized_copy(end_ - n, end_, end_);
            end_ += n;
            wstl::move_backward(pos, old_end - n, old_end);
            wstl::uninitialized_fill_n(pos, n, value_copy);
        }
        else {
            end_ = wstl::uninitialized_fill_n(end_, n - after_elems, value_copy);
            end_ = wstl::uninitialized_move(pos, old_end, end_);
            wstl::uninitialized_fill_n(pos, after_elems, value_copy);

        }
    }
    else {
        const auto new_size = get_new_cap(1);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try
        {
            new_end = wstl::uninitialized_move(begin_, pos, new_begin);
            new_end = wstl::uninitialized_fill_n(new_end, n, value);
            new_end = wstl::uninitialized_move(pos, end_, new_end);
        }
        catch(...)
        {
            destroy_and_recovery(new_begin, new_end, new_size);
            throw;
        }
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = begin_ + new_size;
    }
    return begin_ + xpos;
}

template <class T>
template <class IIter>
void vector<T>::copy_insert(iterator pos, IIter first, IIter last)
{
    if(first == last) return;

    const auto n = wstl::distance(first, last);
    if((cap_ - end_) >= n) {
        const auto after_elems = end_ - pos;
        auto old_end = end_;
        if(after_elems > n) {
            end_ = wstl::uninitialized_copy(end_ - n, end_, end_);
            wstl::move_backward(pos, old_end - n, old_end);
            wstl::uninitialized_copy(first, last, pos);
        }
        else {
            auto mid = first;
            wstl::advance(mid, after_elems);
            end_ = wstl::uninitialized_copy(mid, last, end_);
            end_ = wstl::uninitialized_move(pos, old_end, end_);
            wstl::uninitialized_copy(first, mid, pos);
        }
    }
    else {
        const auto new_size = get_new_cap(1);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try
        {
            new_end = wstl::uninitialized_move(begin_, pos, new_begin);
            new_end = wstl::uninitialized_copy(first, last, new_end);
            new_end = wstl::uninitialized_move(pos, end_, new_end);
        }
        catch(...)
        {
            destroy_and_recovery(new_begin, new_end, new_size);
            throw;
        }

        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = begin_ + new_size;        
    }
}

/******************************************* */
// overload operator

template <class T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs)
{
    return lhs.size() == rhs.size() &&
        wstl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class T>
bool operator<(const vector<T>& lhs, const vector<T>& rhs)
{
    return wstl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T>
bool operator!=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs == rhs);
}

template <class T>
bool operator>(const vector<T>& lhs, const vector<T>& rhs)
{
    return rhs < lhs;
}

template <class T>
bool operator<=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(rhs < lhs);
}

template <class T>
bool operator>=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs < rhs);
}

template <class T>
void swap(vector<T>& lhs, vector<T>& rhs)
{
    lhs.swap(rhs);
}

}   // namespace wstl
#endif

/**
 * [day01]: implement a try_init() to verify that the allocator is valid
 * [day02]: add explicit vector(size_type n) and its dependency function
 * [day03]: add copy/move constrcutor and move constructor
 *          add copy/move the assignment opeator function
 * [day04]: add fucntion related with iterator and capacity
 * [day05]: add some type of T
 *          add member function, [erase], [fill_assign]
 * [day06]: add member function, [assign], [copy_assign]
 * [day07]: add member function, [emplace],[insert],[resize]
 */