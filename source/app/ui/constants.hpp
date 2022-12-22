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

#include <limits>  // numeric_limits

namespace tactile {

inline constexpr auto min_float = (std::numeric_limits<float>::min)();
inline constexpr auto max_float = (std::numeric_limits<float>::max)();

inline constexpr auto min_int = (std::numeric_limits<int>::min)();
inline constexpr auto max_int = (std::numeric_limits<int>::max)();

inline constexpr auto max_uint = (std::numeric_limits<unsigned>::max)();

}  // namespace tactile
