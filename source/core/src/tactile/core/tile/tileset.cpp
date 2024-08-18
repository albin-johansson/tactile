// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tileset.hpp"

#include <system_error>  // make_error_code, errc
#include <utility>       // move

#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/numeric/vec_format.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/io/texture.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/tile/animation.hpp"
#include "tactile/core/tile/tile.hpp"
#include "tactile/core/tile/tileset_spec.hpp"
#include "tactile/core/ui/viewport.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {
namespace tileset {

[[nodiscard]]
auto add_tileset_component(Registry& registry,
                           const EntityID tileset_id,
                           const Int2& texture_size,
                           const Int2& tile_size) -> Result<void>
{
  if (tile_size.x() <= 0 || tile_size.y() <= 0) {
    TACTILE_LOG_ERROR("Invalid tileset tile size: {}", tile_size);
    return unexpected(std::make_error_code(std::errc::invalid_argument));
  }

  const MatrixExtent extent {
    .rows = static_cast<MatrixExtent::value_type>(texture_size.y() / tile_size.y()),
    .cols = static_cast<MatrixExtent::value_type>(texture_size.x() / tile_size.x()),
  };

  if (extent.rows <= 0 || extent.cols <= 0) {
    TACTILE_LOG_ERROR("Invalid tileset extent: {}", extent);
    return unexpected(std::make_error_code(std::errc::invalid_argument));
  }

  auto& tileset = registry.add<CTileset>(tileset_id);
  tileset.tile_size = tile_size;
  tileset.extent = extent;

  return kOK;
}

void add_viewport_component(Registry& registry,
                            const EntityID tileset_id,
                            const Int2& texture_size)
{
  auto& viewport = registry.add<CViewport>(tileset_id);
  viewport.pos = Float2 {0.0f, 0.0f};
  viewport.size = vec_cast<Float2>(texture_size) * 0.5f;
  viewport.scale = 1.0f;
}

void create_tiles(Registry& registry, CTileset& tileset)
{
  const auto tile_count = tileset.extent.rows * tileset.extent.cols;
  tileset.tiles.reserve(saturate_cast<usize>(tile_count));

  for (ssize index = 0; index < tile_count; ++index) {
    const auto tile_index = saturate_cast<TileIndex>(index);
    const auto tile_entity = make_tile(registry, tile_index);

    tileset.tiles.push_back(tile_entity);
  }
}

[[nodiscard]]
auto create_tiles(Registry& registry,
                  CTileset& tileset,
                  const ir::Tileset& ir_tileset) -> Result<void>
{
  const auto tile_count = saturate_cast<usize>(ir_tileset.tile_count);
  tileset.tiles.resize(tile_count, kInvalidEntity);

  for (const auto& ir_tile : ir_tileset.tiles) {
    const auto tile_id = make_tile(registry, ir_tile);
    if (!tile_id.has_value()) {
      return propagate_unexpected(tile_id);
    }

    const auto tile_index = saturate_cast<usize>(ir_tile.index);
    if (tile_index >= tileset.tiles.size()) {
      return unexpected(std::make_error_code(std::errc::result_out_of_range));
    }

    tileset.tiles[tile_index] = *tile_id;
  }

  TACTILE_ASSERT(tileset.tiles.size() == tile_count);
  for (usize tile_index = 0; tile_index < tile_count; ++tile_index) {
    if (tileset.tiles[tile_index] == kInvalidEntity) {
      tileset.tiles[tile_index] = make_tile(registry, saturate_cast<TileIndex>(tile_index));
    }
  }

  return kOK;
}

}  // namespace tileset

auto is_tileset(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) &&     //
         registry.has<CTileset>(entity) &&  //
         registry.has<CTexture>(entity) &&  //
         registry.has<CViewport>(entity);
}

auto is_tileset_instance(const Registry& registry, const EntityID entity) -> bool
{
  return is_tileset(registry, entity) && registry.has<CTilesetInstance>(entity);
}

auto make_tileset(Registry& registry, const TilesetSpec& spec) -> EntityID
{
  const auto tileset_id = registry.make_entity();

  const auto add_tileset_result =
      tileset::add_tileset_component(registry, tileset_id, spec.texture.size, spec.tile_size);
  if (!add_tileset_result.has_value()) {
    return kInvalidEntity;
  }

  tileset::add_viewport_component(registry, tileset_id, spec.texture.size);

  registry.add<CMeta>(tileset_id);
  registry.add<CTexture>(tileset_id, spec.texture);

  auto& tileset = registry.get<CTileset>(tileset_id);
  tileset::create_tiles(registry, tileset);

  TACTILE_ASSERT(is_tileset(registry, tileset_id));
  TACTILE_ASSERT(tileset.extent.rows > 0);
  TACTILE_ASSERT(tileset.extent.cols > 0);
  return tileset_id;
}

