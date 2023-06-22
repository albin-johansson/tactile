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

#include "common/debug/assert.hpp"
#include "common/tile_matrix.hpp"
#include "common/util/lookup.hpp"
#include "model/components/context.hpp"
#include "model/components/texture.hpp"
#include "model/components/tile.hpp"
#include "model/components/viewport.hpp"
#include "model/systems/validation_system.hpp"
#include "model/tilesets/attached_tileset_ops.hpp"
#include "model/tilesets/tileset_components.hpp"

namespace tactile::sys {

auto get_tile_appearance(const Model& model,
                         const Entity tileset_entity,
                         const TileIndex tile_index) -> TileIndex
{
  const auto& cache = model.get<TilesetRenderCache>(tileset_entity);

  // Check if the appearance has been computed before
  if (const auto iter = cache.appearance.find(tile_index);
      iter != cache.appearance.end()) {
    return iter->second;
  }

  const auto& tileset = model.get<Tileset>(tileset_entity);
  const auto tile_entity = lookup_in(tileset.tile_index_map, tile_index);

  if (const auto* animation = model.try_get<TileAnimation>(tile_entity)) {
    // The tile is animated, so update the cache and return the frame tile index
    const auto& current_frame = animation->frames.at(animation->index);
    cache.appearance[tile_index] = current_frame.tile_index;
    return current_frame.tile_index;
  }

  // If the tile isn't animated it appears as itself
  return tile_index;
}

void make_tile_animated(Model& model, const Entity tile_entity)
{
  TACTILE_ASSERT(is_tile_entity(model, tile_entity));
  TACTILE_ASSERT(!model.has<TileAnimation>(tile_entity));

  const auto& tile = model.get<Tile>(tile_entity);

  auto& animation = model.add<TileAnimation>(tile_entity);
  animation.index = 0;
  animation.frames.push_back(TileAnimationFrame {tile.index, ms_t {1'000}});
  animation.last_update = Clock::now();
}

auto is_valid_tile_identifier(const Model& model, const Map& map, const TileID tile_id)
    -> bool
{
  if (tile_id == kEmptyTile) {
    return true;
  }

  return find_tileset_with_tile(model, map, tile_id) != kNullEntity;
}

auto is_valid_tile_identifier(const Model& model,
                              const Entity map_entity,
                              const TileID tile_id) -> bool
{
  TACTILE_ASSERT(is_map_entity(model, map_entity));
  const auto& map = model.get<Map>(map_entity);
  return is_valid_tile_identifier(model, map, tile_id);
}

auto convert_tile_id_to_index(const Model& model, const Map& map, const TileID tile_id)
    -> Maybe<TileIndex>
{
  const auto attached_tileset_entity = find_tileset_with_tile(model, map, tile_id);

  if (attached_tileset_entity != kNullEntity) {
    const auto& attached_tileset = model.get<AttachedTileset>(attached_tileset_entity);
    return to_tile_index(attached_tileset, tile_id).value();
  }

  return nothing;
}

auto find_tileset_with_tile(const Model& model, const Map& map, const TileID tile_id)
    -> Entity
{
  for (const auto attached_tileset_entity: map.attached_tilesets) {
    const auto& attached_tileset = model.get<AttachedTileset>(attached_tileset_entity);

    if (is_valid_tile(attached_tileset, tile_id)) {
      return attached_tileset_entity;
    }
  }

  return kNullEntity;
}

}  // namespace tactile::sys
