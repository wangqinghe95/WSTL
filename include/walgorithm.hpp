#ifndef ALGORITHM_HPP__
#define ALGORITHM_HPP__

/**
 * @file algorithm.hpp
 * @brief data structure and algorithm related function
 * @author wangqinghe
 * @date 9/25/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

#include <cstring>
#include "utils.hpp"
#include "witerator.hpp"

namespace wstl
{

/**
 * @brief max, compare two data
 * @param 
 *      lhs - left parameter used to be compared
 *      rhs - right parameter used to be compared
 * @return
 *      returns the larger for both
 */
template <class T>
const T& max(const T& lhs, const T& rhs)
{
    return lhs < rhs ? rhs : lhs;
}

/**
 * @brief   max, the overload version
 *          use functional class to replace comparsion
 */
template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare comp)
{
    return comp(lhs, rhs) ? rhs : lhs;
}

template <class T>
const T& min(const T& lhs, const T& rhs)
{
    return rhs < lhs ? rhs : lhs;
}

template <class T, class Compare>
const T& min(const T& lhs, const T& rhs, Compare comp)
{
    return comp(rhs, lhs) ? rhs : lhs;
}

template <class Tp, class Size, class Up>
typename std::enable_if<
    std::is_integral<Tp>::value && sizeof(Tp) == 1 && 
    !std::is_same<Tp,bool>::value &&
    std::is_integral<Up>::value && sizeof(Up) == 1, Tp*>::type
unchecked_fill_n(Tp* first, Size n, Up value)
{
    if(n > 0) {
        std::memset(first, (unsigned char)value, (size_t)(n));
    }

    return first + n;
}

template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value)
{
    for(; n > 0; --n, ++first) {
        *first = value;
    }
    return first;
}

template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value)
{
    return unchecked_fill_n(first, n, value);
}

/**
 * 
 */
