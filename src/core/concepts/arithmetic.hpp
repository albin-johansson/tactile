#pragma once

#include <concepts>     // same_as
#include <type_traits>  // is_arithmetic_v

namespace tactile {

// clang-format off

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T> && !std::same_as<T, bool>;

// clang-format on

}  // namespace tactile
