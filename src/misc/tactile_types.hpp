#pragma once

#include <gsl/gsl>
#include <type_traits>

namespace tactile {

using tile_id = int;

inline constexpr tile_id empty = 0;

template <typename T>
using owner = gsl::owner<T>;

template <typename T>
using not_null = gsl::not_null<T>;

using czstring = gsl::czstring<>;

}  // namespace tactile
