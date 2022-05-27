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

#include "core/common/ints.hpp"
#include "core/common/uuid.hpp"

namespace tactile {

// TODO remove most of these identifiers

using ContextID = UUID;
using ComponentID = UUID;

using LayerID = int32;
using ObjectID = int32;
using TilesetID = int32;
using TileID = int32;

inline constexpr TileID empty_tile = 0;

}  // namespace tactile