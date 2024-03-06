// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // hash

#include "tactile/base/int.hpp"

namespace tactile {

/**
 * Hashes a value and combines the result with an existing hash value.
 *
 * \tparam T Any hashable type.
 *
 * \param[out] seed  The hash that will be updated.
 * \param      value The value that will be hashed.
 *
 * \see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0814r0.pdf
 */
template <typename T>
void hash_combine(usize& seed, const T& value) noexcept
{
  const auto value_hash = std::hash<T> {}(value);
  seed ^= value_hash + usize {0x9E3779B9} + (seed << usize {6}) + (seed >> usize {2});
}

/**
 * Hashes a collection of values.
 *
 * \param args The values that will be hashed.
 *
 * \return A hash of the values.
 */
[[nodiscard]]
auto hash_combine(const auto&... args) noexcept -> usize
{
  usize seed = 0;
  (hash_combine(seed, args), ...);
  return seed;
}

}  // namespace tactile
