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

#include "common/debug/assert.hpp"
#include "common/primitives.hpp"
#include "common/type/vector.hpp"

namespace tactile {

template <typename T>
void insert_at(Vector<T>& vec, const usize index, T value)
{
  TACTILE_ASSERT(index <= vec.size());

  if (index < vec.size()) {
    vec.insert(vec.begin() + static_cast<ssize>(index), std::move(value));
  }
  else if (index == vec.size()) {
    vec.push_back(std::move(value));
  }
}

template <typename T>
void erase_at(Vector<T>& vec, const usize index)
{
  TACTILE_ASSERT(index < vec.size());

  if (index < vec.size()) {
    vec.erase(vec.begin() + static_cast<ssize>(index));
  }
}

template <typename T>
[[nodiscard]] auto contains_value(const Vector<T>& vec, const T& value) -> bool
{
  return std::ranges::find(vec, value) != vec.end();
}

}  // namespace tactile