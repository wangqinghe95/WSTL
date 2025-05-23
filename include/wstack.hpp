#ifndef WSTACK_HPP__
#define WSTACK_HPP__

#include "wdeque.hpp"

namespace wstl
{

template <class T, class Container = wstl::deque<T>>
class stack
{

public:
    typedef Container                       container_type;

    typedef typename Container::value_type          value_type;
    typedef typename Container::size_type           size_type;
    typedef typename Container::reference           reference;
    typedef typename Container::const_reference     const_reference;

    static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");

private:
    container_type c_;

public:
    stack() = default;

    explicit stack(size_type n) : c_(n){}

    stack(size_type n, const value_type& value) : c_(n, value) {}

    template <class IIter> stack(IIter first, IIter last) : c_(first, last){}

    stack(std::initializer_list<T> ilist) : c_(ilist.begin(), ilist.end()){}

    stack(const Container& c) : c_(c) {
        LOGD("stack(const Container& c)");
    }

    stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value) : c_(wstl::move(c)){
        LOGD("stack(Container&& c)");
    }

    stack(const stack& rhs) : c_(rhs.c_) {
        LOGD("stack(const stack& rhs)");
    }

    stack(stack&& rhs) noexcept (std::is_nothrow_move_constructible<Container>::value) : c_(wstl::move(rhs.c_)){
        LOGD("stack(stack&& rhs)");
    }

    stack& operator=(const stack& rhs) 
    {
        LOGD("stack& operator=");
        c_ = rhs.c_;
        return *this;
    }

    ~stack() = default;

public:

    reference top() {
        return c_.back();
    }
    
    const_reference top() const {
        return c_.back();
    }

    bool empty() const noexcept {
        return c_.empty();
    }

    size_type size() const noexcept {
        return c_.size();
    }

    template <class... Args>
    void emplace(Args&& ...args) {
        return c_.emplace_back(wstl::forward<Args>(args)...);
    }

    void push(const value_type& value) {
        LOGD("push(const value_type& value)");
        c_.push_back(value);
    }

    void push(value_type&& value) {
        LOGD("push(value_type&& value)");
        c_.push_back(wstl::move(value));
    }

    void pop() {
        c_.pop_back();
    }

    void clear() {
        while (!empty())
        {
            pop();
        }
    }

    void swap(stack& rhs) noexcept(noexcept(wstl::swap(c_, rhs.c_))) {
        wstl::swap(c_, rhs.c_);
    }

public:
    friend bool operator==(const stack& lhs, const stack& rhs) {
        return lhs.c_ == rhs.c_;
    }

    friend bool operator<(const stack& lhs, const stack& rhs) {
        return lhs.c_ < rhs.c_;
    }

};  // stack

template <class T, class Container>
bool operator==(const stack<T,Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs == rhs;
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return lhs < rhs;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
    return !(lhs < rhs);
}

template <class T, class Container>
void swap(const stack<T, Container>& lhs, const stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

}   // wstl

#endif