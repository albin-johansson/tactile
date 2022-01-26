#include "tileset_system.hpp"

#include <unordered_map>  // unordered_map
#include <utility>        // move

#include <tactile_stdlib.hpp>

#include "assert.hpp"
#include "core/components/animation.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "core/viewport.hpp"
#include "logging.hpp"
#include "property_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto create_source_rect_cache(const Tileset& tileset)
    -> std::unordered_map<TileID, cen::irect>
{
  std::unordered_map<TileID, cen::irect> cache;

  const auto amount = (tileset.last_id + 1) - tileset.first_id;
  cache.reserve(static_cast<usize>(amount));

  for (TileID id{tileset.first_id}; id <= tileset.last_id; ++id) {
    const auto index = id - tileset.first_id;

    const auto [row, col] = ToMatrixCoords(index, tileset.column_count);
    const auto x = col * tileset.tile_width;
    const auto y = row * tileset.tile_height;

    cache.emplace(id, cen::irect{x, y, tileset.tile_width, tileset.tile_height});
  }

  return cache;
}

void refresh_tileset_cache(entt::registry& registry, const entt::entity entity)
{
  const auto& tileset = registry.get<Tileset>(entity);

  auto& cache = registry.emplace_or_replace<TilesetCache>(entity);
  cache.source_rects = create_source_rect_cache(tileset);

  for (auto&& [tileEntity, tile] : registry.view<comp::fancy_tile>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      cache.tiles.try_emplace(tile.id, tileEntity);
    }
  }
}

void register_new_tiles_in_tile_context(entt::registry& registry,
                                        const entt::entity tilesetEntity)
{
  auto& tilesets = registry.ctx<TilesetContext>();
  const auto& tileset = registry.get<Tileset>(tilesetEntity);

  for (TileID tile = tileset.first_id; tile <= tileset.last_id; ++tile) {
    tilesets.tile_to_tileset[tile] = tilesetEntity;
  }
}

void unregister_tiles_from_tile_context(entt::registry& registry,
                                        const entt::entity tilesetEntity)
{
  auto& tilesets = registry.ctx<TilesetContext>();
  const auto& tileset = registry.get<Tileset>(tilesetEntity);

  for (TileID tile = tileset.first_id; tile <= tileset.last_id; ++tile) {
    tilesets.tile_to_tileset.erase(tile);
  }
}

void add_viewport(entt::registry& registry,
                  const entt::entity tilesetEntity,
                  const int32 tileWidth,
                  const int32 tileHeight)
{
  auto& viewport = registry.emplace<Viewport>(tilesetEntity);
  viewport.x_offset = 0;
  viewport.y_offset = 0;
  viewport.tile_width = static_cast<float>(tileWidth);
  viewport.tile_height = static_cast<float>(tileHeight);
}

}  // namespace

void update_tilesets(entt::registry& registry)
{
  for (auto&& [entity, cache] : registry.view<TilesetCache>().each()) {
    cache.source_to_render.clear();
  }
}

auto make_tileset(entt::registry& registry,
                  const TileID firstId,
                  const Texture& texture,
                  const int32 tileWidth,
                  const int32 tileHeight) -> entt::entity
{
  auto& tilesets = registry.ctx<TilesetContext>();

  const auto id = tilesets.next_id;
  ++tilesets.next_id;

  LogDebug("Creating tileset with ID '{}'", id);

  const auto tilesetEntity = registry.create();
  auto& tileset = registry.emplace<Tileset>(tilesetEntity);
  tileset.id = id;

  tileset.tile_width = tileWidth;
  tileset.tile_height = tileHeight;

  tileset.row_count = texture.height / tileHeight;
  tileset.column_count = texture.width / tileWidth;
  tileset.tile_count = tileset.row_count * tileset.column_count;

  tileset.first_id = firstId;
  tileset.last_id = tileset.first_id + TileID{tileset.tile_count};
  tilesets.next_tile_id += tileset.tile_count + 1;

  registry.emplace<Texture>(tilesetEntity, texture);

  auto& uv = registry.emplace<UvTileSize>(tilesetEntity);
  uv.width = static_cast<float>(tileWidth) / static_cast<float>(texture.width);
  uv.height = static_cast<float>(tileHeight) / static_cast<float>(texture.height);

  auto& cache = registry.emplace<TilesetCache>(tilesetEntity);
  cache.source_rects = create_source_rect_cache(tileset);

  auto& context = AddPropertyContext(registry, tilesetEntity);
  context.name = texture.path.stem().string();

  registry.emplace<TilesetSelection>(tilesetEntity);

  add_viewport(registry, tilesetEntity, tileset.tile_width, tileset.tile_height);

  register_new_tiles_in_tile_context(registry, tilesetEntity);

  return tilesetEntity;
}

auto make_tileset(entt::registry& registry,
                  const Texture& texture,
                  const int32 tileWidth,
                  const int32 tileHeight) -> entt::entity
{
  auto& context = registry.ctx<TilesetContext>();
  return make_tileset(registry, context.next_tile_id, texture, tileWidth, tileHeight);
}