auto make_tileset(Registry& registry,
                  IRenderer& renderer,
                  const ir::TilesetRef& ir_tileset_ref) -> Result<EntityID>
{
  auto texture_result = load_texture(renderer, ir_tileset_ref.tileset.image_path);
  if (!texture_result.has_value()) {
    return propagate_unexpected(texture_result);
  }

  const auto tileset_id = registry.make_entity();
  registry.add<CMeta>(tileset_id);

  auto& texture = registry.add<CTexture>(tileset_id, std::move(*texture_result));

  tileset::add_viewport_component(registry, tileset_id, texture.size);

  const auto add_tileset_result =
      tileset::add_tileset_component(registry,
                                     tileset_id,
                                     texture.size,
                                     ir_tileset_ref.tileset.tile_size);
  if (!add_tileset_result.has_value()) {
    return kInvalidEntity;
  }

  auto& tileset = registry.get<CTileset>(tileset_id);

  const auto create_tiles_result =
      tileset::create_tiles(registry, tileset, ir_tileset_ref.tileset);
  if (!create_tiles_result.has_value()) {
    return propagate_unexpected(create_tiles_result);
  }

  const auto init_instance_result =
      init_tileset_instance(registry, tileset_id, ir_tileset_ref.first_tile_id);
  if (!init_instance_result.has_value()) {
    return propagate_unexpected(init_instance_result);
  }

  auto& tileset_instance = registry.get<CTilesetInstance>(tileset_id);
  tileset_instance.is_embedded = ir_tileset_ref.tileset.is_embedded;

  convert_ir_metadata(registry, tileset_id, ir_tileset_ref.tileset.meta);

  TACTILE_ASSERT(is_tileset(registry, tileset_id));
  return tileset_id;
}

auto init_tileset_instance(Registry& registry,
                           const EntityID tileset_entity,
                           const TileID first_tile_id) -> Result<void>
{
  TACTILE_ASSERT(is_tileset(registry, tileset_entity));
  const SetLogScope log_scope {"Tileset"};

  if (first_tile_id < TileID {1}) {
    TACTILE_LOG_ERROR("Tried to use invalid first tile identifier: {}", first_tile_id);
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  if (registry.has<CTilesetInstance>(tileset_entity)) {
    TACTILE_LOG_ERROR("Tried to initialize tileset instance more than once");
    return unexpected(make_error(GenericError::kInvalidParam));
  }

  const auto& tileset = registry.get<CTileset>(tileset_entity);

  const TileRange tile_range {
    .first_id = first_tile_id,
    .count = saturate_cast<int32>(tileset.tiles.size()),
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
  tile_cache.tileset_mapping.reserve(tile_cache.tileset_mapping.size() + tileset.tiles.size());

  for (int32 index = 0; index < tile_range.count; ++index) {
    const TileID tile_id {tile_range.first_id + index};
    tile_cache.tileset_mapping.insert_or_assign(tile_id, tileset_entity);
  }

  TACTILE_LOG_DEBUG("Initialized tileset instance with tile range [{}, {})",
                    tile_range.first_id,
                    tile_range.first_id + tile_range.count);
  return kOK;
}

auto make_tileset_instance(Registry& registry,
                           const TilesetSpec& spec,
                           const TileID first_tile_id) -> Result<EntityID>
{
  const auto tileset_id = make_tileset(registry, spec);

  if (tileset_id == kInvalidEntity) {
    TACTILE_LOG_ERROR("Could not create tileset");
    return unexpected(make_error(GenericError::kInvalidState));
  }

  const auto init_instance_result = init_tileset_instance(registry, tileset_id, first_tile_id);
  if (!init_instance_result) {
    TACTILE_LOG_ERROR("Could not create tileset instance: {}",
                      init_instance_result.error().message());
    return propagate_unexpected(init_instance_result);
  }

  return tileset_id;
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

auto copy_tileset(Registry& registry, const EntityID old_tileset_entity) -> EntityID
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

  if (const auto* instance = registry.find<CTilesetInstance>(old_tileset_entity)) {
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

auto get_tile_index(const Registry& registry, const TileID tile_id) -> Maybe<TileIndex>
{
  TACTILE_ASSERT(registry.has<CTileCache>());

  const auto tileset_entity = find_tileset(registry, tile_id);

  const auto* instance = registry.find<CTilesetInstance>(tileset_entity);
  if (instance != nullptr && has_tile(instance->tile_range, tile_id)) {
    return TileIndex {tile_id - instance->tile_range.first_id};
  }

  return kNone;
}

auto is_tile_range_available(const Registry& registry, const TileRange& range) -> bool
{
  const auto first_tile = range.first_id;
  const auto last_tile = range.first_id + range.count - 1;

  if (first_tile < TileID {1}) {
    return false;
  }

  for (const auto& [tileset_entity, instance] : registry.each<CTilesetInstance>()) {
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
