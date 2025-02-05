#ifndef WDEQUE_HPP__
#define WDEQUE_HPP__

#include "witerator.hpp"
#include "wmemory.hpp"
#include "walgorithm.hpp"
#include "uninitialized.hpp"

#include <initializer_list>

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
    typedef deque_iterator<T, T&, T*>               iterator;
    typedef deque_iterator<T, const T&, const T*>   const_iterator;
    typedef deque_iterator                          self;

    typedef Ref                                     reference;
    typedef ptrdiff_t                               difference_type;
    typedef T**                                     map_pointer;
    typedef T*                                      value_pointer;
    typedef size_t                                  size_type;

    value_pointer   cur;
    value_pointer   first;
    value_pointer   last;
    map_pointer     node;

    static const size_type buffer_size = deque_buf_size<T>::value;

    deque_iterator() noexcept
                : cur(nullptr), first(nullptr), last(nullptr), node(nullptr){}

    deque_iterator(value_pointer v, map_pointer n) 
                : cur(v), first(*n), last(*n + buffer_size), node(n){ }

    deque_iterator (const iterator& rhs) 
                : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}

    deque_iterator(iterator&& rhs) noexcept
                : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
    {
        rhs.cur = nullptr;
        rhs.first = nullptr;
        rhs.last = nullptr;
        rhs.node = nullptr;
    }

    deque_iterator (const const_iterator& rhs) 
                : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}

    self& operator=(const iterator& rhs) {
        if(this != &rhs) {
            cur = rhs.cur;
            first = rhs.first;
            last = rhs.last;
            node = rhs.node;
        }
        return *this;
    }

    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    difference_type operator-(const self& x) const {
        return static_cast<difference_type>(buffer_size) * (node - x.node) + (cur - first) - (x.cur - x.first);
    }

    // self operator+(difference_type n) const {
    //     self tmp = *this;
    //     return tmp += n;
    // }

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

    self operator+(difference_type n) const{
        self tmp = *this;
        return tmp += n;
    }

    self& operator++(){
        ++cur;
        if(cur == last) {
            set_node(node+1);
            cur = first;
        }
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {
        if(cur == first) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int) {
        self tmp = *this;
        --*this;
        return tmp;
    }

    self& operator-=(difference_type n) {
        return *this += -n;
    }

    self operator-(difference_type n) const {
        self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    bool operator==(const self& rhs) const {
        return cur == rhs.cur;
    }

    bool operator<(const self& rhs) const {
        return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node);
    }

    bool operator!=(const self& rhs) const {
        return !(*this == rhs);
    }

    bool operator<=(const self& rhs) const {
        return !(rhs < *this);
    }
};

template <class T>
class deque
{

public:
    typedef wstl::allocator<T>                          allocator_type;
    typedef wstl::allocator<T>                          data_allocator;
    typedef wstl::allocator<T*>                         map_allocator;

    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;

    typedef pointer*                                    map_pointer;
    typedef deque_iterator<T, T&, T*>                   iterator;
    typedef deque_iterator<T, const T&, const T*>       const_iterator;

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

    deque(std::initializer_list<value_type> ilist) {
        copy_init(ilist.begin(), ilist.end(), wstl::forward_iterator_tag());
    }

    deque(const deque& rhs) {
        copy_init(rhs.begin(), rhs.end(), wstl::forward_iterator_tag());
    }

    deque(deque&& rhs) noexcept
        : begin_(wstl::move(rhs.begin_))
        , end_(wstl::move(rhs.end_))
        , map_(rhs.map_)
        , map_size_(rhs.map_size_)
    {
        rhs.map_ = nullptr;
        rhs.map_size_ = 0;
    }

    deque& operator=(const deque& rhs);
    deque& operator=(deque&& rhs);

public:
    
    // iterator related
    iterator begin() noexcept {
        return begin_;
    }

    const_iterator begin() const noexcept {
        return begin_;
    }

    iterator end() noexcept {
        return  end_;
    }

    const_iterator end() const noexcept {
        return end_;
    }

    // capacity
    bool empty() const noexcept {
        return begin() == end();
    }
    size_type size() const noexcept {
        return end_ - begin_;
    }
    size_type max_size() const noexcept {
        return static_cast<size_type>(-1);
    }
    void shrink_to_fit() noexcept;

    // visit
    reference operator[](size_type n) {
        WSTL_DEBUG(n < size());
        return begin_[n];
    }

    reference front() {
        WSTL_DEBUG(!empty());
        return *begin();
    }

    const_reference front() const {
        WSTL_DEBUG(!empty());
        return *begin();
    }

    reference back() {
        WSTL_DEBUG(!empty());
        return *(end() - 1);
    }

    const_reference back() const {
        WSTL_DEBUG(!empty());
        return *(end() - 1);
    }

    // emplace

    template <class ...Args>
    void emplace_front(Args&& ...args);
    template <class ...Args>
    void emplace_back(Args&& ...args);

    void push_front(const value_type& value);
    void push_back(const value_type& value);

    void push_front(value_type&& value) {
        emplace_front(wstl::move(value));
    }

