#include "tileset_system.hpp"

#include <cassert>        // assert
#include <unordered_map>  // unordered_map
#include <utility>        // move

#include <tactile_stdlib.hpp>

#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/property_context.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/components/uv_tile_size.hpp"
#include "property_system.hpp"

namespace Tactile::Sys {
namespace {

[[nodiscard]] auto CreateSourceRectCache(const Tileset& tileset)
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

void RefreshTilesetCache(entt::registry& registry, const entt::entity entity)
{
  const auto& tileset = registry.get<Tileset>(entity);

  auto& cache = registry.emplace_or_replace<TilesetCache>(entity);
  cache.source_rects = CreateSourceRectCache(tileset);

  for (auto&& [tileEntity, tile] : registry.view<FancyTile>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      cache.tiles.try_emplace(tile.id, tileEntity);
    }
  }
}

}  // namespace

auto MakeTileset(entt::registry& registry,
                 const TileID firstId,
                 const Texture& texture,
                 const int32 tileWidth,
                 const int32 tileHeight) -> entt::entity
{
  auto& tilesets = registry.ctx<TilesetContext>();

  const auto id = tilesets.next_id;
  ++tilesets.next_id;

  const auto entity = registry.create();
  auto& tileset = registry.emplace<Tileset>(entity);
  tileset.id = id;

  tileset.tile_width = tileWidth;
  tileset.tile_height = tileHeight;

  tileset.row_count = texture.height / tileHeight;
  tileset.column_count = texture.width / tileWidth;
  tileset.tile_count = tileset.row_count * tileset.column_count;

  tileset.first_id = firstId;
  tileset.last_id = tileset.first_id + TileID{tileset.tile_count};
  tilesets.next_tile_id += tileset.tile_count + 1;

  registry.emplace<Texture>(entity, texture);

  auto& uv = registry.emplace<UvTileSize>(entity);
  uv.width = static_cast<float>(tileWidth) / static_cast<float>(texture.width);
  uv.height = static_cast<float>(tileHeight) / static_cast<float>(texture.height);

  auto& cache = registry.emplace<TilesetCache>(entity);
  cache.source_rects = CreateSourceRectCache(tileset);

  auto& context = AddPropertyContext(registry, entity);
  context.name = texture.path.stem().string();

  registry.emplace<TilesetSelection>(entity);

  return entity;
}

auto MakeTileset(entt::registry& registry,
                 const Texture& texture,
                 const int32 tileWidth,
                 const int32 tileHeight) -> entt::entity
{
  auto& context = registry.ctx<TilesetContext>();
  return MakeTileset(registry, context.next_tile_id, texture, tileWidth, tileHeight);
}

auto RestoreTileset(entt::registry& registry, TilesetSnapshot snapshot) -> entt::entity
{
  const auto entity = registry.create();

  registry.emplace<Tileset>(entity, std::move(snapshot.core));
  registry.emplace<TilesetSelection>(entity, snapshot.selection);
  registry.emplace<Texture>(entity, snapshot.texture);
  registry.emplace<UvTileSize>(entity, snapshot.uv);

  RefreshTilesetCache(registry, entity);
  RestorePropertyContext(registry, entity, std::move(snapshot.context));

  return entity;
}

auto CopyTileset(const entt::registry& registry, const entt::entity source)
    -> TilesetSnapshot
{
  assert(source != entt::null);
  TilesetSnapshot snapshot;

  snapshot.core = registry.get<Tileset>(source);
  snapshot.selection = registry.get<TilesetSelection>(source);
  snapshot.texture = registry.get<Texture>(source);
  snapshot.uv = registry.get<UvTileSize>(source);
  snapshot.context = CopyPropertyContext(registry, source);

  return snapshot;
}

void SelectTileset(entt::registry& registry, const TilesetID id)
{
  const auto entity = FindTileset(registry, id);
  assert(entity != entt::null);

  auto& activeTileset = registry.ctx<ActiveTileset>();
  activeTileset.entity = entity;
}

