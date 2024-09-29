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
#include "walgorithm.hpp"
#include "wconstruct.hpp"

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

template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
    return wstl::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
    auto cur = result;
    try
    {
        for(; first != last; ++first, ++cur) {
            wstl::construct(&*cur, *first);
        }
    }
    catch(...)
    {
        for(; result != cur; --cur) {
            wstl::destroy(&*cur);
        }
    }
    return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
    return wstl::unchecked_uninit_copy(first, last, result,
                                std::is_trivially_copy_assignable<
                                typename iterator_traits<ForwardIter>::
                                value_type>{});
}


template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type)
{
    return wstl::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter
unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type)
{
    ForwardIter cur = result;
    try
    {
        for(; first != last; ++first, ++cur) {
            wstl::construct(&*cur, wstl::move(*first));
        }
    }
    catch(...)
    {
        wstl::destory(result, cur);
    }
    return cur;
}


template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last,
                                ForwardIter result)
{
    return wstl::unchecked_uninit_move(first, result,
                                        std::is_trivially_move_assignable<
                                        typename iterator_traits<InputIter>::
                                        value_type>{});
}


}

#endif

/**
 * [day02]: add function, including [uninitialized_fill_n], [unchecked_uninit_fill_n]
 * [day04]: add unchecked_uninit_move(), uninitialized_move()
 */