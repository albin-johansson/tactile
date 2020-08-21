#pragma once

#include <concepts>
#include <gsl/gsl>
#include <type_traits>

namespace tactile {

using tile_id = int;

inline constexpr tile_id empty {0};

// clang-format off

template <typename T> requires std::is_pointer_v<T>
using owner = gsl::owner<T>;

template <typename T> requires std::is_pointer_v<T>
using not_null = gsl::not_null<T>;

// clang-format on

using czstring = gsl::czstring<>;
using u16_czstring = gsl::cu16zstring<>;

}  // namespace tactile
