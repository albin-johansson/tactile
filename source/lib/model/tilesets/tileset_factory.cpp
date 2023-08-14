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

#include "tileset_factory.hpp"

#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/services/backend_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/textures/texture_components.hpp"
#include "model/tiles/tile_factory.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_ops.hpp"
#include "model/viewports/viewport_components.hpp"
#include "tactile/core/common/assert.hpp"

namespace tactile::sys {

auto create_tileset(Registry& registry, const Int2& tile_size, const Path& image_path)
    -> Entity
{
  auto& backend_service = ServiceLocator<BackendService>::get();

  const auto tileset_entity = registry.create_entity();

  registry.add<TilesetRenderCache>(tileset_entity);

  auto& context = registry.add<Context>(tileset_entity);
  context.name = "Tileset";

  auto& tileset = registry.add<Tileset>(tileset_entity);
  tileset.tile_size = tile_size;
  tileset.texture = backend_service.load_texture(registry, image_path);

  TACTILE_ASSERT(is_texture_entity(registry, tileset.texture));
  const auto& texture = registry.get<Texture>(tileset.texture);

  tileset.row_count = texture.size.y / tile_size.y;
  tileset.column_count = texture.size.x / tile_size.x;
  tileset.uv_size = Float2 {tile_size} / Float2 {texture.size};

  const auto tile_count = tileset.row_count * tileset.column_count;
  tileset.tiles.reserve(static_cast<usize>(tile_count));

  for (TileIndex tile_index = 0; tile_index < tile_count; ++tile_index) {
    const auto tile_entity = create_tile(registry, tileset, tile_index);
    tileset.tiles.push_back(tile_entity);
    tileset.tile_index_map[tile_index] = tile_entity;
  }

  TACTILE_ASSERT(is_tileset_entity(registry, tileset_entity));
  return tileset_entity;
}

auto create_attached_tileset(Registry& registry,
                             const Entity tileset_entity,
                             const TileID first_tile) -> Entity
{
  TACTILE_ASSERT(is_tileset_entity(registry, tileset_entity));
  const auto& tileset = registry.get<Tileset>(tileset_entity);

  const auto attached_tileset_entity = registry.create_entity();
  registry.add<DynamicViewportInfo>(attached_tileset_entity);

  auto& attached_tileset = registry.add<AttachedTileset>(attached_tileset_entity);
  attached_tileset.tileset = tileset_entity;
  attached_tileset.first_tile = first_tile;
  attached_tileset.last_tile = first_tile + tile_count(tileset);
  attached_tileset.embedded = false;  // TODO

  // TODO set limits
  auto& viewport = registry.add<Viewport>(attached_tileset_entity);
  viewport.offset = Float2 {0, 0};
  viewport.tile_size = tileset.tile_size;

  TACTILE_ASSERT(is_attached_tileset_entity(registry, attached_tileset_entity));
  return attached_tileset_entity;
}

}  // namespace tactile::sys
