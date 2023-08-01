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

#include "tileset_system.hpp"

#include "common/util/lookup.hpp"
#include "core/debug/assert.hpp"
#include "core/tiles/tile_matrix.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tilesets/attached_tileset_ops.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile::sys {

auto get_tile_appearance(const Registry& registry,
                         const Entity tileset_entity,
                         const TileIndex tile_index) -> TileIndex
{
  const auto& cache = registry.get<TilesetRenderCache>(tileset_entity);

  // Check if the appearance has been computed before
  if (const auto iter = cache.appearance.find(tile_index);
      iter != cache.appearance.end()) {
    return iter->second;
  }

  const auto& tileset = registry.get<Tileset>(tileset_entity);
  const auto tile_entity = lookup_in(tileset.tile_index_map, tile_index);

  if (const auto* animation = registry.try_get<TileAnimation>(tile_entity)) {
    // The tile is animated, so update the cache and return the frame tile index
    const auto& current_frame = animation->frames.at(animation->index);
    cache.appearance[tile_index] = current_frame.tile_index;
    return current_frame.tile_index;
  }

  // If the tile isn't animated it appears as itself
  return tile_index;
}

auto is_valid_tile_identifier(const Registry& registry,
                              const Map& map,
                              const TileID tile_id) -> bool
{
  if (tile_id == kEmptyTile) {
    return true;
  }

  return find_tileset_with_tile(registry, map, tile_id) != kNullEntity;
}

auto convert_tile_id_to_index(const Registry& registry,
                              const Map& map,
                              const TileID tile_id) -> Maybe<TileIndex>
{
  const auto attached_tileset_entity = find_tileset_with_tile(registry, map, tile_id);

  if (attached_tileset_entity != kNullEntity) {
    const auto& attached_tileset = registry.get<AttachedTileset>(attached_tileset_entity);
    return to_tile_index(attached_tileset, tile_id).value();
  }

  return nothing;
}

auto find_tileset_with_tile(const Registry& registry,
                            const Map& map,
                            const TileID tile_id) -> Entity
{
  for (const auto attached_tileset_entity: map.attached_tilesets) {
    const auto& attached_tileset = registry.get<AttachedTileset>(attached_tileset_entity);

    if (is_valid_tile(attached_tileset, tile_id)) {
      return attached_tileset_entity;
    }
  }

  return kNullEntity;
}

}  // namespace tactile::sys
