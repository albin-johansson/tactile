// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tileset.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/io/texture.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/numeric/narrow.hpp"
#include "tactile/core/numeric/vec_stream.hpp"
#include "tactile/core/tile/animation.hpp"
#include "tactile/core/tile/tile.hpp"
#include "tactile/core/tile/tileset_spec.hpp"
#include "tactile/core/ui/viewport.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {
inline namespace tileset {

void _create_tiles(Registry& registry, CTileset& tileset)
{
  const auto tile_count = tileset.extent.rows * tileset.extent.cols;
  tileset.tiles.reserve(tile_count);

  for (usize index = 0; index < tile_count; ++index) {
    const auto tile_index = narrow_cast<TileIndex>(index);
    const auto tile_entity = make_tile(registry, tile_index);

    tileset.tiles.push_back(tile_entity);
  }
}

}  // namespace tileset

auto is_tileset(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) &&     //
         registry.has<CTileset>(entity) &&  //
         registry.has<CTexture>(entity) &&  //
         registry.has<CViewport>(entity);
}

auto make_tileset(Registry& registry, const TilesetSpec& spec) -> EntityID
{
  const SetLogScope log_scope {"Tileset"};

  if (spec.tile_size.x() <= 0 || spec.tile_size.y() <= 0) {
    TACTILE_LOG_ERROR("Tried to create tileset with invalid tile size: {}",
                      spec.tile_size);
    return kInvalidEntity;
  }

  const MatrixExtent extent {
    .rows = static_cast<MatrixExtent::value_type>(spec.texture.size.y() /
                                                  spec.tile_size.y()),
    .cols = static_cast<MatrixExtent::value_type>(spec.texture.size.x() /
                                                  spec.tile_size.x()),
  };

  if (extent.rows <= 0 || extent.cols <= 0) {
    TACTILE_LOG_ERROR("Tried to create tileset with invalid extent: {}",
                      extent);
    return kInvalidEntity;
  }

  const auto tileset_entity = registry.make_entity();

  auto& tileset = registry.add<CTileset>(tileset_entity);
  tileset.tile_size = spec.tile_size;
  tileset.extent = extent;

  auto& viewport = registry.add<CViewport>(tileset_entity);
  viewport.pos = Float2 {0.0f, 0.0f};
  viewport.size = vec_cast<Float2>(spec.texture.size) * 0.5f;
  viewport.scale = 1.0f;

  registry.add<CMeta>(tileset_entity);
  registry.add<CTexture>(tileset_entity, spec.texture);

  _create_tiles(registry, tileset);

  TACTILE_ASSERT(is_tileset(registry, tileset_entity));
  TACTILE_ASSERT(tileset.extent.rows > 0);
  TACTILE_ASSERT(tileset.extent.cols > 0);
  return tileset_entity;
}