auto restore_tileset(entt::registry& registry, TilesetSnapshot snapshot) -> entt::entity
{
  LogDebug("Restoring tileset with ID '{}'", snapshot.core.id);
  const auto tilesetEntity = registry.create();

  auto& tileset = registry.emplace<Tileset>(tilesetEntity, std::move(snapshot.core));
  registry.emplace<TilesetSelection>(tilesetEntity, snapshot.selection);
  registry.emplace<Texture>(tilesetEntity, snapshot.texture);
  registry.emplace<UvTileSize>(tilesetEntity, snapshot.uv);

  add_viewport(registry, tilesetEntity, tileset.tile_width, tileset.tile_height);

  refresh_tileset_cache(registry, tilesetEntity);
  register_new_tiles_in_tile_context(registry, tilesetEntity);

  RestorePropertyContext(registry, tilesetEntity, std::move(snapshot.context));

  return tilesetEntity;
}

auto copy_tileset(const entt::registry& registry, const entt::entity source)
    -> TilesetSnapshot
{
  TACTILE_ASSERT(source != entt::null);
  TilesetSnapshot snapshot;

  snapshot.core = registry.get<Tileset>(source);
  snapshot.selection = registry.get<TilesetSelection>(source);
  snapshot.texture = registry.get<Texture>(source);
  snapshot.uv = registry.get<UvTileSize>(source);
  snapshot.context = CopyPropertyContext(registry, source);

  return snapshot;
}

void select_tileset(entt::registry& registry, const TilesetID id)
{
  LogVerbose("Selecting tileset '{}'", id);

  const auto entity = find_tileset(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  auto& activeTileset = registry.ctx<ActiveTileset>();
  activeTileset.entity = entity;
}

void remove_tileset(entt::registry& registry, const TilesetID id)
{
  LogDebug("Removing tileset '{}'", id);

  const auto entity = find_tileset(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  auto& activeTileset = registry.ctx<ActiveTileset>();
  if (entity == activeTileset.entity) {
    activeTileset.entity = entt::null;
  }

  auto& activeContext = registry.ctx<comp::active_attribute_context>();
  if (entity == activeContext.entity) {
    activeContext.entity = entt::null;
  }

  unregister_tiles_from_tile_context(registry, entity);

  registry.destroy(entity);

  if (!registry.view<Tileset>().empty()) {
    activeTileset.entity = registry.view<Tileset>().front();
  }
}

void update_tileset_selection(entt::registry& registry, const Region& region)
{
  auto& active = registry.ctx<ActiveTileset>();
  TACTILE_ASSERT(active.entity != entt::null);

  auto& selection = registry.get<TilesetSelection>(active.entity);
  selection.region = region;
}

auto find_tileset(const entt::registry& registry, const TilesetID id) -> entt::entity
{
  for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
    if (tileset.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto find_tileset_with_tile(const entt::registry& registry, const TileID id)
    -> entt::entity
{
  for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
    if (id >= tileset.first_id && id <= tileset.last_id) {
      return entity;
    }
  }

  return entt::null;
}

auto find_active_tileset(const entt::registry& registry) -> entt::entity
{
  const auto& active = registry.ctx<ActiveTileset>();
  return active.entity;
}

auto is_tileset_selection_not_empty(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTileset>();
  if (active.entity != entt::null) {
    const auto& selection = registry.get<TilesetSelection>(active.entity);
    return selection.region.has_value();
  }
  else {
    return false;
  }
}

auto is_single_tile_selected_in_tileset(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTileset>();
  if (active.entity != entt::null) {
    const auto& selection = registry.get<TilesetSelection>(active.entity);
    if (selection.region) {
      const auto& region = *selection.region;
      return (region.end - region.begin) == tile_position{1, 1};
    }
  }

  return false;
}

auto get_tile_to_render(const entt::registry& registry,
                        const entt::entity tilesetEntity,
                        const TileID id) -> TileID
{
  const auto& cache = registry.get<TilesetCache>(tilesetEntity);

  /* Check for already cached tile to render */
  if (const auto iter = cache.source_to_render.find(id);
      iter != cache.source_to_render.end()) {
    return iter->second;
  }

  if (const auto iter = cache.tiles.find(id); iter != cache.tiles.end()) {
    const auto entity = iter->second;

    if (const auto* animation = registry.try_get<comp::animation>(entity)) {
      const auto frameEntity = animation->frames.at(animation->index);
      const auto& frame = registry.get<comp::animation_frame>(frameEntity);

      /* This cache is cleared before each frame */
      cache.source_to_render[id] = frame.tile;

      return frame.tile;
    }
  }

  return id;
}

auto get_source_rect(const entt::registry& registry,
                     const entt::entity tilesetEntity,
                     const TileID id) -> const cen::irect&
{
  const auto& cache = registry.get<TilesetCache>(tilesetEntity);
  return cache.source_rects.at(id);
}

auto get_tile_from_tileset(const entt::registry& registry,
                           const entt::entity entity,
                           const tile_position& position) -> TileID
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<Tileset>(entity));
  const auto& tileset = registry.get<Tileset>(entity);

  const auto row = position.row();
  const auto col = position.col();

  if ((row >= 0) && (col >= 0) && (row < tileset.row_count) &&
      (col < tileset.column_count)) {
    const auto index = row * tileset.column_count + col;
    return tileset.first_id + TileID{index};
  }
  else {
    return empty_tile;
  }
}

auto convert_to_local(const entt::registry& registry, const TileID global)
    -> Maybe<TileID>
{
  const auto entity = find_tileset_with_tile(registry, global);
  if (entity != entt::null) {
    const auto& tileset = registry.get<Tileset>(entity);
    return global - tileset.first_id;
  }
  else {
    return nothing;
  }
}

}  // namespace tactile::sys
