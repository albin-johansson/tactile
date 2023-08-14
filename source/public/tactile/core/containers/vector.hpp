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

#include <algorithm>  // find
#include <utility>    // move
#include <vector>     // vector

#include "tactile/core/common/prelude.hpp"

namespace tactile {

template <typename T>
using Vector = std::vector<T>;

/** Inserts or appends an element to a vector. */
template <typename T>
auto insert_at(Vector<T>& vec, const usize index, T value) -> bool
{
  if (index < vec.size()) {
    vec.insert(vec.begin() + static_cast<ssize>(index), std::move(value));
    return true;
  }
  else if (index == vec.size()) {
    vec.push_back(std::move(value));
    return true;
  }

  return false;
}

template <typename T>
auto erase_at(Vector<T>& vec, const usize index) -> bool
{
  if (index < vec.size()) {
    vec.erase(vec.begin() + static_cast<ssize>(index));
    return true;
  }

  return false;
}

template <typename T, typename U>
[[nodiscard]] auto exists_in(const Vector<T>& vec, const U& value) -> bool
{
  return std::ranges::find(vec, value) != vec.end();
}

}  // namespace tactile
