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
    list_iterator(const list_iterator& rhs) : node_(rhs.node_){}
    
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

    self operator++(int) {
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
struct list_const_iterator : public wstl::iterator<wstl::bidirectional_iterator_tag,T>
{
    typedef T                                       value_type;
    typedef const T*                                      pointer;
    typedef const T&                                      reference;
    typedef typename node_traits<T>::base_ptr       base_ptr;
    typedef typename node_traits<T>::node_ptr       node_ptr;
    typedef list_const_iterator<T>                  self;

    base_ptr node_;

    list_const_iterator() = default;
    list_const_iterator(base_ptr p) : node_(p){}
    list_const_iterator(node_ptr p) : node_(p->as_base()){}
    list_const_iterator(const list_const_iterator& rhs) : node_(rhs.node_){}
    list_const_iterator(const list_iterator<T>& rhs) : node_(rhs.node_){}
    
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
    typedef list_const_iterator<T>                  const_iterator;
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

    template <class Iter, typename std::enable_if<
        wstl::is_input_iterator<Iter>::value, int>::type = 0>
    list(Iter first, Iter last) {
        copy_init(first, last);
    }

    list(std::initializer_list<T> ilist) {
        copy_init(ilist.begin(), ilist.end());
    }

    list(const list& rhs) {
        copy_init(rhs.cbegin(), rhs.cend());
    }

    list(list&& rhs) noexcept
        : node_(rhs.node_), size_(rhs.size_) {
        rhs.node_ = nullptr;
        rhs.size_ = 0;
    }

    list& operator=(const list& rhs) {
        if(this != &rhs) {
            assign(rhs.begin(), rhs.end());
        }
        return *this;
    }

    list& operator=(list&& rhs) noexcept {
        clear();

    }

    // erase / clear
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void    clear();

    // capacity
    bool empty() const noexcept {
        return node_->next == node_;
    }

    size_type max_size() const noexcept {
        return static_cast<size_type>(-1);
    }

    size_type size() {
        return size_;
    }

    // iterator
    iterator begin() noexcept {
        return node_->next;
    }
    const_iterator begin() const noexcept {
        return node_->next;
    }

    iterator end() noexcept {
        return node_;
    }

    const_iterator end() const noexcept {
        return node_;
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    // resize list size func
    template <class Iter, typename std::enable_if<
        wstl::is_input_iterator<Iter>::value, int>::type = 0>
    void assign(Iter first, Iter last) {
        copy_assign(first, last);
    }

    void assign(std::initializer_list<T> ilist) {
        copy_assign(ilist.begin(), ilist.end());
    }

private:

    // create / destroy
    template <class ...Args>
    node_ptr    create_node(Args&& ...args);
    void        destroy_node(node_ptr p);

    // initialize
    void    fill_init(size_type n, const value_type& value);
    template <class Iter>
    void    copy_init(Iter first, Iter last);

    // assign
    template <class Iter>
    void    copy_assign(Iter first, Iter last);
    
    // insert
    iterator    fill_insert(const_iterator pos, size_type n, const value_type& value);

    template    <class Iter>
    iterator    copy_insert(const_iterator pos, size_type n, Iter first);

    // link / unlink
    iterator    link_iter_node(const_iterator pos, base_ptr node);
    void        link_nodes_at_front(base_ptr first, base_ptr last);
    void        link_nodes_at_back(base_ptr first, base_ptr last);
    void        link_nodes(base_ptr pos, base_ptr first, base_ptr last);
    void        unlink_nodes(base_ptr f, base_ptr l);

    // insert
    iterator insert(const_iterator pos, const value_type& value) {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(value);
        ++size_;
        return link_iter_node(pos, link_node->as_base());
    }

    iterator insert(const_iterator pos, value_type&& value) {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(wstl::move(value));
        ++size_;
        return link_iter_node(pos, link_node->as_base());
    }

    iterator insert(const_iterator pos, size_type n, const value_type& value) {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
        return fill_insert(pos, n , value);
    }

    template <class Iter, typename std::enable_if<wstl::is_input_iterator<Iter>::value, int>::type = 0>
    iterator insert(const_iterator pos, Iter first, Iter last) {
        size_type n = wstl::distance(first, last);
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
        return copy_insert(pos, n, first);
    }

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
typename list<T>::iterator list<T>::erase(const_iterator pos)
{
    WSTL_DEBUG(pos != cend());
    auto n = pos.node_;
    auto next = n->next;
    unlink_nodes(n,n);
    destroy_node(n->as_node());
    --size_;
    return iterator(next);
}

template <class T>
typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last)
{
    if(first != last) {
        unlink_nodes(first.node_, last.node_->prev);
        while (first != last)
        {
            auto cur = first.node_;
            ++first;
            destroy_node(cur->as_node());
            --size_;
        }
    }
    return iterator(last.node_);
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
template <class Iter>
void list<T>::copy_init(Iter first, Iter last)
{
    node_ = base_allocator::allocate(1);
    node_->unlink();
    size_type n = wstl::distance(first, last);
    size_ = n;
    try
    {
        for(; n > 0; --n, ++first) {
            auto node = create_node(*first);
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
template <class Iter>
void list<T>::copy_assign(Iter f2, Iter l2)
{
    iterator f1 = begin();
    iterator l1 = end();
    for(; f1 != l1 && f2 != l2; ++f1, ++f2) {
        *f1 = *f2;
    }
    if(f2 == l2) {
        erase(f1, l1);
    }
    else {
        insert(l1, f2, l2);
    }
}

template <class T>
typename list<T>::iterator list<T>::fill_insert(const_iterator pos, size_type n, const value_type& value)
{
    iterator r(pos.node_);
    if(0 != n) {
        const auto add_size = n;
        auto node = create_node(value);
        node->prev = nullptr;
        r = iterator(node);
        iterator end = r;
        try {
            for(--n; n > 0; --n, ++end) {
                auto next = create_node(value);
                end.node_->next = next->as_base();
                next->prev = end.node_;
            }
            size_ += add_size;
        }
        catch(...) {
            auto enode = end.node_;
            while (true)
            {
                auto prev = enode->prev;
                destroy_node(enode->as_node());
                if(nullptr == prev) break;
                enode = prev;
            }
            throw;
        }
        link_nodes(pos.node_, r.node_, end.node_);
    }
    return r;
}

template <class T>
template <class Iter>
typename list<T>::iterator list<T>::copy_insert(const_iterator pos, size_type n, Iter first)
{
    iterator r(pos.node_);
    if(0 != n) {
        const auto add_size = n;
        auto node = create_node(*first);
        node->prev = nullptr;
        r = iterator(node);
        iterator end = r;
        try
        {
            for(--n, ++first; n > 0; --n, ++first, ++end) {
                auto next = create_node(*first);
                end.node_->next = next->as_base();
                next->prev = end.node_;
            }
            size_ += add_size;
        }
        catch(...)
        {
            auto enode = end.node_;
            while (true)
            {
                auto prev = enode->prev;
                destroy_node(enode->as_node());
                if(prev == nullptr) break;
                enode = prev;
            }
            throw;
        }
        link_nodes(pos.node_, r.node_, end.node_);
    }
    return r;
}

template <class T>
typename list<T>::iterator list<T>::link_iter_node(const_iterator pos, base_ptr link_node)
{
    if(pos == node_->next) {
        link_nodes_at_front(link_node);
    }
    else if(pos == node_) {
        link_nodes_at_back(link_node, link_node);
    }
    else {
        link_nodes(pos.node_, link_node, link_node);
    }
    return iterator(link_node);
}

template <class T>
void list<T>::link_nodes_at_front(base_ptr first, base_ptr last)
{
    first->prev = node_;
    last->next = node_->next;
    last->next->prev = last;
    node_->next = first;
}


template <class T>
void list<T>::link_nodes_at_back(base_ptr first, base_ptr last)
{
    last->next = node_;
    first->prev = node_->prev;
    first->prev->next = first;
    node_->prev = last;
}

template <class T>
void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last)
{
    pos->prev->next = first;
    first->prev = pos->prev;
    pos->prev = last;
    last->next = pos;
}


template <class T>
void list<T>::unlink_nodes(base_ptr first, base_ptr last)
{
    first->prev->next = last->next;
    last->next->prev = first->prev;
}

/*************** private ***************/

}

#endif