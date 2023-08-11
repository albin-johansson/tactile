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

#include "tactile/core/common/error.hpp"

namespace tactile {

template <typename T>
[[nodiscard]] auto find_in(T& map, const auto& key) -> typename T::mapped_type*
{
  if (const auto iter = map.find(key); iter != map.end()) {
    return &iter->second;
  }

  return nullptr;
}

template <typename T>
[[nodiscard]] auto find_in(const T& map, const auto& key) -> const
    typename T::mapped_type*
{
  if (const auto iter = map.find(key); iter != map.end()) {
    return &iter->second;
  }

  return nullptr;
}

template <typename T>
[[nodiscard]] auto lookup_in(T& map, const auto& key) -> typename T::mapped_type&
{
  if (auto* value = find_in(map, key)) {
    return *value;
  }

  throw Error {"Did not find element in map"};
}

template <typename T>
[[nodiscard]] auto lookup_in(const T& map, const auto& key) -> const
    typename T::mapped_type&
{
  if (const auto* value = find_in(map, key)) {
    return *value;
  }

  throw Error {"Did not find element in map"};
}

template <typename T>
auto erase_in(T& map, const auto& key) -> typename T::mapped_type
{
  if (const auto iter = map.find(key); iter != map.end()) {
    auto value = iter->second;
    map.erase(iter);
    return value;
  }

  throw Error {"Did not find element to remove"};
}

template <typename T>
[[nodiscard]] auto exists_in(const T& map, const auto& key) -> bool
{
  return map.find(key) != map.end();
}

}  // namespace tactile
