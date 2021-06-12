#pragma once

#include <concepts>  // convertible_to

namespace tactile {

// clang-format off

template <typename T>
concept Ordered = requires (T t)
{
  { t < t } -> std::convertible_to<bool>;
};

// clang-format on

}  // namespace tactile
