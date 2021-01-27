#pragma once

#include <type_traits>  // is_pointer_v

namespace tactile {

template <typename T>
concept is_pointer = std::is_pointer_v<T>;

template <is_pointer T>
using not_null = T;

}  // namespace tactile
