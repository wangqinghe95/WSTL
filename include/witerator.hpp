#ifndef W_ITERATOR_HPP__
#define W_ITERATOR_HPP__

/**
 * @file witerator.hpp
 * @brief A abstract class iterator
 * @author wangqinghe
 * @date 9/24/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

#include <type_traits>
#include <cstddef>

namespace wstl
{

template <class T, T v>
struct m_integral_constant
{
    static constexpr T value = v;
};

template <bool b>
using m_bool_constant = m_integral_constant<bool,b>;

typedef m_bool_constant<true> m_true_type;
typedef m_bool_constant<false> m_false_type;

/**
 * @brief iterator_tag, empty constructs, are used for type marking
 */

/**
 * @brief Input iterator
 * @note    1. can only move by one step forward
 *          2. can read data but can't modify them
 */
struct input_iterator_tag{};

/**
 * @brief Output iterator
 * @note    1. can only move one step forward
 *          2. can write data but can't read them
 */
struct output_iterator_tag{};

/**
 * @brief Forward iterator
 * @note    1. can read and write data
 *          2. able to move any numbers of steps forward
 */
struct forward_iterator_tag : public input_iterator_tag{};

/**
 * @brief Bidirectional iterator
 * @note    1. with all of the capabilities of forward iterator
 *          2. able to move backforward
 */
struct bidirectional_iterator_tag : public forward_iterator_tag{};

/**
 * @brief Random access iterator
 * @note    1. with all of the capabilities of bidirectional iterator
 *          2. be able to access randomly the element at any location
 */
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/**
 * @brief A modular iterator
 */
template <class Category
         , class T
         , class Distance = ptrdiff_t
         , class Pointer = T*
         , class Reference = T&>
struct iterator
{
    /** 
     * @brief iterator_category: common categories are [iterator_tag]
     */
    typedef Category                iterator_category;
    /**
     * @brief T: the element type referred to the iterator
     */
    typedef T                       value_type;
    /**
     * @brief pointer: pointer type of the element referred to by the iterator
     */
    typedef Pointer                 pointer;
    /**
     * @brief reference: reference type of the element referred to by the iterator
     */
    typedef Reference               reference;
    /**
     * @brief difference_type: type of distance between the two iterators
     */
    typedef Distance                difference_type; 
};
/**
 * @brief A auxiliary struct
 *          it can help to select the appropriate iterator feature
 *          implementation at compile time based on the iterator feature
 * @param Iterator : iterator type, this structure is usually used to implement
 *                      a specialized version of the iterator feature(iterator traits)
 *                      to provide customized behavior for different types of iterators
 * @param bool : boolean parameter, is used to enable or disable certain features,
 *                   or for conditional compilation at compile-time
 */
template <class Iterator, bool>
struct iterator_traits_impl{};

/**
 * iterator_traits_impl<Iterator, true>: A specialized version of iterator_traits_impl
 *                                      this specialized version will be selected
 *                                      instead of the generic template when iterator_traits_impl
 *                                      is instantiated if the second parameter is true.
 */
template <class Iterator>
struct iterator_traits_impl<Iterator, true>
{
    typedef typename Iterator::iterator_category        iterator_category;
    typedef typename Iterator::value_type               value_type;
    typedef typename Iterator::pointer                  pointer; 
    typedef typename Iterator::reference                reference; 
    typedef typename Iterator::difference_type          difference_type; 
};

/**
 * @brief iterator_traits_helper: A template structure, its function
 *                                  as same as [iterator_traits_impl]
 */
template <class Iterator, bool>
struct iterator_traits_helper{};

/**
 * @brief A auxiliary structure, to handle those structure with
 *              iterator_category member types
 * @param true: indicates that this specialization applies to those iterators
 *              with the iterator_category member type.
 * @note if Iterator::iterator_category can be converted to input_iterator_tag,
 *          then Iterator is an input iterator.
 *       else if Iterator::iterator_category can be converted to output_interator_tag,
 *          then Iterator is an output iterator
 *       else Iterator is an advanced iterator, such forward_iterator_tag
 */         
template <class Iterator>
struct iterator_traits_helper<Iterator, true>
        : public iterator_traits_impl<Iterator,
            std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value || 
            std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>
{}; 

/**
 * @brief A template class is used to detect whether a type T has
 *          iterator_category member type.
 */