auto init_tileset_instance(Registry& registry,
                           const EntityID tileset_entity,
                           const TileID first_tile_id) -> Result<void>
{
  TACTILE_ASSERT(is_tileset(registry, tileset_entity));
  const SetLogScope log_scope {"Tileset"};

  if (first_tile_id < TileID {1}) {
    TACTILE_LOG_ERROR("Tried to use invalid first tile identifier: {}",
                      first_tile_id);
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  if (registry.has<CTilesetInstance>(tileset_entity)) {
    TACTILE_LOG_ERROR("Tried to initialize tileset instance more than once");
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  const auto& tileset = registry.get<CTileset>(tileset_entity);

  const TileRange tile_range {
    .first_id = first_tile_id,
    .count = narrow_cast<int32>(tileset.tiles.size()),
  };

  if (!is_tile_range_available(registry, tile_range)) {
    TACTILE_LOG_ERROR("Requested tile range is unavailable: [{}, {})",
                      tile_range.first_id,
                      tile_range.first_id + tile_range.count);
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  auto& instance = registry.add<CTilesetInstance>(tileset_entity);
  instance.tile_range = tile_range;
  instance.is_embedded = false;  // TODO

  auto& tile_cache = registry.get<CTileCache>();
  tile_cache.tileset_mapping.reserve(tile_cache.tileset_mapping.size() +
                                     tileset.tiles.size());

  for (int32 index = 0; index < tile_range.count; ++index) {
    const TileID tile_id {tile_range.first_id + index};
    tile_cache.tileset_mapping.insert_or_assign(tile_id, tileset_entity);
  }

  TACTILE_LOG_DEBUG("Initialized tileset instance with tile range [{}, {})",
                    tile_range.first_id,
                    tile_range.first_id + tile_range.count);
  return kOK;
}

void destroy_tileset(Registry& registry, const EntityID tileset_entity)
{
  TACTILE_ASSERT(is_tileset(registry, tileset_entity));
  const auto& tileset = registry.get<CTileset>(tileset_entity);

  for (const auto tile_entity : tileset.tiles) {
    destroy_tile(registry, tile_entity);
  }

  if (const auto* instance = registry.find<CTilesetInstance>(tileset_entity)) {
    auto& tile_cache = registry.get<CTileCache>();
    for (int32 index = 0; index < instance->tile_range.count; ++index) {
      const TileID tile_id {instance->tile_range.first_id + index};
      tile_cache.tileset_mapping.erase(tile_id);
    }
  }

  registry.destroy(tileset_entity);
}

auto copy_tileset(Registry& registry,
                  const EntityID old_tileset_entity) -> EntityID
{
  TACTILE_ASSERT(is_tileset(registry, old_tileset_entity));
  const auto& old_meta = registry.get<CMeta>(old_tileset_entity);
  const auto& old_tileset = registry.get<CTileset>(old_tileset_entity);

  const auto new_tileset_entity = registry.make_entity();

  registry.add<CMeta>(new_tileset_entity, old_meta);

  auto& new_tileset = registry.add<CTileset>(new_tileset_entity);
  new_tileset.extent = old_tileset.extent;
  new_tileset.tile_size = old_tileset.tile_size;

  new_tileset.tiles.reserve(old_tileset.tiles.size());
  for (const auto old_tile_entity : old_tileset.tiles) {
    new_tileset.tiles.push_back(copy_tile(registry, old_tile_entity));
  }

  if (const auto* instance =
          registry.find<CTilesetInstance>(old_tileset_entity)) {
    registry.add<CTilesetInstance>(new_tileset_entity, *instance);
  }

  return new_tileset_entity;
}

auto get_tile_appearance(const Registry& registry,
                         const EntityID tileset_entity,
                         const TileIndex tile_index) -> TileIndex
{
  TACTILE_ASSERT(is_tileset(registry, tileset_entity));
  const auto& tileset = registry.get<CTileset>(tileset_entity);

  const auto tile_entity = tileset.tiles.at(static_cast<usize>(tile_index));
  if (const auto* animation = registry.find<CAnimation>(tile_entity)) {
    return animation->frames.at(animation->frame_index).tile_index;
  }

  return tile_index;
}

auto find_tileset(const Registry& registry, const TileID tile_id) -> EntityID
{
  TACTILE_ASSERT(registry.has<CTileCache>());
  const auto& tile_cache = registry.get<CTileCache>();

  const auto* tileset_entity = find_in(tile_cache.tileset_mapping, tile_id);
  if (tileset_entity != nullptr) {
    return *tileset_entity;
  }

  return kInvalidEntity;
}

auto get_tile_index(const Registry& registry,
                    const TileID tile_id) -> Maybe<TileIndex>
{
  TACTILE_ASSERT(registry.has<CTileCache>());

  const auto tileset_entity = find_tileset(registry, tile_id);

  const auto* instance = registry.find<CTilesetInstance>(tileset_entity);
  if (instance != nullptr && has_tile(instance->tile_range, tile_id)) {
    return TileIndex {tile_id - instance->tile_range.first_id};
  }

  return kNone;
}

auto is_tile_range_available(const Registry& registry,
                             const TileRange& range) -> bool
{
  const auto first_tile = range.first_id;
  const auto last_tile = range.first_id + range.count - 1;

  if (first_tile < TileID {1}) {
    return false;
  }

  for (const auto& [tileset_entity, instance] :
       registry.each<CTilesetInstance>()) {
    if (has_tile(instance.tile_range, first_tile) ||
        has_tile(instance.tile_range, last_tile)) {
      return false;
    }
  }

  return true;
}

auto has_tile(const TileRange& tile_range, const TileID tile_id) -> bool
{
  return (tile_id >= tile_range.first_id) &&
         (tile_id < tile_range.first_id + tile_range.count);
}

}  // namespace tactile
