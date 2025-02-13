#ifndef WQUEUE_HPP__
#define WQUEUE_HPP__

#include "wdeque.hpp"

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

}   // wstl

#endif