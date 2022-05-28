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

#include "restore_map_from_ir.hpp"

#include <filesystem>  // absolute
#include <variant>     // get

#include <entt/entity/registry.hpp>
#include <fmt/format.h>
#include <glm/vec2.hpp>
#include <spdlog/spdlog.h>

#include "core/commands/command_stack.hpp"
#include "core/common/math.hpp"
#include "core/components/animation.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/components/objects.hpp"
#include "core/components/tiles.hpp"
#include "core/documents/document.hpp"
#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/model.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tilesets/tileset_document_system.hpp"
#include "core/utils/texture_manager.hpp"
#include "io/maps/ir.hpp"
#include "io/maps/parser/parse_data.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

void _restore_properties(entt::registry& registry,
                         const entt::entity entity,
                         const ir::AttributeContextData& source)
{
  const auto count = source.properties.size();

  auto& context = (entity != entt::null) ? registry.get<comp::AttributeContext>(entity)
                                         : ctx_get<comp::AttributeContext>(registry);
  context.properties.reserve(count);

  for (const auto& [propertyName, propertyValue] : source.properties) {
    const auto propertyEntity = registry.create();

    auto& property = registry.emplace<comp::Property>(propertyEntity);
    property.name = propertyName;
    property.value = propertyValue;

    context.properties.push_back(propertyEntity);
  }
}

void _restore_components(entt::registry& registry,
                         const entt::entity entity,
                         const ir::AttributeContextData& source)
{
  auto& context = (entity != entt::null) ? registry.get<comp::AttributeContext>(entity)
                                         : ctx_get<comp::AttributeContext>(registry);
  context.components.reserve(source.components.size());

  for (const auto& [type, attributes] : source.components) {
    const auto defEntity = sys::find_component_def(registry, type);
    TACTILE_ASSERT(defEntity != entt::null);

    const auto componentEntity = registry.create();
    context.components.push_back(componentEntity);

    const auto& def = checked_get<comp::ComponentDef>(registry, defEntity);
    auto& component = registry.emplace<comp::Component>(componentEntity);
    component.type = def.id;

    for (const auto& [attrName, attrValue] : attributes) {
      component.values[attrName] = attrValue;
    }
  }
}

auto _restore_object(entt::registry& registry, const ir::ObjectData& objectData)
    -> entt::entity
{
  const auto objectEntity = registry.create();

  auto& object = registry.emplace<comp::Object>(objectEntity);
  object.id = objectData.id;
  object.type = objectData.type;

  object.x = objectData.x;
  object.y = objectData.y;
  object.width = objectData.width;
  object.height = objectData.height;

  object.tag = objectData.tag;
  object.visible = objectData.visible;

  auto& context = sys::add_attribute_context(registry, objectEntity);
  context.name = objectData.name;

  _restore_properties(registry, objectEntity, objectData.context);
  _restore_components(registry, objectEntity, objectData.context);

  return objectEntity;
}

void _restore_object_layer(entt::registry& registry,
                           const entt::entity entity,
                           const ir::ObjectLayerData& objectLayerData)
{
  auto& objectLayer = registry.emplace<comp::ObjectLayer>(entity);
  objectLayer.objects.reserve(objectLayerData.objects.size());

  for (const auto& objectData : objectLayerData.objects) {
    const auto objectEntity = _restore_object(registry, objectData);
    objectLayer.objects.push_back(objectEntity);
  }
}

auto _restore_layer(entt::registry& registry,
                    const ir::LayerData& layerData,
                    const entt::entity parent = entt::null) -> entt::entity
{
  const auto entity = sys::new_layer_skeleton(registry,  //
                                              layerData.id,
                                              layerData.type,
                                              layerData.name,
                                              parent);

  auto& node = checked_get<comp::LayerTreeNode>(registry, entity);
  node.index = layerData.index;

  auto& layer = checked_get<comp::Layer>(registry, entity);
  layer.opacity = layerData.opacity;
  layer.visible = layerData.visible;

  switch (layerData.type) {
    case LayerType::TileLayer: {
      const auto& tileLayerData = std::get<ir::TileLayerData>(layerData.data);

      auto& tileLayer = registry.emplace<comp::TileLayer>(entity);
      tileLayer.matrix = tileLayerData.tiles;

      break;
    }
    case LayerType::ObjectLayer: {
      const auto& objectLayerData = std::get<ir::ObjectLayerData>(layerData.data);
      _restore_object_layer(registry, entity, objectLayerData);
      break;
    }
    case LayerType::GroupLayer: {
      registry.emplace<comp::GroupLayer>(entity);

      const auto& groupData = std::get<ir::GroupLayerData>(layerData.data);
      for (const auto& childLayerData : groupData.children) {
        _restore_layer(registry, *childLayerData, entity);
      }

      break;
    }
  }

  _restore_properties(registry, entity, layerData.context);
  _restore_components(registry, entity, layerData.context);

  return entity;
}

void _restore_layers(MapDocument& document, const ir::MapData& mapData)
{
  auto& registry = document.get_registry();
  for (const auto& layerData : mapData.layers) {
    _restore_layer(registry, layerData);
  }

  sys::sort_layers(registry);

  if (!registry.storage<comp::LayerTreeNode>().empty()) {
    auto& activeLayer = ctx_get<comp::ActiveLayer>(registry);
    activeLayer.entity = registry.view<comp::LayerTreeNode>().front();
  }
}