template <class InputIter, class OutputIter>
OutputIter
unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
                    wstl::input_iterator_tag)
{
    for(; first != last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter
unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
                    wstl::random_access_iterator_tag)
{
    for(auto n = last - first; n > 0; --n, ++first, ++result) {
        *result = *first;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter
unchecked_copy(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_copy_cat(first, last, result, iterator_category(first));
}

template <class Tp, class Up>
typename std::enable_if<
            std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
            std::is_trivially_copy_assignable<Up>::value, Up*>::type
unchecked_copy(Tp* first, Tp* last, Up* result)
{
    const auto n = static_cast<size_t> (last - first);
    if(0 != n) {
        std::memmove(result, first, n * sizeof(Up));
    }
    return result+n;
}

template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_copy(first, last, result);
}

template <class ForwardIter, class T>
void fill_cat(ForwardIter first, ForwardIter last, const T& value,
                    wstl::forward_iterator_tag)
{
    for(; first != last; ++first) {
        *first = value;
    }
}

template <class RandomIter, class T>
void fill_cat(RandomIter first, RandomIter last, const T& value,
                wstl::random_access_iterator_tag)
{
    fill_n(first, last-first, value);
}

template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value) 
{
    fill_cat(first, last, value, iterator_category(first));
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                            BidirectionalIter2 result,wstl::bidirectional_iterator_tag)
{
    while (first != last)
    {
        *--result = wstl::move(*--last);
    }
    return result;    
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_move_backward(BidirectionalIter1 first,
                                            BidirectionalIter1 last,
                                            BidirectionalIter2 result)
{
    return unchecked_move_backward_cat(first, last, result, iterator_category(first));
}

template <class Tp, class Up>
typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type,Up>::value
                        && std::is_trivially_move_assignable<Up>::value,Up*>::type
unchecked_move_backward(Tp* first, Tp* last, Up* result)
{
    const size_t n = static_cast<size_t>(last - first);
    if(0 != n) {
        result -= n;
        std::memmove(result, first, n*sizeof(Up));
    }
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 move_backward(BidirectionalIter1 first,
                                BidirectionalIter1 last,
                                BidirectionalIter2 result)
{
    return unchecked_move_backward(first, last, result);
}

template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_move_assignable<Up>::value,
    Up*>::type
unchecked_move(Tp* first, Tp* last, Up* result)
{
    const size_t n = static_cast<size_t>(last - first);
    if(0 != n) {
        std::memmove(result, first, n * sizeof(Up));
    }
    return result + n;
}

template <class InputIter, class OutputIter>
OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_copy_cat(first, last, result, iterator_category(first));
}

// move
template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result)
{
    return unchecked_move(first, last, result);
}

template <class BidirectioanlIter1, class BidirectioanlIter2>
BidirectioanlIter2 unchecked_copy_backward_cat(BidirectioanlIter1 first, BidirectioanlIter1 last,
                                              BidirectioanlIter2 result, wstl::bidirectional_iterator_tag)
{
    while (first != last)
    {
        *--result = *--last;
    }
    return result;
}

template <class RandomIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
                                                BidirectionalIter2 result, wstl::random_access_iterator_tag)
{
    for(auto n = last - first; n > 0; --n) {
        *--result = *--last;
    }
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_copy_backward(BidirectionalIter1 first
                                        , BidirectionalIter1 last
                                        , BidirectionalIter2 result)
{
    return unchecked_copy_backward_cat(first, last, result, iterator_category(first));
}

template <class Tp, class Up>
typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type,Up>::value
                        && std::is_trivially_copy_assignable<Up>::value, Up*>::type
unchecked_copy_backward(Tp* first, Tp* last, Up* result)
{
    const auto n = static_cast<size_t>(last - first);
    if(0 != n) {
        result -= n;
        std::memmove(result, first, n * sizeof(Up));
    }
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 copy_backward(BidirectionalIter1 first
                                , BidirectionalIter1 last
                                , BidirectionalIter2 result)
{
    return unchecked_copy_backward(first, last, result);
}

/****************** */

template <class InputIter1, class InputIter2>
bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
{
    for(; first1 != last1; ++first1, ++first2) {
        if(*first1 != *first2) {
            return false;
        }
    }
    return true;
}

template <class InputIter1, class InputIter2, class Compared>
bool equal(InputIter1 first1, InputIter2 last1, InputIter2 first2, Compared comp)
{
    for(; first1 != last1; ++first1, ++first2) {
        if(!comp(*first1, *first2)) {
            return false;
        }
    }
    return true;
}

/**
 * lexicographical_compare
 * sort two sequence according to lexicographical
 */
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2)
{
    for(; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if(*first1 < *first2) return true;
        if(*first2 < *first1) return false;
    }

    return first1 == last1 && first2 != last2;
}

template <class InputIter1, class InputIter2, class Compared>
bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                            InputIter2 first2, InputIter2 last2, Compared comp)
{
    for(; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if(comp(*first1 < *first2)) return true;
        if(comp(*first2 < *first1)) return false;
    }

    return first1 == last1 && first2 != last2;
}

template <class InputIter1, class InputIter2, class Compred>
bool lexicographical_compare(const unsigned char* first1,
                            const unsigned char* last1,
                            const unsigned char* first2,
                            const unsigned char* last2)
{
    const auto len1 = last1 - first1;
    const auto len2 = last2 - first2;
    const auto result = std::memcmp(first1, first2, wstl::min(len1,len2));
    return result != 0 ? result < 0 : len1 < len2;
}

template <class InputIter, class T>
InputIter find(InputIter first, InputIter last, const T& value)
{
    while (first != last && *first != value)
    {
        ++first;
    }
    return first;
}

}   // namespace wstl

#endif

/**
 * [day02]: add compare function [max] 
 *          and fill function, such as [fill_n],[unchecked_fill_n]
 * [dao05]: add fill function, [fill], [fill_cat]
*/