template <class T>
struct has_iterator_cat
{
private:
    struct two { char a; char b;};
    /**
     * @brief a function template, it will be called as a backup option
     *           when any other overload is not available
     * @param ... : lists of variable parameters
     * @return struct two: 
     */
    template <class U> static two test(...);
    /**
     * @brief A function template, this version will be seclecte if U has a iterator_category member
     * @param A pointer to U::iterator_category
     * @return An object of type char
     */
    template <class U> static char test(typename U::iterator_category* = 0);
public:
    /**
     * @brief if T has a iterator_category member, then the second test will be selected
     *          so, the value of [sizeof(test<T>(0))] is [ static char], it means results
     *          can be equal with  sizeof(char), and [the const bool value] is true
     *         else the first test will be selected, and the return data is struct two,
     *          in this case, the value of [sizeof(test<T>(0))] is 2,
     *          and the [the const bool value] is false
     * @result value = true, if T has iterator_category member
     *         value = false, T don't has iterator_category member
     */
    static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

/**
 * @brief A template structure, it is used to extract the features of the iterators
 * @note if [has_iterator_cat<Iterator>::value] is true, that means Iterator has a iterator_category member
 *          then iterator_traits inherits a specialized version of iterator_traits_helper with a member variable
 */
template <class Iterator>
struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>
{};

/**
 * @brief A partially specialized version of the iterator_traits,
 *        dedicated to iterators of pointer type
 */
template <class T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag          iterator_category;
    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef T&                                  reference;
    typedef ptrdiff_t                           difference_type;
};

/**
 * @brief A partially specialized version of iterator_traits template
 *        for handling the pointer type const T* to constants
 */
template <class T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag          iterator_category;
    typedef const T                             value_type;
    typedef const T*                            pointer;
    typedef const T&                            reference;
    typedef ptrdiff_t                           difference_type;
};


/*
template <class Iterator>
struct is_iterator : public m_bool_constant<is_input>
*/

/**
 * @brief to test whether the iterator_category of one iterator type
 *           can be converted to another type
 */
template <class T, class U, bool = has_iterator_cat<iterator_traits<T>>::value>
struct has_iterator_cat_of 
    : public m_bool_constant<std::is_convertible
    < typename iterator_traits<T>::iterator_category, U>::value>
{};

template <class T, class U>
struct has_iterator_cat_of<T,U,false> : public m_false_type{};

template <class Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category Category;
    return Category();
}

/**
 * @brief check if an iterator is an input iterator
 */
template <class Iter>
struct is_input_iterator : public has_iterator_cat_of<Iter, input_iterator_tag>
{};

template <class Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
        ++first;
        ++n;
    }
    return n;    
}

template <class RandomIterator>
typename iterator_traits<RandomIterator>::difference_type
distance_dispatch(RandomIterator first, RandomIterator last, random_access_iterator_tag)
{
    return last - first;
}

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    return distance_dispatch(first, last, iterator_category(first));
}

// reverse iterator
template <class Iterator>
class reverse_iterator
{
private:
    Iterator current;
public:
    typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
    typedef typename iterator_traits<Iterator>::value_type          value_type;
    typedef typename iterator_traits<Iterator>::difference_type     difference_type;
    typedef typename iterator_traits<Iterator>::pointer             pointer;
    typedef typename iterator_traits<Iterator>::reference           reference;

    typedef Iterator                                                iterator_type;
    typedef reverse_iterator<Iterator>                              self;

public:
    // constructor
    reverse_iterator(){}
    explicit reverse_iterator(iterator_type iter) : current(iter){}
    reverse_iterator(const self& rhs) : current(rhs.current){}

public:
    // get corresponding forward iterator
    iterator_type base() const {
        return current;
    }

    // reload operator
    reference operator*() const {
        auto tmp = current;
        return *--tmp;
    }
    pointer operator->() const {
        return &(operator*());
    }

    // reload opeartor++
    // reverse iterator(--) means forward iterator(++)
    self& operator++(){
        --current;
        return *this;
    }

    self operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }

    // reverse iterator(++) means forward iterator(--)
    self& operator--() {
        ++current;
        return *this;
    }
    
    self operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }

    self& operator+=(difference_type n) {
        current -= n;
        return *this;
    }

    self operator+(difference_type n) const {
        return self(current - n);
    }

    self& operator-=(difference_type n) {
        current += n;
        return *this;
    }

    self operator-(difference_type n) const {
        return self(current + n);
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }
};

template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return lhs.base() == rhs.base();
}

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
{
    return !(lhs == rhs);
}

template <class InputIterator, class Distance>
void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag)
{
    while(n--) {
        ++i;
    }
}

template <class BidirectionalIterator, class Distance>
void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
    if(n >= 0) {
        while(n--) ++i;
    }
    else {
        while(n++) --i;
    }
}

template <class RandomIter, class Distance>
void advance_dispatch(RandomIter& i, Distance n, random_access_iterator_tag)
{
    i += n;
}

template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n)
{
    advance_dispatch(i, n, iterator_category(i));
}

}   // namespace

#endif

/**
 * [day02]: add template class iterator as well as a series of auxiliary
 *          functions related to it, including [iterator_traits_impl], [iterator_traits_helper]
 *          [has_iterator_cat], [iterator_traits]
 * [day04]: add reverse_iterator class
 * [day06]: add [advance], [advance_dispatch] template function
 */