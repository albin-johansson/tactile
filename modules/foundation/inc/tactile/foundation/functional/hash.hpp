// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <functional>  // hash

#include "tactile/foundation/prelude.hpp"

namespace tactile {

template <typename T>
void hash_combine(usize& seed, const T& value) noexcept
{
  using namespace int_literals;

  // https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0814r0.pdf
  const auto value_hash = std::hash<T> {}(value);
  seed ^= value_hash + 0x9E3779B9_uz + (seed << 6_uz) + (seed >> 2_uz);
}

[[nodiscard]] auto hash_combine(const auto&... args) noexcept -> usize
{
  usize seed = 0;
  (hash_combine(seed, args), ...);
  return seed;
}

}  // namespace tactile

#define TACTILE_IMPLEMENT_HASH(Type, ...)                           \
  template <>                                                       \
  struct std::hash<Type> final {                                    \
    [[nodiscard]] auto operator()(const Type& value) const noexcept \
        -> tactile::usize                                           \
    {                                                               \
      return tactile::hash_combine(__VA_ARGS__);                    \
    }                                                               \
  }
