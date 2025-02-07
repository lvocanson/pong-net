#pragma once
#include "EnumOperators.h"

template <IsEnum T>
constexpr T& operator|=(T& lhs, T rhs)
{
    using UType = std::underlying_type_t<T>;
    lhs = static_cast<T>(static_cast<UType>(lhs) | static_cast<UType>(rhs));
    return lhs;
}

template <IsEnum T>
constexpr T& operator&=(T& lhs, T rhs)
{
    using UType = std::underlying_type_t<T>;
    lhs = static_cast<T>(static_cast<UType>(lhs) & static_cast<UType>(rhs));
    return lhs;
}

template <IsEnum T>
constexpr T& operator^=(T& lhs, T rhs)
{
    using UType = std::underlying_type_t<T>;
    lhs = static_cast<T>(static_cast<UType>(lhs) ^ static_cast<UType>(rhs));
    return lhs;
}

template <IsEnum T>
constexpr T operator|(T lhs, T rhs)
{
    return lhs |= rhs;
}

template <IsEnum T>
constexpr T operator&(T lhs, T rhs)
{
    using UType = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<UType>(lhs) & static_cast<UType>(rhs));
}

template <IsEnum T>
constexpr T operator^(T lhs, T rhs)
{
    using UType = std::underlying_type_t<T>;
    return static_cast<T>(static_cast<UType>(lhs) ^ static_cast<UType>(rhs));
}

template <IsEnum T>
constexpr T operator~(T value)
{
    using UType = std::underlying_type_t<T>;
    return static_cast<T>(~static_cast<UType>(value));
}

template <IsEnum T>
constexpr T& operator++(T& value)
{
    using UType = std::underlying_type_t<T>;
    value = static_cast<T>(static_cast<UType>(value) + 1);
    return value;
}

template <IsEnum T>
constexpr T operator++(T& value, int)
{
    T oldValue = value;
    ++value;
    return oldValue;
}

template <IsEnum T>
constexpr T& operator--(T& value)
{
    using UType = std::underlying_type_t<T>;
    value = static_cast<T>(static_cast<UType>(value) - 1);
    return value;
}

template <IsEnum T>
constexpr T operator--(T& value, int)
{
    T oldValue = value;
    --value;
    return oldValue;
}

template <IsEnum T>
constexpr T& operator+=(T& lhs, std::integral auto rhs)
{
    using UType = std::underlying_type_t<T>;
    lhs = static_cast<T>(static_cast<UType>(lhs) + rhs);
    return lhs;
}

template <IsEnum T>
constexpr T& operator-=(T& lhs, std::integral auto rhs)
{
    using UType = std::underlying_type_t<T>;
    lhs = static_cast<T>(static_cast<UType>(lhs) - rhs);
    return lhs;
}

template <IsEnum T>
constexpr T operator+(T lhs, std::integral auto rhs)
{
    lhs += rhs;
    return lhs;
}

template <IsEnum T>
constexpr T operator-(T lhs, std::integral auto rhs)
{
    lhs -= rhs;
    return lhs;
}

template <IsEnum T>
constexpr std::underlying_type_t<T> operator-(T lhs, T rhs)
{
    using UType = std::underlying_type_t<T>;
    return static_cast<UType>(lhs) - static_cast<UType>(rhs);
}
