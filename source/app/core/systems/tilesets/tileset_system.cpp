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

#include <utility>  // move

#include <centurion/math.hpp>
#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

#include "core/common/associative.hpp"
#include "core/common/ecs.hpp"
#include "core/components/animation.hpp"
#include "core/components/attributes.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/components/viewport.hpp"
#include "core/systems/context_system.hpp"
#include "core/utils/tiles.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _create_source_rect_cache(const comp::TilesetRef& ref,
                                             const comp::Tileset& tileset)
    -> HashMap<TileID, cen::irect>
{
  HashMap<TileID, cen::irect> cache;

  const auto amount = (ref.last_id + 1) - ref.first_id;
  cache.reserve(static_cast<usize>(amount));

  for (TileID id{ref.first_id}; id <= ref.last_id; ++id) {
    const auto index = id - ref.first_id;

    const auto [row, col] = to_matrix_coords(index, tileset.column_count);
    const auto x = col * tileset.tile_size.x;
    const auto y = row * tileset.tile_size.y;

    cache[id] = cen::irect{x, y, tileset.tile_size.x, tileset.tile_size.y};
  }

  return cache;
}

void _refresh_tileset_cache(entt::registry& mapRegistry,
                            const entt::entity tilesetEntity,
                            const comp::Tileset& tileset)
{
  const auto& ref = checked_get<comp::TilesetRef>(mapRegistry, tilesetEntity);

  auto& cache = mapRegistry.emplace_or_replace<comp::TilesetCache>(tilesetEntity);
  cache.source_rects = _create_source_rect_cache(ref, tileset);

  for (auto&& [tileEntity, tile] : mapRegistry.view<comp::MetaTile>().each()) {
    if (tile.id >= ref.first_id && tile.id <= ref.last_id) {
      cache.tiles.try_emplace(tile.id, tileEntity);
    }
  }
}

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

void _add_viewport(entt::registry& registry,
                   const entt::entity tilesetEntity,
                   const Vector2i& tileSize)
{
  auto& viewport = registry.emplace<comp::Viewport>(tilesetEntity);
  viewport.offset = {0, 0};
  viewport.tile_size = tileSize;
}

}  // namespace

void update_tilesets(entt::registry& registry)
{
  for (auto&& [entity, cache] : registry.view<comp::TilesetCache>().each()) {
    cache.source_to_render.clear();
  }
}

