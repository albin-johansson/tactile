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

#include "convert_map_to_ir.hpp"

#include "core/common/ecs.hpp"
#include "core/components/animation.hpp"
#include "core/components/attributes.hpp"
#include "core/components/map_info.hpp"
#include "core/components/parent.hpp"
#include "core/components/texture.hpp"
#include "core/components/tiles.hpp"
#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/model.hpp"
#include "core/systems/component_system.hpp"
#include "core/systems/tilesets/tileset_system.hpp"
#include "meta/profile.hpp"

namespace tactile::io {
namespace {

void _convert_attribute_context(ir::AttributeContextData& data,
                                const comp::AttributeContext& context,
                                const entt::registry& registry)
{
  for (const auto propertyEntity : context.properties) {
    const auto& property = checked_get<comp::Property>(registry, propertyEntity);
    data.properties[property.name] = property.value;
  }

  for (const auto componentEntity : context.components) {
    const auto& component = checked_get<comp::Component>(registry, componentEntity);
    const auto type = sys::get_component_def_name(registry, component.type);

    auto& attributes = data.components[type];
    for (const auto& [attrName, attrValue] : component.values) {
      attributes[attrName] = attrValue;
    }
  }
}

void _convert_object(ir::ObjectData& data,
                     const comp::Object& object,
                     const comp::AttributeContext& context,
                     const entt::registry& registry)
{
  data.id = object.id;
  data.type = object.type;

  data.x = object.x;
  data.y = object.y;
  data.width = object.width;
  data.height = object.height;

  data.tag = object.tag;
  data.visible = object.visible;

  data.name = context.name;
  _convert_attribute_context(data.context, context, registry);
}

void _convert_layer(ir::LayerData& data,
                    usize index,
                    const entt::registry& registry,
                    entt::entity entity);

void _convert_object_layer(ir::ObjectLayerData& data,
                           const entt::registry& registry,
                           const entt::entity entity)
{
  const auto& objectLayer = checked_get<comp::ObjectLayer>(registry, entity);
  data.objects.reserve(objectLayer.objects.size());

  for (const auto objectEntity : objectLayer.objects) {
    const auto& object = checked_get<comp::Object>(registry, objectEntity);
    const auto& context = checked_get<comp::AttributeContext>(registry, objectEntity);

    auto& objectData = data.objects.emplace_back();
    _convert_object(objectData, object, context, registry);
  }
}

void _convert_group_layer(ir::GroupLayerData& data,
                          const entt::registry& registry,
                          const entt::entity entity)
{
  const auto& node = checked_get<comp::LayerTreeNode>(registry, entity);
  data.children.reserve(node.children.size());

  usize index = 0;
  for (const auto childEntity : node.children) {
    auto& childLayerData = data.children.emplace_back(std::make_unique<ir::LayerData>());
    _convert_layer(*childLayerData, index, registry, childEntity);
    ++index;
  }
}

void _convert_layer(ir::LayerData& data,
                    const usize index,
                    const entt::registry& registry,
                    const entt::entity entity)
{
  const auto& layer = checked_get<comp::Layer>(registry, entity);

  data.index = index;

  /* We don't use plain integer IDs for layers, but this approximation should work */
  data.id = static_cast<int32>(index) + 1;
  data.type = layer.type;
  data.opacity = layer.opacity;
  data.visible = layer.visible;

  switch (layer.type) {
    case LayerType::TileLayer: {
      const auto& tileLayer = checked_get<comp::TileLayer>(registry, entity);

      auto& tileLayerData = data.data.emplace<ir::TileLayerData>();
      tileLayerData.tiles = tileLayer.matrix;

      const auto& mapInfo = ctx_get<comp::MapInfo>(registry);
      tileLayerData.row_count = mapInfo.row_count;
      tileLayerData.col_count = mapInfo.column_count;

      break;
    }
    case LayerType::ObjectLayer: {
      auto& objectLayerData = data.data.emplace<ir::ObjectLayerData>();
      _convert_object_layer(objectLayerData, registry, entity);
      break;
    }
    case LayerType::GroupLayer: {
      auto& groupLayerData = data.data.emplace<ir::GroupLayerData>();
      _convert_group_layer(groupLayerData, registry, entity);
      break;
    }
  }

  const auto& context = checked_get<comp::AttributeContext>(registry, entity);
  data.name = context.name;
  _convert_attribute_context(data.context, context, registry);
}

void _convert_layers(ir::MapData& data, const entt::registry& registry)
{
  usize index = 0;

  /* Only iterate top-level layers */
  for (auto&& [entity, node] : registry.view<comp::LayerTreeNode>().each()) {
    const auto& parent = checked_get<comp::Parent>(registry, entity);
    if (parent.entity == entt::null) {
      auto& layerData = data.layers.emplace_back();
      _convert_layer(layerData, index, registry, entity);
      ++index;
    }
  }
}

void _convert_fancy_tile_animation(ir::MetaTileData& data,
                                   const comp::Animation& animation,
                                   const entt::registry& registry)
{
  data.frames.reserve(animation.frames.size());

  for (const auto frameEntity : animation.frames) {
    const auto& frame = checked_get<comp::AnimationFrame>(registry, frameEntity);

    auto& frameData = data.frames.emplace_back();
    frameData.local_id = sys::convert_to_local(registry, frame.tile).value();
    frameData.duration_ms = frame.duration.count();
  }
}

void _convert_fancy_tiles(ir::TilesetData& data,
                          const comp::TilesetRef& tileset,
                          const entt::registry& registry)
{
  for (auto&& [entity, tile, context] :
       registry.view<comp::MetaTile, comp::AttributeContext>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      const bool interesting = registry.all_of<comp::Animation>(entity) ||
                               !context.properties.empty() ||
                               !context.properties.empty() || !tile.objects.empty();

      if (interesting) {
        const auto local = sys::convert_to_local(registry, tile.id).value();
        auto& tileData = data.fancy_tiles[local];

        if (const auto* animation = registry.try_get<comp::Animation>(entity)) {
          _convert_fancy_tile_animation(tileData, *animation, registry);
        }

        if (!context.properties.empty() || !context.components.empty()) {
          _convert_attribute_context(tileData.context, context, registry);
        }

        if (!tile.objects.empty()) {
          for (const auto objectEntity : tile.objects) {
            const auto& object = checked_get<comp::Object>(registry, objectEntity);
            const auto& objectContext =
                checked_get<comp::AttributeContext>(registry, objectEntity);

            auto& objectData = tileData.objects.emplace_back();
            _convert_object(objectData, object, objectContext, registry);
          }
        }
      }
    }
  }
}

void _convert_tilesets(const DocumentModel& model,
                       const entt::registry& mapRegistry,
                       ir::MapData& data)
{
  for (auto&& [entity, tilesetRef, context] :
       mapRegistry.view<comp::TilesetRef, comp::AttributeContext>().each()) {
    auto& tilesetData = data.tilesets.emplace_back();
    tilesetData.name = context.name;

    const auto& tilesetDocument = model.view_tileset(tilesetRef.source_tileset);
    const auto& tileset = tilesetDocument.info();
    const auto& texture = tilesetDocument.texture();

    tilesetData.first_tile = tilesetRef.first_id;
    tilesetData.tile_width = tileset.tile_size.x;
    tilesetData.tile_height = tileset.tile_size.y;
    tilesetData.column_count = tileset.column_count;

    tilesetData.image_path = texture.path;
    tilesetData.image_width = texture.size.x;
    tilesetData.image_height = texture.size.y;

    _convert_fancy_tiles(tilesetData, tilesetRef, mapRegistry);
  }
}

void _convert_component_definitions(ir::MapData& data, const entt::registry& registry)
{
  for (auto&& [entity, def] : registry.view<comp::ComponentDef>().each()) {
    auto& attributes = data.component_definitions[def.name];
    for (const auto& [attrName, attrValue] : def.attributes) {
      attributes[attrName] = attrValue;
    }
  }
}

void _convert_basic_map_info(ir::MapData& data, const comp::MapInfo& mapInfo)
{
  data.row_count = mapInfo.row_count;
  data.col_count = mapInfo.column_count;

  data.tile_width = mapInfo.tile_size.x;
  data.tile_height = mapInfo.tile_size.y;

  data.next_object_id = mapInfo.next_object_id;
}

}  // namespace

auto convert_map_to_ir(const DocumentModel& model, const UUID& documentId) -> ir::MapData
{
  TACTILE_PROFILE_START

  const auto& document = model.view_map(documentId);
  const auto& mapRegistry = document.get_registry();

  ir::MapData data;
  _convert_basic_map_info(data, document.info());

  _convert_component_definitions(data, mapRegistry);
  _convert_tilesets(model, mapRegistry, data);

  data.layers.reserve(mapRegistry.storage<comp::Layer>().size());
  _convert_layers(data, mapRegistry);

  _convert_attribute_context(data.context,
                             ctx_get<comp::AttributeContext>(mapRegistry),
                             mapRegistry);

  TACTILE_PROFILE_END("Converted document to IR")
  return data;
}

}  // namespace tactile::io