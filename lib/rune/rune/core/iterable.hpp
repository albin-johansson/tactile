#ifndef RUNE_CORE_ITERABLE_HPP
#define RUNE_CORE_ITERABLE_HPP

#include <algorithm>  // for_each

namespace rune {

/// \addtogroup core
/// \{

// clang-format off

template <typename T, typename Elem>
concept iterable = requires (T obj)
{
  { std::ranges::for_each(obj, [](const Elem& elem) {}) };
};

// clang-format on

/// \} End of group core

}  // namespace rune

#endif  // RUNE_CORE_ITERABLE_HPP
