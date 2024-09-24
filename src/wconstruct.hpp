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

}

#endif


/**
 * [day01]: use ::new to implement a simply function [void construct(Ty* ptr)]
 *          that only call construct of the template class, that is Ty 
 */