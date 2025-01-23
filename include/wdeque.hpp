#ifndef WDEQUE_HPP__
#define WDEQUE_HPP__

#include "witerator.hpp"
#include "wmemory.hpp"
#include "walgorithm.hpp"
#include "uninitialized.hpp"

namespace wstl
{

#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

template <class T>
struct deque_buf_size
{
    static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

template <class T, class Ref, class Ptr>
struct deque_iterator : public iterator<random_access_iterator_tag, T>
{
    typedef deque_iterator  self;

    typedef Ref             reference;
    typedef ptrdiff_t       difference_type;
    typedef T**             map_pointer;
    typedef T*              value_pointer;
    typedef size_t          size_type;

    value_pointer   cur;
    value_pointer   first;
    value_pointer   last;
    map_pointer     node;

    static const size_type buffer_size = deque_buf_size<T>::value;

    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    difference_type operator-(const self& x) const {
        return static_cast<difference_type>(buffer_size) * (node - x.node) + (cur - first) - (x.cur - x.first);
    }

    self operator+(difference_type n) const {
        self tmp = *this;
        return tmp += n;
    }

    reference operator*() const {
        return *cur;
    }

    self& operator+=(difference_type n)
    {
        const auto offset = n + (cur - first);
        if(offset >= 0 && offset < static_cast<difference_type>(buffer_size)) {
            cur += n;
        }
        else {
            const auto node_offset = offset > 0
                                    ? offset / static_cast<difference_type>(buffer_size)
                                    : -static_cast<difference_type>((-offset - 1) / buffer_size) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
        }

        return *this;
    }

    self& operator+(difference_type n) {
        self tmp = *this;
        return tmp += n;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }
};

template <class T>
class deque
{

public:
    typedef wstl::allocator<T>                  allocator_type;
    typedef wstl::allocator<T>                  data_allocator;
    typedef wstl::allocator<T*>                 map_allocator;

    typedef typename allocator_type::value_type value_type;
    typedef typename allocator_type::pointer    pointer;
    typedef typename allocator_type::size_type  size_type;
    typedef typename allocator_type::reference  reference;

    typedef pointer*                            map_pointer;
    typedef deque_iterator<T, T&, T*>           iterator;

    static const size_type buffer_size = deque_buf_size<T>::value;

// private member
private:
    iterator            begin_;
    iterator            end_;
    map_pointer         map_;
    size_type           map_size_;

// public function
public:
    // construct/copy/move/destruct
    
    deque() {
        fill_init(0, value_type());
    }

    explicit deque(size_type n) {
        fill_init(n, value_type());
    }

    deque(size_type n, const value_type& value) {
        fill_init(n, value);
    }

    template <class IIter, typename std::enable_if<
                wstl::is_input_iterator<IIter>::value,int>::type = 0>
    deque(IIter first, IIter last) {
        copy_init(first, last, iterator_category(first));
    }

public:
    
    // iterator related

    iterator begin() noexcept {
        return begin_;
    }

    iterator end() noexcept {
        return  end_;
    }

    // capacity
    size_type size() const noexcept {
        return end_ - begin_;
    }
    size_type max_size() const noexcept {
        return static_cast<size_type>(-1);
    }

    // visit
    reference operator[](size_type n) {
        WSTL_DEBUG(n < size());
        return begin_[n];
    }

    // emplace

    template <class ...Args>
    void emplace_back(Args&& ...args);


// private member function
private:

    // create/destroy node
    map_pointer create_map(size_type size);
    void create_buffer(map_pointer nstart, map_pointer nfinish);

    // initialize
    void map_init(size_type nElem);
    void fill_init(size_type n, const value_type& value);

    template <class IIter>
    void copy_init(IIter, IIter, input_iterator_tag);

    template <class IIter>
    void copy_init(IIter, IIter, forward_iterator_tag);

    // reallocate
    void require_capacity(size_type n, bool front);
    void reallocate_map_at_front(size_type need);
    void reallocate_map_at_back(size_type need);
};

/** implementation of private member functions start */

template <class T>
typename deque<T>::map_pointer deque<T>::create_map(size_type size)
{
    map_pointer mp = nullptr;
    mp = map_allocator::allocate(size);
    for(size_type i = 0; i < size; ++i) {
        *(mp+1) = nullptr;
    }
    return mp;
}

template <class T>
void deque<T>::create_buffer(map_pointer nstart, map_pointer nfinish)
{
    map_pointer cur = nullptr;
    try
    {
        for(cur = nstart; cur <= nfinish; ++cur) {
            *cur = data_allocator::allocate(buffer_size);
        }
    }
    catch(...)
    {
        while (cur != nstart)
        {
            --cur;
            data_allocator::deallocate(*cur, buffer_size);
            *cur = nullptr;
        }
        throw;
    }
    
}

template <class T>
void deque<T>::map_init(size_type nElem)
{
    const size_type nNode = nElem / buffer_size + 1;
    map_size_ = wstl::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), nNode+2);
    try
    {
        map_ = create_map(map_size_);
    }
    catch(...)
    {
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }

