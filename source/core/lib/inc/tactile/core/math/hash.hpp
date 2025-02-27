// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // hash

#include "tactile/core/basic/primitives.hpp"

namespace tactile {

/// Hashes a value and combines the result with an existing hash value.
///
/// See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0814r2.pdf
template <typename T>
constexpr void hash_combine(usize& seed, const T& value) noexcept
{
  const auto value_hash = std::hash<T> {}(value);
  seed ^= value_hash + usize {0x9E3779B9} + (seed << usize {6}) + (seed >> usize {2});
}

/// Hashes a generic collection of values.
[[nodiscard]]
constexpr auto hash_combine(const auto&... args) noexcept -> usize
{
  usize seed = 0;
  (hash_combine(seed, args), ...);
  return seed;
}

}  // namespace tactile
