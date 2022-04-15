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

#include "restore_document_from_ir.hpp"

#include <variant>  // get

#include <entt/entt.hpp>
#include <fmt/format.h>

#include "core/components/animation.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/components/tiles.hpp"
#include "core/map.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/utils/texture_manager.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

void _restore_properties(entt::registry& registry,
                         const entt::entity entity,
                         const ir::AttributeContextData& source)
{
  const auto count = source.properties.size();

  auto& context = (entity != entt::null) ? registry.get<comp::AttributeContext>(entity)
                                         : registry.ctx<comp::AttributeContext>();
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
                                         : registry.ctx<comp::AttributeContext>();
  context.components.reserve(source.components.size());

  for (const auto& [type, attributes] : source.components) {
    const auto defEntity = sys::find_component_def(registry, type);
    TACTILE_ASSERT(defEntity != entt::null);

    const auto componentEntity = registry.create();
    context.components.push_back(componentEntity);

    const auto& def = registry.get<comp::ComponentDef>(defEntity);
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
  const auto entity = sys::make_basic_layer(registry,  //
                                            layerData.id,
                                            layerData.type,
                                            layerData.name,
                                            parent);

  auto& node = registry.get<comp::LayerTreeNode>(entity);
  node.index = layerData.index;

  auto& layer = registry.get<comp::Layer>(entity);
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

void _restore_layers(Document& document, const ir::MapData& mapData)
{
  for (const auto& layerData : mapData.layers) {
    _restore_layer(document.registry, layerData);
  }

  sys::sort_layers(document.registry);

  if (!document.registry.storage<comp::LayerTreeNode>().empty()) {
    auto& activeLayer = document.registry.ctx<comp::ActiveLayer>();
    activeLayer.entity = document.registry.view<comp::LayerTreeNode>().front();
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

    cache.tiles.try_emplace(tile.id, tileEntity);

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

void _restore_tileset(entt::registry& registry,
                      TextureManager& textures,
                      const ir::TilesetData& tilesetData)
{
  const auto texture = textures.load(tilesetData.image_path).value();
  const auto entity = sys::make_tileset(registry,
                                        tilesetData.first_tile,
                                        texture,
                                        tilesetData.tile_width,
                                        tilesetData.tile_height);

  registry.get<comp::AttributeContext>(entity).name = tilesetData.name;

  auto& cache = registry.get<comp::TilesetCache>(entity);
  _restore_fancy_tiles(registry, cache, tilesetData);

  _restore_properties(registry, entity, tilesetData.context);
  _restore_components(registry, entity, tilesetData.context);
}

void _restore_tilesets(Document& document,
                       TextureManager& textures,
                       const ir::MapData& mapData)
{
  for (const auto& tilesetData : mapData.tilesets) {
    _restore_tileset(document.registry, textures, tilesetData);
  }

  if (!document.registry.storage<comp::Tileset>().empty()) {
    auto& activeTileset = document.registry.ctx<comp::ActiveTileset>();
    activeTileset.entity = document.registry.view<comp::Tileset>().front();
  }
}

void _restore_root_attribute_context(Document& document)
{
  auto& context = document.registry.ctx<comp::AttributeContext>();
  context.name = document.path.filename().string();
}

void _restore_map_context(MapInfo& map, const ir::MapData& mapData)
{
  map.next_layer_id = mapData.next_layer_id;
  map.next_object_id = mapData.next_object_id;

  map.tile_width = mapData.tile_width;
  map.tile_height = mapData.tile_height;

  map.row_count = mapData.row_count;
  map.column_count = mapData.col_count;
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

auto restore_document_from_ir(const parsing::ParseData& data, TextureManager& textures)
    -> Document
{
  Document document;
  document.path = data.path();
  document.registry = sys::make_document_registry();

  const auto& mapData = data.data();
  _restore_component_definitions(document.registry, mapData);

  _restore_map_context(document.registry.ctx<MapInfo>(), mapData);
  _restore_root_attribute_context(document);

  _restore_tilesets(document, textures, mapData);
  _restore_layers(document, mapData);

  _restore_properties(document.registry, entt::null, mapData.context);
  _restore_components(document.registry, entt::null, mapData.context);

  return document;
}

}  // namespace tactile