    map_pointer nstart = map_ + (map_size_ - nNode) / 2;
    map_pointer nfinish = nstart + nNode - 1;
    try
    {
        create_buffer(nstart, nfinish);
    }
    catch(...)
    {
        map_allocator::deallocate(map_, map_size_);
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }
    
    begin_.set_node(nstart);
    end_.set_node(nfinish);
    begin_.cur = begin_.first;
    end_.cur = end_.first + (nElem % buffer_size);
}

template <class T>
void deque<T>::fill_init(size_type n, const value_type& value)
{
    map_init(n);
    if(0 != n) {
        for(auto cur = begin_.node; cur < end_.node; ++cur) {
            wstl::uninitialized_fill(*cur, *cur+buffer_size, value);
        }
        wstl::uninitialized_fill(end_.first, end_.cur, value);
    }
}

template <class T>
template <class IIter>
void deque<T>::copy_init(IIter first, IIter last, input_iterator_tag)
{
    const size_type n = wstl::distance(first, last);
    map_init(n);
    for(; first != last; ++first) {
        emplace_back(*first);
    }
}

template <class T>
template <class IIter>
void deque<T>::copy_init(IIter first, IIter last, forward_iterator_tag)
{
    const size_type n = wstl::distance(first, last);
    map_init(n);
    for(auto cur = begin_.node; cur < end_.node; ++cur) {
        auto next = first;
        wstl::advance(next, buffer_size);
        wstl::uninitialized_copy(first, next, *cur);
        first = next;
    }
    wstl::uninitialized_copy(first, last, end_.first);
}

template <class T>
void deque<T>::reallocate_map_at_front(size_type need_buffer)
{
    const size_type new_map_size = wstl::max(map_size_ << 1, map_size_ + need_buffer + DEQUE_MAP_INIT_SIZE);
    map_pointer new_map = create_map(new_map_size);
    const size_type old_buffer = end_.node - begin_.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;

    auto begin = new_map + (new_map_size - new_buffer) / 2;
    auto mid = begin + need_buffer;
    auto end = mid + need_buffer;
    create_buffer(begin, mid-1);

    for(auto begin1 = mid, begin2 = begin_.node; begin1 != end; ++begin1, begin2) {
        *begin1 = *begin2;
    }

    map_allocator::deallocate(map_, map_size_);
    map_ = new_map;
    map_size_ = new_map_size;
    begin_ = iterator(*mid + (begin_.cur - begin_.first), mid);
    end_ = iterator(*(end - 1) + (end_.cur - end_.first), end - 1);
}

template <class T>
void deque<T>::reallocate_map_at_back(size_type need_buffer)
{
    const size_type new_map_size = wstl::max(map_size_ << 1, map_size_ + need_buffer + DEQUE_MAP_INIT_SIZE);
    map_pointer new_map = create_map(new_map_size);
    const size_type old_buffer = end_.node - begin_.node + 1;
    const size_type new_buffer = old_buffer + need_buffer;

    auto begin = new_map + ((new_map_size - new_buffer) / 2);
    auto mid = begin + old_buffer;
    auto end = mid + need_buffer;

    for(auto begin1 = begin, begin2 = begin_.node; begin1 != mid; ++begin1, begin2){
        *begin1 = *begin2;
    }
    create_buffer(mid, end-1);

    map_allocator::deallocate(map_, map_size_);
    map_ = new_map;
    map_size_ = new_map_size;
    begin_ = iterator(*begin + (begin_.cur - begin_.first), begin);
    end_ = iterator(*(mid - 1) + (end_.cur - end_.first), mid - 1);
}

template <class T>
void deque<T>::require_capacity(size_type n, bool front)
{
    if(front && (static_cast<size_type>(begin_.cur - begin_.first) < n)) {
        const size_type need_buffer = (n - (begin_.cur - begin_.first)) / buffer_size + 1;
        if(need_buffer > static_cast<size_type>(begin_.node - map_)) {
            reallocate_map_at_front(need_buffer);
            return;
        }
        create_buffer(begin_.node - need_buffer, begin_.node - 1);
    }
    else if (!front && (static_cast<size_type>(end_.last - end_.cur - 1) < n)) {
        const size_type need_buffer = (n - (end_.last - end_.cur - 1)) / buffer_size + 1;
        if(need_buffer > static_cast<size_type>((map_ + map_size_) - end_.node)) {
            reallocate_map_at_back(need_buffer);
            return;
        }
        create_buffer(end_.node + 1, end_.node + need_buffer);
    }
}

/** implementation of private member functions end */


/****** implementation of public member functions start ********* */
template <class T>
template <class ...Args>
void deque<T>::emplace_back(Args&& ...args)
{
    if(end_.cur != end_.last - 1) {
        data_allocator::construct(end_.cur, wstl::forward<Args>(args)...);
        ++end_.cur;
    }
    else {
        require_capacity(1, false);
        data_allocator::construct(end_.cur, wstl::forward<Args>(args)...);
        ++end_;
    }
}

}   // wstl

#endif