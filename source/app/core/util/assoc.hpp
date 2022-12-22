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
#include <utility>   // forward

#include "debug/panic.hpp"

namespace tactile {

template <typename M, std::equality_comparable_with<typename M::key_type> K>
[[nodiscard]] auto lookup_in(M& map, K&& key) -> typename M::mapped_type&
{
  const auto iter = map.find(std::forward<K>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

template <typename M, std::equality_comparable_with<typename M::key_type> K>
[[nodiscard]] auto lookup_in(const M& map, K&& key) -> const typename M::mapped_type&
{
  const auto iter = map.find(std::forward<K>(key));
  if (iter != map.end()) {
    return iter->second;
  }
  else {
    throw TactileError {"Invalid key!"};
  }
}

template <typename M, std::equality_comparable_with<typename M::key_type> K>
[[nodiscard]] auto has_key(const M& map, K&& key) -> bool
{
  return map.find(std::forward<K>(key)) != map.end();
}

}  // namespace tactile