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

#include <algorithm>   // find_if
#include <concepts>    // equality_comparable_with
#include <functional>  // less
#include <map>         // map

#include <EASTL/algorithm.h>
#include <EASTL/hash_map.h>
#include <EASTL/vector_map.h>

#include "core/container/eastl_new.hpp"
#include "misc/panic.hpp"

namespace tactile {

// clang-format off

template <typename T>
concept MapType = requires (T t) {
  typename T::key_type;
  typename T::mapped_type;
  { t.begin() };
  { t.end() };
};

// clang-format on

template <typename K, typename V>
using TreeMap = std::map<K, V, std::less<>>;

template <typename K, typename V>
using HashMap = eastl::hash_map<K, V>;

template <typename K, typename V>
using VectorMap = eastl::vector_map<K, V>;

template <MapType Map, std::equality_comparable_with<typename Map::key_type> T>
[[nodiscard]] auto lookup_in(Map& map, T&& key) -> typename Map::mapped_type&
{
  const auto iter = std::find_if(map.begin(), map.end(), [&](const auto& pair) {
    return pair.first == key;
  });

  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

template <MapType Map, std::equality_comparable_with<typename Map::key_type> T>
[[nodiscard]] auto lookup_in(const Map& map, T&& key) -> const typename Map::mapped_type&
{
  const auto iter = std::find_if(map.begin(), map.end(), [&](const auto& pair) {
    return pair.first == key;
  });

  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

}  // namespace tactile