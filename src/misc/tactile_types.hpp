#pragma once

#include <type_traits>

namespace tactile {

using tile_id = int;
inline constexpr tile_id empty = 0;

template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
using owner = T;

using czstring = const char*;

}  // namespace tactile