    void push_back(value_type&& value) {
        emplace_back(wstl::move(value));
    }

    // pop_front / pop_back
    void pop_back();
    void pop_front();

    // insert()
    iterator insert(iterator position, const value_type& value);
    iterator insert(iterator position, value_type&& value);
    template <class IIter, typename std::enable_if<
                wstl::is_input_iterator<IIter>::value, int>::type = 0>
    void insert(iterator position, IIter first, IIter last)
    {
        insert_dispatch(position, first, last, iterator_category(first));
    }

    // clear / erase
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void clear();

    // swap
    void swap(deque& rhs) noexcept;

// private member function
private:

    // create/destroy node
    map_pointer create_map(size_type size);
    void create_buffer(map_pointer nstart, map_pointer nfinish);
    void destroy_buffer(map_pointer nstart, map_pointer nfinish);

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

    // insert
    template <class... Args>
    iterator insert_aux(iterator position, Args&& ...args);
    template <class FIter>
    void copy_insert(iterator, FIter, FIter, size_type);
    template <class IIter>
    void insert_dispatch(iterator, IIter, IIter, input_iterator_tag);
    template <class FIter>
    void insert_dispatch(iterator, FIter, FIter, forward_iterator_tag);
};

/************* construct fonction */

template <class T>
deque<T>& deque<T>::operator=(const deque& rhs)
{
    if(this != &rhs) {
        const auto len = size();
        if(len >= rhs.size()) {
            erase(wstl::copy(rhs.begin_, rhs.end_, begin_), end_);
        }
        else {
            iterator mid = rhs.begin() + static_cast<difference_type>(len);
            wstl::copy(rhs.begin_, mid, begin_);
            insert(end_, mid, rhs.end_);
        }
    }
    return *this;
}

