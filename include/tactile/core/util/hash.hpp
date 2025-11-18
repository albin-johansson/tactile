// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <concepts>
#include <cstddef>
#include <functional>
#include <type_traits>

#include "tactile/core/primitives.hpp"

namespace tactile {
namespace hash_detail {

template <typename T>
concept Hashable = requires(const T& value) {
  { std::hash<T> {}(value) } -> std::convertible_to<usize>;
} && std::is_nothrow_invocable_v<std::hash<T>, const T&>;

template <Hashable T>
void hash_combine_impl(usize& seed, const T& val) noexcept
{
  // https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0814r0.pdf
  seed ^= std::hash<T> {}(val) + 0x9E3779B9uz + (seed << 6uz) + (seed >> 2uz);
}

}  // namespace hash_detail

/// Calculates a hash of the given arguments.
///
/// \param[in] args
///            The arguments to hash.
/// \returns   A hash of the arguments.
auto hash_combine(const auto&... args) noexcept -> usize
{
  usize seed {0};
  (hash_detail::hash_combine_impl(seed, args), ...);
  return seed;
}

}  // namespace tactile

/// Specializes `std::hash` for a given type.
///
/// The variadic macro arguments should specify the members to include in the hash calculation
/// by querying an object called `value`. This macro uses `hash_combine` to calculate the hash.
///
/// ```C++
/// TACTILE_IMPL_HASH(::lib::MyType, value.foo, value.bar);
/// ```
///
/// \param[in] T The fully qualified name of the type to make hashable.
#define TACTILE_IMPL_HASH(T, ...)                                    \
  template <>                                                        \
  struct ::std::hash<T> final                                        \
  {                                                                  \
    static auto operator()(const T& value) noexcept -> ::std::size_t \
    {                                                                \
      return ::tactile::hash_combine(__VA_ARGS__);                   \
    }                                                                \
  }
