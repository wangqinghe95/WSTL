#ifndef UTILS_HPP__
#define UTILS_HPP__

/**
 * @file utils.hpp
 * @brief Responsible for the implementation of some general functions
 * @author wangqinghe
 * @date 9/24/2021
 * @version 1.0
 * 
 * Copyright © Luis. All rights reserved.
 */

#include <iostream>
#include <sstream>
#include <string>

#include "logger.hpp"

namespace wstl
{

template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}

}

#endif

/**
 * [day01]: use C++11 new feature to implement a LOG micro
 */