// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <concepts>
#include <type_traits>

#include "tactile/core/primitives.hpp"

namespace tactile {

template <typename T>
concept UnsignedInteger =
    std::same_as<T, unsigned char> || std::same_as<T, unsigned short> ||
    std::same_as<T, unsigned int> || std::same_as<T, unsigned long> ||
    std::same_as<T, unsigned long long>;

template <typename T>
concept SignedInteger = std::same_as<T, signed char> || std::same_as<T, signed short> ||
                        std::same_as<T, signed int> || std::same_as<T, signed long> ||
                        std::same_as<T, signed long long>;

template <typename T>
concept Integer = UnsignedInteger<T> || SignedInteger<T>;

template <typename T>
concept Number = Integer<T> || std::floating_point<T>;

template <typename T, typename... Args>
concept NothrowInvocable = std::is_nothrow_invocable_v<T, Args...>;

}  // namespace tactile
