#ifndef W_CONSTRUCT_HPP__
#define W_CONSTRUCT_HPP__

/**
 * @file wconstruct.hpp
 * @brief Responsible for the construction and desconstruction of the object
 * @author wangqinghe
 * @date 9/24/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

#include <new>
#include "witerator.hpp"
#include "utils.hpp"

namespace wstl
{

// construct Ty with no parameters
template <class Ty>
void construct(Ty* ptr)
{
    /**
     * Q: ::new is a global operator, it not only applies for memory
     *      but also calls for a constructor, why will it only call
     *      the constructor here, not playing the role of the request memory
     */
    ::new ((void*)ptr) Ty();
}

template <class Ty1, class Ty2>
void construct (Ty1* ptr, const Ty2& value)
{
    ::new ((void*)ptr) Ty2(value);
}

template <class Ty, class... Args>
void construct(Ty* ptr, Args&&... args)
{
    ::new ((void*)ptr) Ty(wstl::forward<Args>(args)...);
}

// destory

template <class Ty>
void destroy_one(Ty*, std::true_type){}

template <class Ty>
void destroy_one(Ty* pointer, std::false_type)
{
    if(pointer != nullptr) {
        pointer->~Ty();
    }
}

template <class ForwardIter>
void destory_cat(ForwardIter, ForwardIter, std::true_type){}

template <class Forwarditer>
void destory_cat(Forwarditer first, Forwarditer last, std::false_type)
{
    for(; first != last; ++first) {
        destroy(&*first);
    }
}

template <class Ty>
void destroy(Ty* pointer)
{
    destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
    destory_cat(first, last, std::is_trivially_destructible<
            typename iterator_traits<ForwardIter>::value_type>{});
}


}

#endif

/**
 * [day01]: use ::new to implement a simply function [void construct(Ty* ptr)]
 *          that only call construct of the template class, that is Ty 
 */