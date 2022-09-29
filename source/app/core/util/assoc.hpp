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

#include <concepts>  // equality_comparable_with

#include <EASTL/utility.h>

#include "core/type/hash_map.hpp"
#include "core/type/tree_map.hpp"
#include "misc/panic.hpp"

namespace tactile {

template <typename T, typename U>
concept TransparentKey = std::equality_comparable_with<T, U>;

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto find_in(HashMap<K, V>& map, T&& key) ->
    typename HashMap<K, V>::iterator
{
  return map.find_as(eastl::forward<T>(key));
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto find_in(const HashMap<K, V>& map, T&& key) ->
    typename HashMap<K, V>::const_iterator
{
  return map.find_as(eastl::forward<T>(key));
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto find_in(TreeMap<K, V>& map, T&& key) ->
    typename TreeMap<K, V>::iterator
{
  return map.find_as(eastl::forward<T>(key), eastl::equal_to_2<K, std::decay_t<T>> {});
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto find_in(const TreeMap<K, V>& map, T&& key) ->
    typename TreeMap<K, V>::const_iterator
{
  return map.find_as(eastl::forward<T>(key), eastl::equal_to_2<K, std::decay_t<T>> {});
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto lookup_in(HashMap<K, V>& map, T&& key) -> V&
{
  const auto iter = find_in(map, eastl::forward<T>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto lookup_in(const HashMap<K, V>& map, T&& key) -> const V&
{
  const auto iter = find_in(map, eastl::forward<T>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto lookup_in(TreeMap<K, V>& map, T&& key) -> V&
{
  const auto iter = find_in(map, eastl::forward<T>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto lookup_in(const TreeMap<K, V>& map, T&& key) -> const V&
{
  const auto iter = find_in(map, eastl::forward<T>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto has_key(const HashMap<K, V>& map, T&& key) -> bool
{
  return find_in(map, eastl::forward<T>(key)) != map.end();
}

template <typename K, typename V, TransparentKey<K> T>
[[nodiscard]] auto has_key(const TreeMap<K, V>& map, T&& key) -> bool
{
  return find_in(map, eastl::forward<T>(key)) != map.end();
}

}  // namespace tactile