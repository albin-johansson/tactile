/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <functional>  // hash

#include "core/vocabulary.hpp"

namespace tactile {

template <typename T>
void hash_combine(usize& seed, const T& val)
{
  // https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0814r0.pdf
  seed ^= std::hash<T> {}(val) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
}

[[nodiscard]] auto hash_combine(const auto&... args) -> usize
{
  usize seed = 0;
  (hash_combine(seed, args), ...);
  return seed;
}

}  // namespace tactile

#define TACTILE_IMPLEMENT_HASH(Type, ...)                                \
  template <>                                                            \
  struct std::hash<Type> final {                                         \
    [[nodiscard]] auto operator()(const Type& t) const -> tactile::usize \
    {                                                                    \
      return tactile::hash_combine(__VA_ARGS__);                         \
    }                                                                    \
  }
