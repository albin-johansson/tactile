// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>     // size_t
#include <functional>  // hash

#include "tactile/base/prelude.hpp"

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
void hash_combine(std::size_t& seed, const T& value) noexcept
{
  const auto value_hash = std::hash<T> {}(value);
  seed ^= value_hash + std::size_t {0x9E3779B9} + (seed << std::size_t {6}) +
          (seed >> std::size_t {2});
}

/**
 * Hashes a collection of values.
 *
 * \param args The values that will be hashed.
 *
 * \return A hash of the values.
 */
[[nodiscard]]
auto hash_combine(const auto&... args) noexcept -> std::size_t
{
  std::size_t seed = 0;
  (hash_combine(seed, args), ...);
  return seed;
}

}  // namespace tactile
