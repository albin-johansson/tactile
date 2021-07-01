#pragma once

#include <type_traits>  // is_pointer_v

namespace Tactile {

template <typename T>
  requires std::is_pointer_v<T>
using NotNull = T;

}  // namespace Tactile
