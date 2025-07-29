// Copyright (C) 2025 Albin Johansson

export module tactile.core.util:hash;

import tactile.core.ext.std;
import tactile.core.prelude;

export namespace tactile {

/// Hashes a value and combines the result with an existing hash value.
///
/// See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0814r2.pdf
template <typename T>
constexpr void hash_combine(usize& seed, const T& value) noexcept
{
  const auto value_hash = std::hash<T> {}(value);
  seed ^= value_hash + 0x9E3779B9uz + (seed << 6uz) + (seed >> 2uz);
}

/// Hashes a generic collection of values.
[[nodiscard]]
constexpr auto hash_combine(const auto&... args) noexcept -> usize
{
  auto seed = 0uz;
  (hash_combine(seed, args), ...);
  return seed;
}

}  // namespace tactile