void select_tileset(entt::registry& registry, const UUID& id)
{
  const auto entity = find_tileset(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  auto& activeTileset = ctx_get<comp::ActiveTileset>(registry);
  activeTileset.entity = entity;
}

void attach_tileset(entt::registry& mapRegistry,
                    const UUID& tilesetId,
                    const comp::Tileset& tileset,
                    const Maybe<TileID> firstTile)
{
  auto& tilesets = ctx_get<comp::TilesetContext>(mapRegistry);

  const auto tilesetEntity = mapRegistry.create();

  auto& context = mapRegistry.emplace<comp::AttributeContext>(tilesetEntity);
  context.name = "TilesetRef";  // FIXME

  auto& viewport = mapRegistry.emplace<comp::Viewport>(tilesetEntity);
  viewport.tile_size = tileset.tile_size;

  auto& limits = mapRegistry.emplace<comp::ViewportLimits>(tilesetEntity);
  limits.min_offset = {0, 0};
  limits.max_offset = {0, 0};  // This is set later by the tileset view

  auto& ref = mapRegistry.emplace<comp::TilesetRef>(tilesetEntity);
  ref.source_tileset = tilesetId;
  ref.embedded = false;  // FIXME

  ref.first_id = firstTile ? *firstTile : tilesets.next_tile_id;
  ref.last_id = ref.first_id + tileset.tile_count();

  if (!firstTile) {
    tilesets.next_tile_id = ref.last_id + 1;
  }

  mapRegistry.emplace<comp::Tileset>(tilesetEntity, tileset);
  mapRegistry.emplace<comp::TilesetSelection>(tilesetEntity);

  _register_new_tiles_in_tile_context(mapRegistry, tilesetEntity);
  _refresh_tileset_cache(mapRegistry, tilesetEntity, tileset);
}

auto detach_tileset(entt::registry& mapRegistry, const UUID& tilesetId) -> TilesetSnapshot
{
  const auto tilesetEntity = find_tileset(mapRegistry, tilesetId);
  if (tilesetEntity != entt::null) {
    const auto& tilesetRef = checked_get<comp::TilesetRef>(mapRegistry, tilesetEntity);

    TilesetSnapshot snapshot;
    // TODO remove snapshot.uv
    snapshot.selection = checked_get<comp::TilesetSelection>(mapRegistry, tilesetEntity);

    mapRegistry.destroy(tilesetEntity);

    return snapshot;
  }
  else {
    throw TactileError{"Invalid tileset identifier!"};
  }
}

void update_tileset_selection(entt::registry& registry, const Region& region)
{
  auto& active = ctx_get<comp::ActiveTileset>(registry);
  TACTILE_ASSERT(active.entity != entt::null);

  auto& selection = checked_get<comp::TilesetSelection>(registry, active.entity);
  selection.region = region;
}

auto find_tileset(const entt::registry& registry, const UUID& id) -> entt::entity
{
  for (auto&& [entity, tileset] : registry.view<comp::TilesetRef>().each()) {
    if (tileset.source_tileset == id) {
      return entity;
    }
  }

  return entt::null;
}

auto find_tile(const entt::registry& registry, const TileID id) -> entt::entity
{
  for (auto&& [entity, fancy] : registry.view<comp::MetaTile>().each()) {
    if (fancy.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto get_tile_entity(const entt::registry& registry, const TileID id) -> entt::entity
{
  const auto entity = find_tile(registry, id);
  if (entity != entt::null) {
    return entity;
  }
  else {
    throw TactileError{"Invalid tile ID!"};
  }
}

auto find_tileset_with_tile(const entt::registry& registry, const TileID id)
    -> entt::entity
{
  for (auto&& [entity, tileset] : registry.view<comp::TilesetRef>().each()) {
    if (id >= tileset.first_id && id <= tileset.last_id) {
      return entity;
    }
  }

  return entt::null;
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

auto get_tile_to_render(const entt::registry& mapRegistry,
                        const entt::entity tilesetEntity,
                        const TileID id) -> TileID
{
  const auto& cache = checked_get<comp::TilesetCache>(mapRegistry, tilesetEntity);

  /* Check for already cached tile to render */
  if (const auto iter = cache.source_to_render.find(id);
      iter != cache.source_to_render.end()) {
    return iter->second;
  }

  if (const auto iter = cache.tiles.find(id); iter != cache.tiles.end()) {
    const auto entity = iter->second;

    if (const auto* animation = mapRegistry.try_get<comp::Animation>(entity)) {
      const auto frameEntity = animation->frames.at(animation->index);
      const auto& frame = checked_get<comp::AnimationFrame>(mapRegistry, frameEntity);

      /* This cache is cleared before each frame */
      cache.source_to_render[id] = frame.tile;

      return frame.tile;
    }
  }

  return id;
}

auto get_source_rect(const entt::registry& mapRegistry,
                     const entt::entity tilesetEntity,
                     const TileID id) -> const cen::irect&
{
  const auto& cache = checked_get<comp::TilesetCache>(mapRegistry, tilesetEntity);
  if (const auto iter = cache.source_rects.find(id); iter != cache.source_rects.end()) {
    return iter->second;
  }
  else {
    throw TactileError{"Invalid tile identifier!"};
  }
}

auto get_tile_from_tileset(const entt::registry& mapRegistry,
                           const entt::entity tilesetEntity,
                           const TilePos& position) -> TileID
{
  const auto& ref = checked_get<comp::TilesetRef>(mapRegistry, tilesetEntity);
  const auto& tileset = checked_get<comp::Tileset>(mapRegistry, tilesetEntity);

  const auto row = position.row();
  const auto col = position.col();

  if (row >= 0 && col >= 0 && row < tileset.row_count && col < tileset.column_count) {
    const auto index = row * tileset.column_count + col;
    return ref.first_id + TileID{index};
  }
  else {
    return empty_tile;
  }
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
