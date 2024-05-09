// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <type_traits>  // is_arithmetic_v

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Describes numeric types, i.e., types that primarily represent numeric values.
 *
 * \tparam T The type to check.
 */
template <typename T>
concept ArithmeticType = std::is_arithmetic_v<T>;

}  // namespace tactile
