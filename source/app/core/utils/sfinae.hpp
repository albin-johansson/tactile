#pragma once

#include <type_traits>  // enable_if_t, ...

namespace tactile {

template <typename T>
using is_integral = std::enable_if_t<std::is_integral_v<T>, int>;

template <typename T>
using is_floating = std::enable_if_t<std::is_floating_point_v<T>, int>;

template <typename T>
using is_unsigned = std::enable_if_t<std::is_unsigned_v<T>, int>;

template <typename Base, typename Derived>
using is_derived_from = std::enable_if_t<std::is_base_of_v<Base, Derived>, int>;

}  // namespace tactile