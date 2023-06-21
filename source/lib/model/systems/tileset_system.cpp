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
#include "model/systems/texture_system.hpp"
#include "model/systems/validation_system.hpp"
#include "model/tilesets/attached_tileset_ops.hpp"
#include "model/tilesets/tileset_components.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _create_tile(Model& model,
                                const Tileset& tileset,
                                const TileIndex tile_index) -> Entity
{
  const auto [row, col] = to_matrix_coords(tile_index, tileset.column_count);

  const auto tile_entity = model.create_entity();

  auto& context = model.add<Context>(tile_entity);
  context.name = "Tile";

  const Int2 tile_position {col * tileset.tile_size.x, row * tileset.tile_size.y};

  auto& tile = model.add<Tile>(tile_entity);
  tile.index = tile_index;
  tile.source = Int4 {tile_position, tileset.tile_size};

  TACTILE_ASSERT(is_tile_entity(model, tile_entity));
  return tile_entity;
}

}  // namespace

auto create_tileset(Model& model, const Int2& tile_size, const Path& image_path) -> Entity
{
  const auto tileset_entity = model.create_entity();

  auto& context = model.add<Context>(tileset_entity);
  context.name = "Tileset";

  auto& tileset = model.add<Tileset>(tileset_entity);
  tileset.tile_size = tile_size;
  tileset.texture = create_texture(model, image_path);

  model.add<TilesetRenderCache>(tileset_entity);

  const auto& texture = model.get<Texture>(tileset.texture);

  tileset.row_count = texture.size.y / tile_size.y;
  tileset.column_count = texture.size.x / tile_size.x;
  tileset.uv_size = Float2 {tile_size} / Float2 {texture.size};

  const auto tile_count = tileset.row_count * tileset.column_count;
  tileset.tiles.reserve(static_cast<usize>(tile_count));

  for (TileIndex tile_index = 0; tile_index < tile_count; ++tile_index) {
    const auto tile_entity = _create_tile(model, tileset, tile_index);
    tileset.tiles.push_back(tile_entity);
    tileset.tile_index_map[tile_index] = tile_entity;
  }

  TACTILE_ASSERT(is_texture_entity(model, tileset.texture));
  TACTILE_ASSERT(is_tileset_entity(model, tileset_entity));
  return tileset_entity;
}

auto create_attached_tileset(Model& model,
                             const Entity tileset_entity,
                             const TileID first_tile) -> Entity
{
  TACTILE_ASSERT(is_tileset_entity(model, tileset_entity));
  const auto& tileset = model.get<Tileset>(tileset_entity);

  const auto attached_tileset_entity = model.create_entity();

  auto& attached_tileset = model.add<AttachedTileset>(attached_tileset_entity);
  attached_tileset.tileset = tileset_entity;
  attached_tileset.embedded = false;
  attached_tileset.first_tile = first_tile;
  attached_tileset.last_tile = first_tile + (tileset.row_count * tileset.column_count);

  // TODO set limits
  auto& viewport = model.add<Viewport>(attached_tileset_entity);
  viewport.offset = Float2 {0, 0};
  viewport.tile_size = tileset.tile_size;

  model.add<DynamicViewportInfo>(attached_tileset_entity);

  TACTILE_ASSERT(is_attached_tileset_entity(model, attached_tileset_entity));
  return attached_tileset_entity;
}

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
