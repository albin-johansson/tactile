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

#include <vector>  // vector

#include <centurion/math.hpp>
#include <entt/entity/entity.hpp>

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/region.hpp"
#include "core/tile_pos.hpp"

namespace tactile::comp {

/// \addtogroup components
/// \{

/**
 * Context component that keeps track of the tileset state of a single map.
 */
struct TilesetContext final
{
  TileID next_tile_id{};  ///< Next available global tile ID.
  HashMap<TileID, entt::entity> tile_to_tileset;
};

/**
 * Provides general information about a tileset.
 *
 * \todo Use ivec2 for tile size.
 */
struct Tileset final
{
  Vector2i tile_size{};  ///< Logical size of tiles.
  int32 row_count{};     ///< Amount of tile rows.
  int32 column_count{};  ///< Amount of tile columns.

  [[nodiscard]] auto tile_count() const noexcept -> int32
  {
    return row_count * column_count;
  }
};

/**
 * Component used in map registries to keep track of all added tilesets.
 */
struct TilesetRef final
{
  UUID source_tileset{};  ///< The source tileset document.
  TileID first_id{};      ///< First associated global tile ID.
  TileID last_id{};       ///< Last associated global tile ID (inclusive).
  bool embedded : 1 {};   ///< Is this tileset embedded in the map?
};

/**
 * Provides information about the size of a single tile in a tileset for rendering.
 */
struct UvTileSize final
{
  Vector2f size{};
};

/**
 * Component that provides meta-information about tileset tiles.
 *
 * Tiles with aspects such as animations, properties or associated objects will feature
 * this component.
 */
struct MetaTile final
{
  TileIndex index{};                  ///< Local index in the tileset.
  std::vector<entt::entity> objects;  ///< Associated object entities.
};

/**
 * Context component that contains cached information about a single tileset.
 */
struct TilesetCache final
{
  HashMap<TileIndex, cen::irect> source_rects;  ///< Tileset source rectangles.
  HashMap<TileIndex, entt::entity> tiles;       ///< Additional tile info.

  /// Frame-by-frame cache that maps tiles to the tile that should be rendered.
  mutable HashMap<TileIndex, TileIndex> display_tiles;
};

/**
 * Represents the tile selection in a tileset.
 */
struct TilesetSelection final
{
  Maybe<Region> region;  ///< The currently selected region, if any.
};

/// \} End of group components

}  // namespace tactile::comp
