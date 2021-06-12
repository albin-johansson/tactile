#pragma once

#include <type_traits>  // is_pointer_v

namespace tactile {

template <typename T>
concept IsPointer = std::is_pointer_v<T>;

}  // namespace tactile