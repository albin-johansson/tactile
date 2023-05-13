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

#include <cstddef>  // size_t, ptrdiff_t, uintptr_t
#include <cstdint>  // int{}_t, uint{}_t

namespace tactile {

using uchar = unsigned char;
using uint = unsigned int;
using ulonglong = unsigned long long int;

using usize = std::size_t;
using ssize = std::ptrdiff_t;
using uintptr = std::uintptr_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using float32 = float;
using float64 = double;

static_assert(sizeof(float32) == 4);
static_assert(sizeof(float64) == 8);

using LayerID = int32;
using ObjectID = int32;
using TileIndex = int32;  ///< For local tile identifiers.
using TileID = int32;     ///< For global tile identifiers.

inline constexpr TileID kEmptyTile = 0;

}  // namespace tactile