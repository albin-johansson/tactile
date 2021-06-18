#pragma once

#include <type_traits>  // underlying_type_t

#include "is_enum.hpp"

namespace tactile {

template <IsEnum T>
[[nodiscard]] constexpr auto ToUnderlying(const T value) noexcept
    -> std::underlying_type_t<T>
{
  return static_cast<std::underlying_type_t<T>>(value);
}

}  // namespace tactile
