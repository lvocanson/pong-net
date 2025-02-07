#pragma once
#include <concepts>
#include <type_traits>

/**
 * @brief Concept to check if a type is an enumeration.
 * @tparam T The type to check.
 */
template <typename T>
concept IsEnum = std::is_enum_v<T>;

template <IsEnum T> constexpr T& operator|=(T& lhs, T rhs);
template <IsEnum T> constexpr T& operator&=(T& lhs, T rhs);
template <IsEnum T> constexpr T& operator^=(T& lhs, T rhs);
template <IsEnum T> constexpr T operator|(T lhs, T rhs);
template <IsEnum T> constexpr T operator&(T lhs, T rhs);
template <IsEnum T> constexpr T operator^(T lhs, T rhs);
template <IsEnum T> constexpr T operator~(T value);
template <IsEnum T> constexpr T& operator++(T& value);
template <IsEnum T> constexpr T operator++(T& value, int);
template <IsEnum T> constexpr T& operator--(T& value);
template <IsEnum T> constexpr T operator--(T& value, int);
template <IsEnum T> constexpr T& operator+=(T& lhs, std::integral auto rhs);
template <IsEnum T> constexpr T& operator-=(T& lhs, std::integral auto rhs);
template <IsEnum T> constexpr T operator+(T lhs, std::integral auto rhs);
template <IsEnum T> constexpr T operator-(T lhs, std::integral auto rhs);
template <IsEnum T> constexpr std::underlying_type_t<T> operator-(T lhs, T rhs);

#include "EnumOperators.inl"
