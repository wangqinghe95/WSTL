#ifndef WQUEUE_HPP__
#define WQUEUE_HPP__

#include "wdeque.hpp"
#include "wvector.hpp"
#include "functional.hpp"
#include "walgorithm.hpp"

namespace wstl
{

template <class T, class Container = wstl::deque<T>>
class queue
{

// member variable
public:
    typedef Container                       container_type;

    typedef typename Container::value_type          value_type;
    typedef typename Container::size_type           size_type;
    typedef typename Container::reference           reference;
    typedef typename Container::const_reference     const_reference;

// member variable
private:
    container_type c_;

// member function
public:
    queue() = default;

    explicit queue(size_type n) : c_(n) {}

    queue(size_type n, const value_type& value) : c_(n, value) {}

    template <class IIter>
    queue(IIter first, IIter last) : c_(first, last){}

    queue(std::initializer_list<T> ilist) : c_(ilist.begin(), ilist.end()) {}

    queue(const Container& c) : c_(c) {}

    queue(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
            : c_(wstl::move(c)) {}

    queue(const queue& rhs) : c_(rhs.c_) {
    }

    queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
            : c_(wstl::move(rhs.c_)) {
    }

    queue& operator=(const queue& rhs) {
        c_ = rhs.c_;
        return *this;
    }

    queue& operator=(queue&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value) {
        c_ = wstl::move(rhs.c_);
        return *this;
    }

    queue& operator=(std::initializer_list<T> list) {
        c_ = list;
        return *this;
    }

    ~queue() = default;

    // way with visiting value
    reference           front()             { return c_.front(); }
    const_reference     front() const       { return c_.front(); }
    reference           back()              { return c_.back(); }
    const_reference     back()  const       { return c_.back(); }

    // capacity related

    size_type size() const noexcept {
        return c_.size();
    }

    bool empty() const noexcept {
        return c_.empty();
    }

    // modify value of container
    template <class ...Args>
    void emplace(Args&& ...args) {
        c_.emplace_back(wstl::forward<Args>(args)...);
    }

    void push(const value_type& value) {
        c_.push_back(value);
    }

    void push(value_type&& value) {
        c_.emplace_back(wstl::move(value));
    }

    void pop() {
        c_.pop_front();
    }

    void clear() {
        while (!empty())
        {
            pop();
        }
        
    }

    void swap(queue& rhs) noexcept(noexcept(wstl::swap(c_, rhs.c_))) {
        wstl::swap(c_, rhs.c_);
    }

// member function
private:

    friend bool operator==(const queue& lhs, const queue& rhs) {
        return lhs.c_ == rhs.c_;
    }

    friend bool operator< (const queue& lhs, const queue& rhs) {
        return lhs.c_ < rhs.c_;
    }

};   // queue

template <class T, class Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

template <class T, class Container>
bool operator==(const queue<T, Container>& lhs, const queue<T,Container>& rhs)
{
    return lhs == rhs;
}

template <class T, class Container>
bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
    return lhs < rhs;
}

template <class T, class Container = wstl::vector<T>,
        class Compare = wstl::less<typename Container::value_type>>
class priority_queue
{
public:
    typedef Container                               container_type;
    typedef Compare                                 value_compare;

    
    typedef typename Container::value_type          value_type;
    typedef typename Container::size_type           size_type;
    typedef typename Container::const_reference     const_reference;

private:
    container_type                  c_;
    value_compare                   comp_;

public:
    priority_queue() = default;

    priority_queue(const Compare& c) : c_(), comp_(c) {}

    explicit priority_queue(size_type n) : c_(n) {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(size_type n, const value_type& value)
            : c_(n, value) 
    {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    template <class IIter>
    priority_queue(IIter first, IIter last) : c_(first, last)
    {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(std::initializer_list<T> ilist) : c_(ilist)
    {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(const Container& s) : c_(s)
    {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(Container&& s) : c_(wstl::move(s))
    {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(const priority_queue& rhs) : c_(rhs.c_), comp_(rhs.comp_)
    {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue(priority_queue&& rhs) : c_(wstl::move(rhs.c_)), comp_(rhs.comp_)
    {
        wstl::make_heap(c_.begin(), c_.end(), comp_);
    }

    priority_queue& operator=(const priority_queue& rhs)
    {
        c_ = rhs.c_;
        comp_ = rhs.comp_;
        wstl::make_heap(c_.begin(), c_.end(), comp_);
        return *this;
    }

    priority_queue& operator=(priority_queue&& rhs)
    {
        c_ = wstl::move(rhs.c_);
        comp_ = rhs.comp_;
        wstl::make_heap(c_.begin(), c_.end(), comp_);
        return *this;
    }

    priority_queue& operator=(std::initializer_list<T> ilist)
    {
        c_ = ilist;
        comp_ = value_compare();
        wstl::make_heap(c_.begin(), c_.end(), comp_);
        return *this;
    }

    ~priority_queue() = default;

public:

    const_reference top() const {
        return c_.front();
    }

    bool empty() const noexcept {
        return c_.empty();
    }

    size_type size() const noexcept {
        return c_.size();
    }

    // modify
    template <class... Args>
    void emplace(Args&& ...args)
    {
        c_.emplace_back(wstl::forward<Args>(args)...);
        wstl::push_heap(c_.begin(), c_.end(), comp_);
    }

    void push(const value_type& value)
    {
        c_.push_back(value);
        wstl::push_heap(c_.begin(), c_.end(), comp_);
    }

    void push(value_type&& value)
    {
        c_.push_back(wstl::move(value));
        wstl::push_heap(c_.begin(), c_.end(), comp_);
    }

    void pop()
    {
        wstl::pop_heap(c_.begin(), c_.end(), comp_);
        c_.pop_back();
    }

    void clear()
    {
        while (!empty())
        {
            pop();
        }
    }

    void swap(priority_queue& rhs) noexcept(noexcept(wstl::swap(c_, rhs.c_)) &&
                                            noexcept(wstl::swap(comp_, rhs.comp_)))
    {
        wstl::swap(c_, rhs.c_);
        wstl::swap(comp_, rhs.comp_);
    }

public:
    friend bool operator==(const priority_queue& lhs, const priority_queue& rhs)
    {
        return lhs.c_ == rhs.c_;
    }

    friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs)
    {
        return lhs.c_ != rhs.c_;
    }

};  // priority queue

template <class T, class Container, class Compare>
bool operator==(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs)
{
    return lhs == rhs;
}

template <class T, class Container, class Compare>
bool operator!=(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs)
{
    return lhs != rhs;
}

template <class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
                priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

}   // wstl

#endif