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

#include "core/common/identifiers.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/region.hpp"
#include "core/tilesets/tileset.hpp"

namespace tactile::core {

// TODO improve selection

struct TilesetRef final
{
  Shared<Tileset> tileset;          /// The referenced tileset.
  TileID          first_tile{};     /// The first associated tile ID (inclusive).
  TileID          last_tile{};      /// The last associated tile ID (inclusive).
  Maybe<Region>   selection;        /// Selected region.
  bool            embedded : 1 {};  /// Whether the tileset is embedded in the map.

  [[nodiscard]] auto is_single_tile_selected() const -> bool
  {
    if (selection) {
      return selection->end - selection->begin == TilePos{1, 1};
    }
    else {
      return false;
    }
  }
};

}  // namespace tactile::core