template <class T>
deque<T>& deque<T>::operator=(deque&& rhs)
{
    clear();
    begin_ = wstl::move(rhs.begin_);
    end_ = wstl::move(rhs.end_);
    map_ = rhs.map_;
    map_size_ = rhs.map_size_;
    rhs.map_ = nullptr;
    rhs.map_size_ = 0;
    return *this;
}

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
void deque<T>::destroy_buffer(map_pointer nstart, map_pointer nfinish)
{
    for(map_pointer n = nstart; n <= nfinish; ++n) {
        data_allocator::deallocate(*n, buffer_size);
        *n = nullptr;
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

    for(auto begin1 = mid, begin2 = begin_.node; begin1 != end; ++begin1, ++begin2) {
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

    for(auto begin1 = begin, begin2 = begin_.node; begin1 != mid; ++begin1, ++begin2){
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

template <class T>
typename deque<T>::iterator deque<T>::insert(iterator position, const value_type& value)
{
    if(position.cur == begin_.cur) {
        push_front(value);
        return begin_;
    }
    else if(position.cur == end_.cur) {
        push_back(value);
        auto tmp = end_;
        --tmp;
        return tmp;
    }
    else {
        return insert_aux(position, value);
    }
}
template <class T>
typename deque<T>::iterator deque<T>::insert(iterator position, value_type&& value)
{
    // todo
    return iterator();
}

template <class T>
template <class... Args>
typename deque<T>::iterator deque<T>::insert_aux(iterator position, Args&& ...args)
{
    const size_type elems_before = position - begin_;
    value_type value_copy = value_type(wstl::forward<Args>(args)...);
    if(elems_before < (size() / 2)) {
        emplace_front(front());
        auto front1 = begin_;
        ++front1;
        auto front2 = front1;
        ++front2;
        position = begin_ + elems_before;
        auto pos = position;
        ++pos;
        wstl::copy(front2, pos, front1);
    }
    else {
        emplace_back(back());
        auto back1 = end_;
        --back1;
        auto back2 = back1;
        --back2;
        position = begin_ + elems_before;
        wstl::copy_backward(position, back2, back1);
    }
    *position = wstl::move(value_copy);
    return position;
}

template <class T>
template <class FIter>
void deque<T>::copy_insert(iterator, FIter, FIter, size_type)
{
    // todo
}

template <class T>
template <class IIter>
void deque<T>::insert_dispatch(iterator position, IIter first, IIter last, input_iterator_tag)
{
    if(last <= first) return;
    const size_type n = wstl::distance(first, last);
    const size_type elems_before = position - begin_;
    if(elems_before < (size() / 2)) {
        require_capacity(n, true);
    }
    else {
        require_capacity(n, false);
    }

    position = begin_ + elems_before;
    auto cur = --last;
    for(size_type i = 0; i < n; ++i, --cur) {
        insert(position, *cur);
    }
}

template <class T>
template <class FIter>
void deque<T>::insert_dispatch(iterator position, FIter first, FIter last, forward_iterator_tag)
{
    if(last <= first) return;
    const size_type n = wstl::distance(first, last);
    if(position.cur == begin_.cur) {
        require_capacity(n, true);
        auto new_begin = begin_ - n;
        try
        {
            wstl::uninitialized_copy(first, last, new_begin);
            begin_ = new_begin;
        }
        catch(...)
        {
            if(new_begin.node != begin_.node) {
                destroy_buffer(new_begin.node, begin_.node - 1);
            }
            throw;
        }
    }
    else if(position.cur == end_.cur) {
        require_capacity(n, false);
        auto new_end = end_ + n;
        try
        {
            wstl::uninitialized_copy(first, last, end_);
            end_ = new_end;
        }
        catch(...)
        {
            if(new_end.node != end_.node){
                destroy_buffer(end_.node + 1, new_end.node);
            }
        }
    }
    else {
        copy_insert(position, first, last, n);
    }
}

/** implementation of private member functions end */


/****** implementation of public member functions start ********* */

template <class T>
void deque<T>::shrink_to_fit() noexcept
{
    for(auto cur = map_; cur < begin_.node; ++cur) {
        data_allocator::deallocate(*cur, buffer_size);
        *cur = nullptr;
    }
    for(auto cur = end_.node + 1; cur < map_ + map_size_; ++cur) {
        data_allocator::deallocate(*cur, buffer_size);
        *cur = nullptr;
    }
}

template <class T>
void deque<T>::push_front(const value_type& value)
{
    if(begin_.cur != begin_.first) {
        data_allocator::construct(begin_.cur - 1, value);
        --begin_.cur;
    }
    else {
        require_capacity(1, true);
        try
        {
            --begin_;
            data_allocator::construct(begin_.cur, value);
        }
        catch(...)
        {
            ++begin_;
            throw;
        }
        
    }
}

template <class T>
void deque<T>::push_back(const value_type& value)
{
    if(end_.cur != end_.last - 1) {
        data_allocator::construct(end_.cur, value);
        ++end_.cur;
    }
    else {
        require_capacity(1, false);
        data_allocator::constrcut(end_.cur, value);
        ++end_;
    }
}

template <class T>
void deque<T>::pop_back()
{
    WSTL_DEBUG(!empty());
    if(end_.cur != end_.first) {
        --end_.cur;
        data_allocator::destroy(end_.cur);
    }
    else {
        --end_;
        data_allocator::destroy(end_.cur);
        destroy_buffer(end_.node + 1, end_.node + 1);
    }
}

template <class T>
void deque<T>::pop_front()
{
    WSTL_DEBUG(!empty());
    if(begin_.cur != begin_.last - 1) {
        data_allocator::destroy(begin_.cur);
        ++begin_.cur;
    }
    else {
        data_allocator::destroy(begin_.cur);
        ++begin_;
        destroy_buffer(begin_.node - 1, begin_.node - 1);
    }
}

template <class T>
typename deque<T>::iterator deque<T>::erase(iterator position)
{
    auto next = position;
    ++next;
    const size_type elems_before = position - begin_;
    if(elems_before < (size() / 2)) {
        wstl::copy_backward(begin_, position, next);
        pop_front();
    }
    else {
        wstl::copy(next, end_, position);
        pop_back();
    }
    return begin_ + elems_before;
}

template <class T>
typename deque<T>::iterator deque<T>::erase(iterator first, iterator last)
{
    if(first == begin_ && last == end_) {
        clear();
        return end_;
    }
    else {
        const size_type len = last -first;
        const size_type elems_before = first - begin_;
        if(elems_before < ((size() - len) / 2)) {
            wstl::copy_backward(begin_, first, last);
            auto new_begin = begin_ + len;
            data_allocator::destroy(begin_.cur, new_begin.cur);
            begin_ = new_begin;
        }
        else {
            wstl::copy(last, end_, first);
            auto new_end = end_ - len;
            data_allocator::destroy(new_end.cur, end_.cur);
            end_ = new_end;
        }
        return begin_ + elems_before;
    }
}

template <class T>
void deque<T>::clear()
{
    for(map_pointer cur = begin_.node + 1; cur < end_.node; ++cur) {
        data_allocator::destroy(*cur, *cur+buffer_size);
    }
    if(begin_.node != end_.node) {
        wstl::destroy(begin_.cur, begin_.last);
        wstl::destroy(end_.first, end_.cur);
    }
    else {
        wstl::destroy(begin_.cur, end_.cur);
    }
    shrink_to_fit();
    end_ = begin_;
}

template <class T>
template <class ...Args>
void deque<T>::emplace_front(Args&& ...args)
{
    if(begin_.cur != begin_.first) {
        data_allocator::construct(begin_.cur - 1, wstl::forward<Args>(args)...);
        --begin_.cur;
    }
    else {
        require_capacity(1, true);
        try
        {
            --begin_;
            data_allocator::construct(begin_.cur, wstl::forward<Args>(args)...);
        }
        catch(...)
        {
            ++begin_;
            throw;
        }
    }
}

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

template <class T>
void deque<T>::swap(deque& rhs) noexcept
{
    if(this != &rhs)
    {
        wstl::swap(begin_, rhs.begin_);
        wstl::swap(end_, rhs.end_);
        wstl::swap(map_, rhs.map_);
        wstl::swap(map_size_, rhs.map_size_);
    }
}

}   // wstl

#endif