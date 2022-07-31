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

#include "core/comp/component_index.hpp"
#include "core/document/map_document.hpp"
#include "core/document/tileset_document.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/model.hpp"
#include "meta/profile.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

void _convert_context(const IContext&       context,
                      const ComponentIndex* components,
                      ir::ContextData&      data)
{
  for (const auto& [name, property] : context.get_props()) {
    data.properties[name] = property;
  }

  if (components) {
    for (const auto& [componentId, component] : context.get_comps()) {
      const auto& definition = components->at(component.definition_id());
      const auto& name = definition.get_name();

      auto& attributes = data.components[name];
      for (const auto& [attrName, attrValue] : component) {
        attributes[attrName] = attrValue;
      }
    }
  }
}

void _convert_object(const Object&         object,
                     const ComponentIndex* components,
                     ir::ObjectData&       data)
{
  TACTILE_ASSERT(object.get_meta_id().has_value());

  data.id = object.get_meta_id().value();
  data.type = object.get_type();

  data.pos = object.get_pos();
  data.size = object.get_size();

  data.tag = object.get_tag();
  data.visible = object.is_visible();

  data.name = object.get_name();
  _convert_context(object, components, data.context);
}

void _convert_layer(const ILayer&         layer,
                    const usize           index,
                    const ComponentIndex* components,
                    ir::LayerData&        data);

void _convert_object_layer(const ObjectLayer&    layer,
                           const ComponentIndex* index,
                           ir::ObjectLayerData&  data)
{
  data.objects.reserve(layer.object_count());
  for (const auto& [id, object] : layer) {
    auto& objectData = data.objects.emplace_back();
    _convert_object(*object, index, objectData);
  }
}

void _convert_group_layer(const GroupLayer&     layer,
                          const ComponentIndex* components,
                          ir::GroupLayerData&   data)
{
  struct Visitor final : IConstLayerVisitor
  {
    usize                 index {0};
    const ComponentIndex* comps {};
    ir::GroupLayerData*   parent {};

    void visit(const TileLayer& layer) override
    {
      handle(layer);
    }

    void visit(const ObjectLayer& layer) override
    {
      handle(layer);
    }

    void visit(const GroupLayer& layer) override
    {
      handle(layer);
    }

    void handle(const ILayer& layer)
    {
      auto& layerData = parent->children.emplace_back(std::make_unique<ir::LayerData>());
      _convert_layer(layer, index, comps, *layerData);
      ++index;
    }
  };

  data.children.reserve(layer.layer_count());

  Visitor visitor;
  visitor.index = 0;
  visitor.comps = components;
  visitor.parent = &data;

  layer.each(visitor);
}

void _convert_layer(const ILayer&         layer,
                    const usize           index,
                    const ComponentIndex* components,
                    ir::LayerData&        data)
{
  TACTILE_ASSERT(layer.get_meta_id().has_value());
  data.index = index;

  data.id = layer.get_meta_id().value();
  data.type = layer.get_type();
  data.opacity = layer.get_opacity();
  data.visible = layer.is_visible();

  data.name = layer.get_name();

  switch (layer.get_type()) {
    case LayerType::TileLayer: {
      const auto& tileLayer = dynamic_cast<const TileLayer&>(layer);

      auto& tileLayerData = data.data.emplace<ir::TileLayerData>();
      tileLayerData.tiles = tileLayer.get_tiles();
      tileLayerData.row_count = tileLayer.row_count();
      tileLayerData.col_count = tileLayer.column_count();

      break;
    }
    case LayerType::ObjectLayer: {
      const auto& objectLayer = dynamic_cast<const ObjectLayer&>(layer);
      auto&       objectLayerData = data.data.emplace<ir::ObjectLayerData>();
      _convert_object_layer(objectLayer, components, objectLayerData);
      break;
    }
    case LayerType::GroupLayer: {
      const auto& groupLayer = dynamic_cast<const GroupLayer&>(layer);
      auto&       groupLayerData = data.data.emplace<ir::GroupLayerData>();
      _convert_group_layer(groupLayer, components, groupLayerData);
      break;
    }
  }

  _convert_context(layer, components, data.context);
}

