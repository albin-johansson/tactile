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

#include "map_to_ir.hpp"

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

void convert_context(const IContext&       context,
                     const ComponentIndex* components,
                     ir::ContextData&      data)
{
  for (const auto& [name, property] : context.get_props()) {
    data.properties[name] = property;
  }

  if (components) {
    for (const auto& [component_id, component] : context.get_comps()) {
      const auto& definition = components->at(component.definition_id());
      const auto& name = definition.get_name();

      auto& attributes = data.components[name];
      for (const auto& [attr_name, attr_value] : component) {
        attributes[attr_name] = attr_value;
      }
    }
  }
}

void convert_object(const Object&         object,
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
  convert_context(object, components, data.context);
}

void convert_layer(const ILayer&         layer,
                   usize                 index,
                   const ComponentIndex* components,
                   ir::LayerData&        data);

void convert_object_layer(const ObjectLayer&    layer,
                          const ComponentIndex* index,
                          ir::ObjectLayerData&  data)
{
  data.objects.reserve(layer.object_count());
  for (const auto& [id, object] : layer) {
    auto& object_data = data.objects.emplace_back();
    convert_object(*object, index, object_data);
  }
}

void convert_group_layer(const GroupLayer&     layer,
                         const ComponentIndex* components,
                         ir::GroupLayerData&   data)
{
  struct Visitor final : IConstLayerVisitor
  {
    usize                 index {0};
    const ComponentIndex* comps {};
    ir::GroupLayerData*   parent {};
    UUID                  parent_id {};

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
      if (layer.get_parent() == parent_id) {
        auto& layer_data =
            parent->children.emplace_back(std::make_unique<ir::LayerData>());
        convert_layer(layer, index, comps, *layer_data);
        ++index;
      }
    }
  };

  data.children.reserve(layer.layer_count());

  Visitor visitor;
  visitor.index = 0;
  visitor.comps = components;
  visitor.parent = &data;
  visitor.parent_id = layer.get_uuid();

  layer.each(visitor);
}

void convert_layer(const ILayer&         layer,
                   const usize           index,
                   const ComponentIndex* components,
                   ir::LayerData&        layer_data)
{
  TACTILE_ASSERT(layer.get_meta_id().has_value());
  layer_data.index = index;

  layer_data.id = layer.get_meta_id().value();
  layer_data.type = layer.get_type();
  layer_data.opacity = layer.get_opacity();
  layer_data.visible = layer.is_visible();

  layer_data.name = layer.get_name();

  switch (layer.get_type()) {
    case LayerType::TileLayer: {
      const auto& tile_layer = dynamic_cast<const TileLayer&>(layer);

      auto& tile_layer_data = layer_data.data.emplace<ir::TileLayerData>();
      tile_layer_data.tiles = tile_layer.get_tiles();
      tile_layer_data.row_count = tile_layer.row_count();
      tile_layer_data.col_count = tile_layer.column_count();

      break;
    }
    case LayerType::ObjectLayer: {
      const auto& object_layer = dynamic_cast<const ObjectLayer&>(layer);
      auto&       object_layer_data = layer_data.data.emplace<ir::ObjectLayerData>();
      convert_object_layer(object_layer, components, object_layer_data);
      break;
    }
    case LayerType::GroupLayer: {
      const auto& group_layer = dynamic_cast<const GroupLayer&>(layer);
      auto&       group_layer_data = layer_data.data.emplace<ir::GroupLayerData>();
      convert_group_layer(group_layer, components, group_layer_data);
      break;
    }
  }

  convert_context(layer, components, layer_data.context);
}

void convert_layers(const MapDocument&    document,
                    const ComponentIndex* components,
                    ir::MapData&          data)
{
  usize index = 0;

  const auto& map = document.get_map();
  map.visit_layers([&](const ILayer* layer) {
    // Only iterate top-level layers, and convert them recursively
    if (!layer->get_parent()) {
      auto& layer_data = data.layers.emplace_back();
      convert_layer(*layer, index, components, layer_data);
      ++index;
    }
  });
}

void convert_fancy_tile_animation(const TileAnimation& animation, ir::MetaTileData& data)
{
  data.frames.reserve(animation.size());

  for (const auto& frame : animation) {
    auto& frame_data = data.frames.emplace_back();
    frame_data.local_id = frame.tile;
    frame_data.duration_ms = static_cast<uint64>(frame.duration.count());
  }
}

void convert_fancy_tiles(const Tileset&        tileset,
                         const ComponentIndex* components,
                         ir::TilesetData&      data)
{
  for (const auto& [id, tile] : tileset) {
    const auto is_animated = tile->is_animated();
    const auto has_objects = tile->object_count() != 0;
    const auto has_props = !tile->get_props().empty();
    const auto has_comps = !tile->get_comps().empty();

    if (is_animated || has_objects || has_props || has_comps) {
      auto& tile_data = data.fancy_tiles[tile->index()];

      if (is_animated) {
        convert_fancy_tile_animation(tile->get_animation(), tile_data);
      }

      if (has_props || has_comps) {
        convert_context(*tile, components, tile_data.context);
      }

      if (has_objects) {
        for (const auto& [objectId, object] : tile->get_objects()) {
          auto& object_data = tile_data.objects.emplace_back();
          convert_object(*object, components, object_data);
        }
      }
    }
  }
}

void convert_tilesets(const MapDocument&    document,
                      const ComponentIndex* components,
                      ir::MapData&          data)
{
  const auto& map = document.get_map();
  for (const auto& [tileset_id, tileset_ref] : map.get_tilesets()) {
    const auto& tileset = tileset_ref.view_tileset();

    auto& tileset_data = data.tilesets.emplace_back();
    tileset_data.name = tileset.get_name();

    tileset_data.first_tile = tileset_ref.first_tile();
    tileset_data.tile_size = tileset.tile_size();
    tileset_data.column_count = tileset.column_count();
    tileset_data.tile_count = tileset.tile_count();

    tileset_data.image_path = tileset.texture_path();
    tileset_data.image_size = tileset.texture_size();

    convert_fancy_tiles(tileset, components, tileset_data);
  }
}

void convert_component_definitions(const ComponentIndex& manager, ir::MapData& data)
{
  for (const auto& [id, definition] : manager) {
    auto& attributes = data.component_definitions[definition.get_name()];
    for (const auto& [attr_name, attr_value] : definition) {
      attributes[attr_name] = attr_value;
    }
  }
}

}  // namespace

auto map_to_ir(const MapDocument& document) -> ir::MapData
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
    convert_component_definitions(*components, data);
  }

  convert_tilesets(document, components, data);
  convert_layers(document, components, data);
  convert_context(map, components, data.context);

  TACTILE_DEBUG_PROFILE_END("Converted document to IR")
  return data;
}

}  // namespace tactile::io