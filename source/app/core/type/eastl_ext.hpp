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

#include <EASTL/string.h>
#include <EASTL/string_view.h>

namespace eastl {

template <typename T, typename Allocator>
inline auto operator<(const basic_string<T, Allocator>& a, const basic_string_view<T>& b)
    -> bool
{
  return basic_string<T, Allocator>::compare(a.begin(), a.end(), b.begin(), b.end()) < 0;
}

template <typename T, typename Allocator>
inline auto operator<(const basic_string_view<T>& a, const basic_string<T, Allocator>& b)
    -> bool
{
  return basic_string_view<T>::compare(a.begin(), a.end(), b.begin(), b.end()) < 0;
}

}  // namespace eastl
