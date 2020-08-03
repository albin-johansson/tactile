#pragma once

#include <type_traits>

namespace tactile {

template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
using owner = T;

using czstring = const char*;

}  // namespace tactile
