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

#include "tileset_system.hpp"

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/components/tiles.hpp"
#include "core/components/viewport.hpp"
#include "core/utils/tiles.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {
namespace {

void _register_new_tiles_in_tile_context(entt::registry& mapRegistry,
                                         const entt::entity tilesetEntity)
{
  auto& tilesets = ctx_get<comp::TilesetContext>(mapRegistry);
  const auto& ref = checked_get<comp::TilesetRef>(mapRegistry, tilesetEntity);

  for (TileID tile = ref.first_id; tile <= ref.last_id; ++tile) {
    tilesets.tile_to_tileset[tile] = tilesetEntity;
  }
}

void _unregister_tiles_from_tile_context(entt::registry& mapRegistry,
                                         const entt::entity tilesetEntity)
{
  auto& tilesets = ctx_get<comp::TilesetContext>(mapRegistry);
  const auto& ref = checked_get<comp::TilesetRef>(mapRegistry, tilesetEntity);

  for (TileID tile = ref.first_id; tile <= ref.last_id; ++tile) {
    tilesets.tile_to_tileset.erase(tile);
  }
}

}  // namespace

void select_tileset(entt::registry& mapRegistry, const UUID& id)
{
  const auto entity = find_tileset(mapRegistry, id);
  TACTILE_ASSERT(entity != entt::null);

  auto& active = ctx_get<comp::ActiveTileset>(mapRegistry);
  active.entity = entity;
}

void attach_tileset(entt::registry& mapRegistry,
                    const UUID& tilesetId,
                    const comp::Tileset& tileset,
                    const Maybe<TileID> firstTile)
{
  auto& tilesets = ctx_get<comp::TilesetContext>(mapRegistry);

  const auto tilesetEntity = mapRegistry.create();

  auto& viewport = mapRegistry.emplace<comp::Viewport>(tilesetEntity);
  viewport.tile_size = tileset.tile_size;

  auto& ref = mapRegistry.emplace<comp::TilesetRef>(tilesetEntity);
  ref.source_tileset = tilesetId;
  ref.embedded = false;  // FIXME

  ref.first_id = firstTile ? *firstTile : tilesets.next_tile_id;
  ref.last_id = ref.first_id + tileset.tile_count();

  if (!firstTile) {
    tilesets.next_tile_id = ref.last_id + 1;
  }

  mapRegistry.emplace<comp::TilesetSelection>(tilesetEntity);

  /* The limits are defined by the tileset view */
  mapRegistry.emplace<comp::ViewportLimits>(tilesetEntity);

  _register_new_tiles_in_tile_context(mapRegistry, tilesetEntity);
}

void detach_tileset(entt::registry& mapRegistry, const UUID& tilesetId)
{
  const auto tilesetEntity = find_tileset(mapRegistry, tilesetId);
  if (tilesetEntity != entt::null) {
    _unregister_tiles_from_tile_context(mapRegistry, tilesetEntity);
    mapRegistry.destroy(tilesetEntity);
  }
  else {
    throw TactileError{"Invalid tileset identifier!"};
  }
}

void update_tileset_selection(entt::registry& mapRegistry, const Region& region)
{
  auto& active = ctx_get<comp::ActiveTileset>(mapRegistry);
  TACTILE_ASSERT(active.entity != entt::null);

  auto& selection = checked_get<comp::TilesetSelection>(mapRegistry, active.entity);
  selection.region = region;
}

auto find_tileset(const entt::registry& mapRegistry, const UUID& id) -> entt::entity
{
  return find_one<comp::TilesetRef>(mapRegistry, [&](const comp::TilesetRef& ref) {
    return ref.source_tileset == id;
  });
}

auto find_tileset_with_tile(const entt::registry& registry, const TileID id)
    -> entt::entity
{
  // TODO this can be made much faster if ranges are cached in a context component
  return find_one<comp::TilesetRef>(registry, [id](const comp::TilesetRef& ref) {
    return id >= ref.first_id && id <= ref.last_id;
  });
}

auto find_active_tileset(const entt::registry& registry) -> entt::entity
{
  const auto& active = ctx_get<comp::ActiveTileset>(registry);
  return active.entity;
}

auto is_tileset_selection_not_empty(const entt::registry& registry) -> bool
{
  const auto& active = ctx_get<comp::ActiveTileset>(registry);
  if (active.entity != entt::null) {
    const auto& selection = checked_get<comp::TilesetSelection>(registry, active.entity);
    return selection.region.has_value();
  }
  else {
    return false;
  }
}

auto is_single_tile_selected_in_tileset(const entt::registry& registry) -> bool
{
  const auto& active = ctx_get<comp::ActiveTileset>(registry);
  if (active.entity != entt::null) {
    const auto& selection = checked_get<comp::TilesetSelection>(registry, active.entity);
    if (selection.region) {
      const auto& region = *selection.region;
      return (region.end - region.begin) == TilePos{1, 1};
    }
  }

  return false;
}

auto convert_to_local(const entt::registry& mapRegistry, const TileID global)
    -> Maybe<TileID>
{
  const auto entity = find_tileset_with_tile(mapRegistry, global);
  if (entity != entt::null) {
    const auto& tileset = checked_get<comp::TilesetRef>(mapRegistry, entity);
    return global - tileset.first_id;
  }
  else {
    return nothing;
  }
}

}  // namespace tactile::sys