void _convert_layers(const MapDocument&    document,
                     const ComponentIndex* components,
                     ir::MapData&          data)
{
  usize index = 0;

  const auto& map = document.get_map();
  map.visit_layers([&](const ILayer* layer) {
    /* Only iterate top-level layers */
    if (!layer->get_parent()) {
      auto& layerData = data.layers.emplace_back();
      _convert_layer(*layer, index, components, layerData);
      ++index;
    }
  });
}

void _convert_fancy_tile_animation(const TileAnimation& animation, ir::MetaTileData& data)
{
  data.frames.reserve(animation.size());

  for (const auto& frame : animation) {
    auto& frameData = data.frames.emplace_back();
    frameData.local_id = frame.tile;
    frameData.duration_ms = static_cast<uint64>(frame.duration.count());
  }
}

void _convert_fancy_tiles(const Tileset&        tileset,
                          const ComponentIndex* components,
                          ir::TilesetData&      data)
{
  for (const auto& [id, tile] : tileset) {
    const auto isAnimated = tile->is_animated();
    const auto hasObjects = tile->object_count() != 0;
    const auto hasProps = !tile->get_props().empty();
    const auto hasComps = !tile->get_comps().empty();

    if (isAnimated || hasObjects || hasProps || hasComps) {
      auto& tileData = data.fancy_tiles[tile->index()];

      if (isAnimated) {
        _convert_fancy_tile_animation(tile->get_animation(), tileData);
      }

      if (hasProps || hasComps) {
        _convert_context(*tile, components, tileData.context);
      }

      if (hasObjects) {
        for (const auto& [objectId, object] : tile->get_objects()) {
          auto& objectData = tileData.objects.emplace_back();
          _convert_object(*object, components, objectData);
        }
      }
    }
  }
}

void _convert_tilesets(const MapDocument&    document,
                       const ComponentIndex* components,
                       ir::MapData&          data)
{
  const auto& map = document.get_map();
  for (const auto& [tilesetId, tilesetRef] : map.get_tilesets()) {
    const auto& tileset = tilesetRef.view_tileset();

    auto& tilesetData = data.tilesets.emplace_back();
    tilesetData.name = tileset.get_name();

    tilesetData.first_tile = tilesetRef.first_tile();
    tilesetData.tile_size = tileset.tile_size();
    tilesetData.column_count = tileset.column_count();

    tilesetData.image_path = tileset.texture_path();
    tilesetData.image_size = tileset.texture_size();

    _convert_fancy_tiles(tileset, components, tilesetData);
  }
}

void _convert_component_definitions(const ComponentIndex& manager, ir::MapData& data)
{
  for (const auto& [id, definition] : manager) {
    auto& attributes = data.component_definitions[definition.get_name()];
    for (const auto& [attrName, attrValue] : definition) {
      attributes[attrName] = attrValue;
    }
  }
}

}  // namespace

auto convert_map_to_ir(const MapDocument& document) -> ir::MapData
{
  TACTILE_DEBUG_PROFILE_START
  const auto& map = document.get_map();

  ir::MapData data;
  data.row_count = map.row_count();
  data.col_count = map.column_count();
  data.tile_size = map.tile_size();
  data.next_object_id = map.next_object_id();
  data.next_layer_id = map.next_layer_id();

  const auto* components = document.view_component_index();

  if (components) {
    _convert_component_definitions(*components, data);
  }

  _convert_tilesets(document, components, data);
  _convert_layers(document, components, data);
  _convert_context(map, components, data.context);

  TACTILE_DEBUG_PROFILE_END("Converted document to IR")
  return data;
}

}  // namespace tactile::io