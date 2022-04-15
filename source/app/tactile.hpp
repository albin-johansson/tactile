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

#include <cstddef>        // size_t
#include <cstdint>        // int{}_t, uint{}_t

#define TACTILE_DEFAULT_COPY(Class) \
  Class(const Class&) = default;    \
  auto operator=(const Class&)->Class& = default;

#define TACTILE_DELETE_COPY(Class) \
  Class(const Class&) = delete;    \
  auto operator=(const Class&)->Class& = delete;

#define TACTILE_DEFAULT_MOVE(Class)  \
  Class(Class&&) noexcept = default; \
  auto operator=(Class&&) noexcept->Class& = default;

#define TACTILE_DELETE_MOVE(Class)  \
  Class(Class&&) noexcept = delete; \
  auto operator=(Class&&) noexcept->Class& = delete;

namespace tactile {

using uchar = unsigned char;
using uint = unsigned int;
using ulonglong = unsigned long long int;
using usize = std::size_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using ContextID = int32;    ///< Identifier for attribute contexts (not persistent).
using ComponentID = int32;  ///< Identifier for component definitions (not persistent).

using MapID = int32;
using LayerID = int32;
using ObjectID = int32;
using TilesetID = int32;
using TileID = int32;

constexpr TileID empty_tile = 0;

}  // namespace tactile
