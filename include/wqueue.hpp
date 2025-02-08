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

    // capacity related

    size_type size() const noexcept {
        return c_.size();
    }

    reference           front()             { return c_.front(); }
    const_reference     front() const       { return c_.front(); }
    reference           back()              { return c_.back(); }
    const_reference     back()  const       { return c_.back(); }

// member function
private:

};   // queue

}   // wstl

#endif