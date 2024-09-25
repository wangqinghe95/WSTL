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
    LOG(value);
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

}   // namespace wstl


#endif

/**
 * [day02]: add compare function [max] 
 *          and fill function, such as [fill_n],[unchecked_fill_n] 
 */