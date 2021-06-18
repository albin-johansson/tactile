#pragma once

#include <type_traits>  // is_enum_v

namespace tactile {

template <typename T>
concept IsEnum = std::is_enum_v<T>;

}  // namespace tactile