void _restore_tile_animation(entt::registry& registry,
                             const entt::entity tileEntity,
                             const int32 firstGlobalId,
                             const ir::MetaTileData& tileData)
{
  auto& animation = registry.emplace<comp::Animation>(tileEntity);
  animation.frames.reserve(tileData.frames.size());

  for (const auto& frameData : tileData.frames) {
    const auto frameEntity = registry.create();

    auto& frame = registry.emplace<comp::AnimationFrame>(frameEntity);
    frame.tile = firstGlobalId + frameData.local_id;
    frame.duration = cen::u64ms{frameData.duration_ms};

    animation.frames.push_back(frameEntity);
  }
}

void _restore_fancy_tile_objects(entt::registry& registry,
                                 comp::MetaTile& tile,
                                 const ir::MetaTileData& tileData)
{
  tile.objects.reserve(tileData.objects.size());

  for (const auto& objectData : tileData.objects) {
    const auto objectEntity = _restore_object(registry, objectData);
    tile.objects.push_back(objectEntity);
  }
}

void _restore_fancy_tiles(entt::registry& registry,
                          comp::TilesetCache& cache,
                          const ir::TilesetData& tilesetData)
{
  const auto firstGlobalId = tilesetData.first_tile;

  for (const auto& [id, tileData] : tilesetData.fancy_tiles) {
    const auto tileEntity = registry.create();

    auto& tile = registry.emplace<comp::MetaTile>(tileEntity);
    tile.id = firstGlobalId + id;

    cache.tiles[tile.id] = tileEntity;

    if (!tileData.frames.empty()) {
      _restore_tile_animation(registry, tileEntity, firstGlobalId, tileData);
    }

    if (!tileData.objects.empty()) {
      _restore_fancy_tile_objects(registry, tile, tileData);
    }

    auto& context = sys::add_attribute_context(registry, tileEntity);
    context.name = fmt::format("Tile {}", tile.id);

    _restore_properties(registry, tileEntity, tileData.context);
    _restore_components(registry, tileEntity, tileData.context);
  }
}

void _restore_tileset(DocumentModel& model,
                      TextureManager& textures,
                      const ir::TilesetData& tilesetData)
{
  TACTILE_ASSERT(model.active_document_id().has_value());

  // TODO compare tileset document absolute paths to recognize the same tileset being
  // loaded multiple times

  const auto texture = textures.load(tilesetData.image_path).value();
  const Vector2i tileSize{tilesetData.tile_width, tilesetData.tile_height};

  const auto tilesetId = model.restore_tileset(texture, tileSize, tilesetData.first_tile);

  auto tileset = model.get_tileset(tilesetId);
  auto& tilesetRegistry = tileset->get_registry();

  ctx_get<comp::AttributeContext>(tilesetRegistry).name = tilesetData.name;

  auto& cache = ctx_get<comp::TilesetCache>(tilesetRegistry);
  _restore_fancy_tiles(tilesetRegistry, cache, tilesetData);

  _restore_properties(tilesetRegistry, entt::null, tilesetData.context);
  _restore_components(tilesetRegistry, entt::null, tilesetData.context);
}

void _restore_tilesets(DocumentModel& model,
                       MapDocument& map,
                       TextureManager& textures,
                       const ir::MapData& mapData)
{
  for (const auto& tilesetData : mapData.tilesets) {
    _restore_tileset(model, textures, tilesetData);
  }

  auto& registry = map.get_registry();
  if (!registry.storage<comp::TilesetRef>().empty()) {
    auto& activeTileset = ctx_get<comp::ActiveTileset>(registry);
    activeTileset.entity = registry.view<comp::TilesetRef>().front();
  }
}

void _restore_component_definitions(entt::registry& registry, const ir::MapData& mapData)
{
  for (const auto& [name, attributes] : mapData.component_definitions) {
    const auto id = sys::make_component_def(registry, name);
    for (const auto& [attrName, attrValue] : attributes) {
      sys::make_component_attribute(registry, id, attrName, attrValue);
    }
  }
}

}  // namespace

void restore_map_from_ir(const parsing::ParseData& data,
                         DocumentModel& model,
                         TextureManager& textures)
{
  const auto& mapData = data.data();

  const auto mapId = model.add_map({mapData.tile_width, mapData.tile_height},
                                   mapData.row_count,
                                   mapData.col_count);
  model.select_document(mapId);

  auto document = model.get_map(mapId);

  auto map = model.get_map(mapId);
  auto& mapRegistry = map->get_registry();

  const auto path = std::filesystem::absolute(data.path());
  mapRegistry.ctx().emplace<std::filesystem::path>(path);

  auto& context = ctx_get<comp::AttributeContext>(mapRegistry);
  context.name = path.filename().string();

  auto& info = ctx_get<comp::MapInfo>(mapRegistry);
  info.next_layer_id = mapData.next_layer_id;
  info.next_object_id = mapData.next_object_id;
  info.tile_size.x = mapData.tile_width;
  info.tile_size.y = mapData.tile_height;
  info.row_count = mapData.row_count;
  info.column_count = mapData.col_count;

  _restore_component_definitions(mapRegistry, mapData);

  _restore_tilesets(model, *map, textures, mapData);
  _restore_layers(*map, mapData);

  _restore_properties(mapRegistry, entt::null, mapData.context);
  _restore_components(mapRegistry, entt::null, mapData.context);

  document->get_history().clear();
}

}  // namespace tactile
