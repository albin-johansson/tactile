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

#include <functional>     // less
#include <map>            // map
#include <unordered_map>  // unordered_map

#include "misc/panic.hpp"

namespace tactile {

template <typename K, typename V>
using TreeMap = std::map<K, V, std::less<>>;

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

template <typename K, typename V>
[[nodiscard]] auto lookup_in(TreeMap<K, V>& map, auto&& key) -> V&
{
  if (const auto iter = map.find(key); iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid key!"};
  }
}

template <typename K, typename V>
[[nodiscard]] auto lookup_in(const TreeMap<K, V>& map, auto&& key) -> const V&
{
  if (const auto iter = map.find(key); iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid key!"};
  }
}

template <typename K, typename V>
[[nodiscard]] auto lookup_in(HashMap<K, V>& map, auto&& key) -> V&
{
  if (const auto iter = map.find(key); iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid key!"};
  }
}

template <typename K, typename V>
[[nodiscard]] auto lookup_in(const HashMap<K, V>& map, auto&& key) -> const V&
{
  if (const auto iter = map.find(key); iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid key!"};
  }
}

}  // namespace tactile