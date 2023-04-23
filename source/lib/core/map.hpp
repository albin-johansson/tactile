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

#include "common/macros.hpp"
#include "common/numeric.hpp"
#include "common/type/func.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/ptr.hpp"
#include "common/type/uuid.hpp"
#include "core/context/context.hpp"
#include "core/layer/layer_type.hpp"
#include "core/tile/tile_extent.hpp"
#include "core/tile/tile_pos.hpp"

namespace tactile {

///
/// Maps a built from multiple layers stacked on top of each other. An invisible group
/// layer (the "root" layer) manages the layers in every map. This root layer is always
/// present, even in empty maps. However, this root layer is never included in save files,
/// it is merely an implementation detail to simplify the map code.
///
/// Each map has a collection of associated tilesets, providing the tiles to be used in
/// tile layers. Tilesets may be shared with other maps in the case of external tilesets.
/// See the Tileset class for more information.
///
/// Every map uses its own tile format specification, which can be used to control aspects
/// such as whether tile layer data is compressed. By default, maps do not use any
/// compression along with plain text encoding.

}  // namespace tactile
