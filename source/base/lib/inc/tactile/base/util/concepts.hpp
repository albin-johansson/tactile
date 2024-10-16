// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <type_traits>  // is_arithmetic_v, is_invocable_r_v

namespace tactile {

/**
 * Describes numeric types, i.e., types that primarily represent numeric values.
 *
 * \tparam T The type to check.
 */
template <typename T>
concept ArithmeticType = std::is_arithmetic_v<T>;

/**
 * Represents a callable type with a given function signature, including the
 * return type.
 *
 * \tparam T    The type to check.
 * \tparam Ret  The required return type.
 * \tparam Args The parameter types.
 */
template <typename T, typename Ret, typename... Args>
concept InvocableType = std::is_invocable_r_v<Ret, T, Args...>;

}  // namespace tactile
