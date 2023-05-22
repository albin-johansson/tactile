/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <functional>     // hash, equal_to
#include <type_traits>    // true_type
#include <unordered_map>  // unordered_map

#include "common/primitives.hpp"
#include "common/type/result.hpp"
#include "common/type/string.hpp"

namespace tactile {

struct StringHash final {
  using hash_type = std::hash<StringView>;
  using is_transparent = std::true_type;

  [[nodiscard]] auto operator()(const char* str) const -> usize
  {
    return hash_type {}(str);
  }

  [[nodiscard]] auto operator()(StringView str) const -> usize
  {
    return hash_type {}(str);
  }

  [[nodiscard]] auto operator()(const String& str) const -> usize
  {
    return hash_type {}(str);
  }
};

/// Variant of hash map optimized for string keys, using heterogeneous lookups.
template <typename T>
using StringMap = std::unordered_map<String, T, StringHash, std::equal_to<>>;

template <typename T>
auto erase_from(StringMap<T>& map, StringView key) -> Result
{
  if (const auto iter = map.find(key); iter != map.end()) {
    map.erase(iter);
    return success;
  }

  return failure;
}

}  // namespace tactile