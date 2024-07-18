// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/map.hpp"

#include "tactile/base/io/save/ir.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer_common.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/map/map_spec.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/tile/tileset.hpp"
#include "tactile/core/tile/tileset_spec.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile {

auto is_map(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) &&           //
         registry.has<CMap>(entity) &&            //
         registry.has<CTileFormat>(entity) &&     //
         registry.has<CMapIdCache>(entity) &&     //
         registry.has<CLayerSuffixes>(entity) &&  //
         registry.has<CViewport>(entity);
}

auto make_map(Registry& registry, const MapSpec& spec) -> EntityID
{
  const SetLogScope log_scope {"Map"};

  if (!is_valid(spec)) {
    TACTILE_LOG_ERROR("Tried to create map from invalid map specification");
    return kInvalidEntity;
  }

  const auto map_entity = registry.make_entity();

  registry.add<CMeta>(map_entity);

  auto& map = registry.add<CMap>(map_entity);
  map.orientation = spec.orientation;
  map.extent = spec.extent;
  map.tile_size = spec.tile_size;
  map.root_layer = make_group_layer(registry);
  map.active_layer = kInvalidEntity;
  map.active_tileset = kInvalidEntity;

  auto& format = registry.add<CTileFormat>(map_entity);
  format.encoding = TileEncoding::kPlainText;
  format.compression = kNone;
  format.comp_level = kNone;

  auto& id_cache = registry.add<CMapIdCache>(map_entity);
  id_cache.next_tile_id = TileID {1};
  id_cache.next_object_id = 1;
  id_cache.next_layer_id = 1;

  auto& suffixes = registry.add<CLayerSuffixes>(map_entity);
  suffixes.tile_layer_suffix = 1;
  suffixes.object_layer_suffix = 1;
  suffixes.group_layer_suffix = 1;

  auto& viewport = registry.add<CViewport>(map_entity);
  viewport.pos = Float2 {0, 0};
  viewport.size = Float2 {0, 0};
  viewport.scale = 1.0f;

  TACTILE_ASSERT(is_map(registry, map_entity));
  return map_entity;
}

auto make_map(Registry& registry,
              IRenderer& renderer,
              const ir::Map& ir_map) -> Result<EntityID>
{
  const auto map_id = registry.make_entity();

  registry.add<CMeta>(map_id);
  convert_ir_metadata(registry, map_id, ir_map.meta);

  auto& map = registry.add<CMap>(map_id);
  map.orientation = TileOrientation::kOrthogonal;  // TODO
  map.extent = ir_map.extent;
  map.tile_size = ir_map.tile_size;
  map.root_layer = make_group_layer(registry);
  map.active_layer = kInvalidEntity;
  map.active_tileset = kInvalidEntity;

  auto& format = registry.add<CTileFormat>(map_id);
  format.encoding = ir_map.tile_format.encoding;
  format.compression = ir_map.tile_format.compression;
  format.comp_level = ir_map.tile_format.compression_level;

  auto& id_cache = registry.add<CMapIdCache>(map_id);
  id_cache.next_tile_id = TileID {1};  // TODO
  id_cache.next_object_id = ir_map.next_object_id;
  id_cache.next_layer_id = ir_map.next_layer_id;

  auto& suffixes = registry.add<CLayerSuffixes>(map_id);
  suffixes.tile_layer_suffix = 1;
  suffixes.object_layer_suffix = 1;
  suffixes.group_layer_suffix = 1;

  auto& viewport = registry.add<CViewport>(map_id);
  viewport.pos = Float2 {0, 0};
  viewport.size = Float2 {0, 0};
  viewport.scale = 1.0f;

  // TODO components

  map.attached_tilesets.reserve(ir_map.tilesets.size());
  for (const auto& ir_tileset_ref : ir_map.tilesets) {
    const auto tileset_id = make_tileset(registry, renderer, ir_tileset_ref);

    if (!tileset_id.has_value()) {
      return propagate_unexpected(tileset_id);
    }

    map.attached_tilesets.push_back(*tileset_id);
  }

  auto& root_layer = registry.get<CGroupLayer>(map.root_layer);
  root_layer.layers.reserve(ir_map.layers.size());

  for (const auto& ir_layer : ir_map.layers) {
    root_layer.layers.push_back(make_layer(registry, ir_layer));
  }

  TACTILE_ASSERT(is_map(registry, map_id));
  return map_id;
}

void destroy_map(Registry& registry, const EntityID map_entity)
{
  TACTILE_ASSERT(is_map(registry, map_entity));

  auto& map = registry.get<CMap>(map_entity);
  auto& root_layer = registry.get<CGroupLayer>(map.root_layer);

  for (const auto layer_entity : root_layer.layers) {
    if (registry.has<CGroupLayer>(layer_entity)) {
      destroy_group_layer(registry, layer_entity);
    }
    else if (registry.has<CObjectLayer>(layer_entity)) {
      destroy_object_layer(registry, layer_entity);
    }
    else {
      destroy_tile_layer(registry, layer_entity);
    }
  }

  registry.destroy(map.root_layer);
  registry.destroy(map_entity);
}

auto add_tileset_to_map(Registry& registry,
                        const EntityID map_entity,
                        const TilesetSpec& tileset_spec) -> Result<void>
{
  TACTILE_ASSERT(is_map(registry, map_entity));

  const SetLogScope log_scope {"Map"};

  auto& map = registry.get<CMap>(map_entity);
  auto& id_cache = registry.get<CMapIdCache>(map_entity);

  const auto tileset_entity = make_tileset(registry, tileset_spec);
  if (tileset_entity == kInvalidEntity) {
    TACTILE_LOG_ERROR("Could not create tileset");
    return unexpected(make_error(GenericError::kInvalidState));
  }

  const auto init_instance_result =
      init_tileset_instance(registry, tileset_entity, id_cache.next_tile_id);
  if (!init_instance_result) {
    TACTILE_LOG_ERROR("Could not initialize tileset instance: {}",
                      init_instance_result.error().message());
    return propagate_unexpected(init_instance_result);
  }

  const auto& tileset = registry.get<CTileset>(tileset_entity);
  id_cache.next_tile_id += saturate_cast<TileID>(tileset.tiles.size());

  map.attached_tilesets.push_back(tileset_entity);
  map.active_tileset = tileset_entity;

  TACTILE_LOG_DEBUG("Created and added tileset to map");
  return kOK;
}

}  // namespace tactile
