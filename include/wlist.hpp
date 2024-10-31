#ifndef WLIST_HPP__
#define WLIST_HPP__

#include "witerator.hpp"
#include "wallocator.hpp"
#include "wmemory.hpp"
#include "utils.hpp"
#include "wconstruct.hpp"
#include "walgorithm.hpp"

namespace wstl
{

template <class T> struct list_node_base;
template <class T> struct list_node;

template <class T>
struct node_traits
{
    typedef list_node_base<T>*  base_ptr;
    typedef list_node<T>*       node_ptr;
};

template <class T>
struct list_node : public list_node_base<T>
{
    typedef typename node_traits<T>::base_ptr   base_ptr;
    typedef typename node_traits<T>::node_ptr   node_ptr;

    T value;

    list_node() = default;

    list_node(const T&v) : value(v) {}

    list_node(T&& v) : value(wstl::move(v)){}

    base_ptr as_base() {
        return static_cast<base_ptr>(&*this);
    }

    node_ptr self() {
        return static_cast<node_ptr>(&*this);
    }
};

template <class T>
struct list_node_base
{

    typedef typename node_traits<T>::base_ptr   base_ptr;
    typedef typename node_traits<T>::node_ptr   node_ptr;

    base_ptr prev;
    base_ptr next;

    list_node_base() = default;
    node_ptr as_node() {
        return static_cast<node_ptr>(self());
    }

    base_ptr self() {
        return static_cast<base_ptr>(&*this);
    }

    void unlink() {
        prev = next = self();
    }

};

template <class T>
struct list_iterator : public wstl::iterator<wstl::bidirectional_iterator_tag,T>
{
    typedef T                                       value_type;
    typedef T*                                      pointer;
    typedef T&                                      reference;
    typedef typename node_traits<T>::base_ptr       base_ptr;
    typedef typename node_traits<T>::node_ptr       node_ptr;
    typedef list_iterator<T>                        self;

    base_ptr node_;

    list_iterator() = default;
    list_iterator(base_ptr p) : node_(p){}
    list_iterator(node_ptr p) : node_(p->as_base()){}
    list_iterator(const node_ptr& rhs) : node_(rhs.node_){}
    
    reference operator*() const {
        return node_->as_node()->value;
    }

    pointer operator->() const {
        return &(operator*());
    }

    self& operator++() {
        WSTL_DEBUG(node_ != nullptr);
        node_ = node_->next;
        return *this;
    }

    self& operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {
        WSTL_DEBUG(node_ != nullptr);
        node_ = node_->prev;
        return *this;
    }

    self& operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self& rhs) const {
        return node_ == rhs.node_;
    }
    bool operator!=(const self& rhs) const {
        return node_ != rhs.node_;
    }
};

template <class T>
class list
{
public:
    typedef wstl::allocator<T>                      allocator_type;
    typedef wstl::allocator<T>                      data_allocator;
    typedef wstl::allocator<list_node_base<T>>      base_allocator;
    typedef wstl::allocator<list_node<T>>           node_allocator;

    typedef typename allocator_type::value_type     value_type;
    typedef typename allocator_type::size_type      size_type;

    typedef list_iterator<T>                        iterator;
    typedef wstl::reverse_iterator<iterator>        reverse_iterator;

    typedef typename node_traits<T>::base_ptr       base_ptr;
    typedef typename node_traits<T>::node_ptr       node_ptr;
private:
    base_ptr    node_;
    size_type   size_;
public:
    list() {
        fill_init(0, value_type());
    }

    explicit list(size_type n) {
        fill_init(n, value_type());
    }

    list(size_type n, const T& value) {
        fill_init(n, value);
    }

    // template <class Iter, typename std::enable_if<
    //     wstl::is_input_iterator<Iter>::value, int>::type = >
    // list(Iter first, Iter last) {
    //     copy_init(first, last);
    // }

    // erase / clear
    void    clear();

    // capacity
    bool empty() const noexcept {
        return node_->next == node_;
    }

    size_type size() {
        return size_;
    }

    // iterator
    iterator begin() noexcept {
        return node_->next;
    }

private:

    // create / destroy
    template <class ...Args>
    node_ptr    create_node(Args&& ...args);
    void        destroy_node(node_ptr p);

    // initialize
    void    fill_init(size_type n, const value_type& value);

    // link / unlink
    void    link_nodes_at_back(base_ptr first, base_ptr last);
};

/*************** private ***************/
template <class T>
template <class ...Args>
typename list<T>::node_ptr list<T>::create_node(Args&& ...args)
{
    node_ptr p = node_allocator::allocate(1);
    try
    {
        data_allocator::construct(wstl::address_of(p->value), wstl::forward<Args>(args)...);
        p->prev = nullptr;
        p->next = nullptr;
    }
    catch(...)
    {
        node_allocator::deallocate(p);
        throw;
    }
    return p;
}

template <class T>
void list<T>::destroy_node(node_ptr p)
{
    data_allocator::destroy(wstl::address_of(p->value));
    node_allocator::deallocate(p);
}

template <class T>
void list<T>::clear()
{
    if(size_ != 0) {
        auto cur = node_->next;
        for(base_ptr next = cur->next; cur != node_; cur = next, next = cur->next) {
            destroy_node(cur->as_node());
        }
        node_->unlink();
        size_ = 0;
    }
}

template <class T>
void list<T>::fill_init(size_type n, const value_type& value)
{
    node_ = base_allocator::allocate(1);
    node_->unlink();
    size_ = n;

    try
    {
        for(; n > 0; --n) {
            auto node = create_node(value);
            link_nodes_at_back(node->as_base(), node->as_base());
        }
    }
    catch(...)
    {
        clear();
        base_allocator::deallocate(node_);
        node_ = nullptr;
        throw;
    }
}

template <class T>
void list<T>::link_nodes_at_back(base_ptr first, base_ptr last)
{
    last->next = node_;
    first->prev = node_->prev;
    first->prev->next = first;
    node_->prev = last;
}

/*************** private ***************/

}

#endif