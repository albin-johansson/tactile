#pragma once

#include <concepts>  // convertible_to

namespace tactile {

// clang-format off

template <typename T>
concept IsNegatable = requires (T t)
{
  { !t } -> std::convertible_to<T>;
};

// clang-format on

}  // namespace tactile
