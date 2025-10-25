// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <concepts>
#include <limits>
#include <type_traits>
#include <utility>

#include "tactile/core/primitives.hpp"

namespace tactile {

/// Indicates whether `T` is without CV-qualifiers.
template <typename T>
concept CvUnqualified = !std::is_const_v<T> && !std::is_volatile_v<T>;

/// Indicates whether `T` is an unsigned integer type.
template <typename T>
concept UnsignedInteger =
    std::same_as<T, unsigned char> || std::same_as<T, unsigned short> ||
    std::same_as<T, unsigned int> || std::same_as<T, unsigned long> ||
    std::same_as<T, unsigned long long>;

/// Indicates whether `T` is a signed integer type.
template <typename T>
concept SignedInteger = std::same_as<T, signed char> || std::same_as<T, signed short> ||
                        std::same_as<T, signed int> || std::same_as<T, signed long> ||
                        std::same_as<T, signed long long>;

/// Indicates whether `T` is an integer type.
template <typename T>
concept Integer = UnsignedInteger<T> || SignedInteger<T>;

/// Indicates whether `T` is a floating-point type.
template <typename T>
concept FloatingPoint = std::floating_point<T> && CvUnqualified<T>;

/// Indicates whether `T` is a numeric type.
template <typename T>
concept Number = Integer<T> || FloatingPoint<T>;

/// Indicates whether `T` is a non-throwing invocable type.
template <typename T, typename... Args>
concept NothrowInvocable = std::is_nothrow_invocable_v<T, Args...>;

/// Indicates whether all `From` values are representable as `To` values.
template <typename From, typename To>
concept TriviallyConvertible =
    Number<From> && Number<To> &&
    std::numeric_limits<From>::digits <= std::numeric_limits<To>::digits &&
    ((Integer<From> && Integer<To> && SignedInteger<From> == SignedInteger<To>) ||
     (Integer<From> && FloatingPoint<To>) || (UnsignedInteger<From> && Integer<To>) ||
     (FloatingPoint<From> && FloatingPoint<To>));

}  // namespace tactile
