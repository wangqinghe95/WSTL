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

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::true_type)
{
    wstl::fill(first, last, value);
}

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::false_type)
{
    auto cur = first;
    try
    {
        for(; cur != last; ++cur) {
            wstl::construct(&*cur, value);
        }
    }
    catch(...)
    {
        for(; first != cur; ++first) {
            wstl::destroy(&*first);
        }
    }
    
}

template <class ForwardIter, class T>
void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value)
{
    wstl::unchecked_uninit_fill(first, last, value,
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
        wstl::destroy(result, cur);
    }
    return cur;
}


template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter first, InputIter last,
                                ForwardIter result)
{
    return wstl::unchecked_uninit_move(first, last ,result,
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