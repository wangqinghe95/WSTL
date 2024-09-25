#ifndef UNINITIALIZED_HPP__
#define UNINITIALIZED_HPP__

/**
 * @file uninitalized.hpp
 * @brief data initialization, memory space data filling related functions 
 * @author wangqinghe
 * @date 9/25/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

#include "witerator.hpp"
#include "algorithm.hpp"

namespace wstl
{

template <class ForwardIter, class Size, class T>
ForwardIter unchecked_uninit_fill_n(ForwardIter first
                                    , Size n
                                    , const T& value
                                    , std::true_type)
{
    return fill_n(first, n, value);
}

template <class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value)
{
    return wstl::unchecked_uninit_fill_n(first, n, value,
                                        std::is_trivially_copy_assignable<
                                        typename iterator_traits<ForwardIter>::value_type>{});
}


}

#endif

/**
 * [day02] add function, including [uninitialized_fill_n], [unchecked_uninit_fill_n]
 */