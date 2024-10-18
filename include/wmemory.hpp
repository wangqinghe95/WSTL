#ifndef WMEMORY_HPP__
#define WMEMORY_HPP__

namespace wstl
{

template <class Tp>
constexpr Tp* address_of(Tp& value) noexcept
{
    return &value;
}

}

#endif