void RemoveTileset(entt::registry& registry, const TilesetID id)
{
  const auto entity = FindTileset(registry, id);
  assert(entity != entt::null);

  auto& activeTileset = registry.ctx<ActiveTileset>();
  if (entity == activeTileset.entity) {
    activeTileset.entity = entt::null;
  }

  auto& activeContext = registry.ctx<ActivePropertyContext>();
  if (entity == activeContext.entity) {
    activeContext.entity = entt::null;
  }

  registry.destroy(entity);

  if (!registry.empty<Tileset>()) {
    activeTileset.entity = registry.view<Tileset>().front();
  }
}

void UpdateTilesetSelection(entt::registry& registry, const Region& region)
{
  auto& active = registry.ctx<ActiveTileset>();
  assert(active.entity != entt::null);

  auto& selection = registry.get<TilesetSelection>(active.entity);
  selection.region = region;
}

auto FindTileset(const entt::registry& registry, const TilesetID id) -> entt::entity
{
  for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
    if (tileset.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto FindTilesetWithTile(const entt::registry& registry, const TileID id) -> entt::entity
{
  for (auto&& [entity, tileset] : registry.view<Tileset>().each()) {
    if (id >= tileset.first_id && id <= tileset.last_id) {
      return entity;
    }
  }

  return entt::null;
}

auto GetActiveTileset(const entt::registry& registry) -> entt::entity
{
  const auto& active = registry.ctx<ActiveTileset>();
  return active.entity;
}

auto HasNonEmptyTilesetSelection(const entt::registry& registry) -> bool
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

auto IsSingleTileSelectedInTileset(const entt::registry& registry) -> bool
{
  const auto& active = registry.ctx<ActiveTileset>();
  if (active.entity != entt::null) {
    const auto& selection = registry.get<TilesetSelection>(active.entity);
    if (selection.region) {
      const auto& region = *selection.region;
      return (region.end - region.begin) == MapPosition{1, 1};
    }
  }

  return false;
}

auto GetTileToRender(const entt::registry& registry,
                     const entt::entity tilesetEntity,
                     const TileID id) -> TileID
{
  const auto& cache = registry.get<TilesetCache>(tilesetEntity);

  if (const auto it = cache.tiles.find(id); it != cache.tiles.end()) {
    const auto entity = it->second;
    if (const auto* animation = registry.try_get<Animation>(entity)) {
      const auto frameEntity = animation->frames.at(animation->index);
      const auto& frame = registry.get<AnimationFrame>(frameEntity);
      return frame.tile;
    }
  }

  return id;
}

auto GetSourceRect(const entt::registry& registry,
                   const entt::entity tilesetEntity,
                   const TileID id) -> const cen::irect&
{
  const auto& cache = registry.get<TilesetCache>(tilesetEntity);
  return cache.source_rects.at(id);
}

auto GetTileFromTileset(const entt::registry& registry,
                        const entt::entity entity,
                        const MapPosition& position) -> TileID
{
  assert(entity != entt::null);
  assert(registry.all_of<Tileset>(entity));
  const auto& tileset = registry.get<Tileset>(entity);

  const auto row = position.GetRow();
  const auto col = position.GetColumn();

  if ((row >= 0) && (col >= 0) && (row < tileset.row_count) &&
      (col < tileset.column_count)) {
    const auto index = row * tileset.column_count + col;
    return tileset.first_id + TileID{index};
  }
  else {
    return empty_tile;
  }
}

auto ConvertToLocal(const entt::registry& registry, const TileID global) -> Maybe<TileID>
{
  const auto entity = FindTilesetWithTile(registry, global);
  if (entity != entt::null) {
    const auto& tileset = registry.get<Tileset>(entity);
    return global - tileset.first_id;
  }
  else {
    return nothing;
  }
}

}  // namespace Tactile::Sys
