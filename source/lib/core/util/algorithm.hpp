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

#include <algorithm>  // find_if
#include <concepts>   // predicate
#include <utility>    // forward

#include "core/debug/panic.hpp"
#include "common/type/vec.hpp"

namespace tactile {

/// Returns the first element in a vector that satisfies the given predicate.
template <typename T, std::predicate<const T&> P>
[[nodiscard]] auto first_in(Vec<T>& vec, P&& pred) -> T&
{
  const auto iter = std::find_if(vec.begin(), vec.end(), std::forward<P>(pred));
  if (iter != vec.end()) {
    return *iter;
  }
  else {
    throw TactileError {"No element matched predicate!"};
  }
}

/// Returns the first element in a vector that satisfies the given predicate.
template <typename T, std::predicate<const T&> P>
[[nodiscard]] auto first_in(const Vec<T>& vec, P&& pred) -> const T&
{
  const auto iter = std::find_if(vec.begin(), vec.end(), std::forward<P>(pred));
  if (iter != vec.end()) {
    return *iter;
  }
  else {
    throw TactileError {"No element matched predicate!"};
  }
}

}  // namespace tactile
