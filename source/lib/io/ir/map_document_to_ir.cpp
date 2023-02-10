/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "map_document_to_ir.hpp"

#include "common/debug/assert.hpp"
#include "common/debug/profile.hpp"
#include "core/component/component_index.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"

namespace tactile {
namespace {

void convert_context(const Context& context,
                     const ComponentIndex* component_index,
                     ContextIR& ir_context)
{
  const auto& ctx = context.get_ctx();

  ctx.each_property([&](const String& property_name, const Attribute& property_value) {
    ir_context.properties[property_name] = property_value;
  });

  if (component_index) {
    ctx.each_component([&](const UUID&, const Component& component) {
      const auto& component_def = component_index->get_comp(component.definition_id());
      const auto& component_name = component_def.get_name();

      auto& component_attributes = ir_context.components[component_name];
      for (const auto& [attr_name, attr_value]: component) {
        component_attributes[attr_name] = attr_value;
      }
    });
  }
}

void convert_object(const Object& object,
                    const ComponentIndex* component_index,
                    ObjectIR& ir_object)
{
  TACTILE_ASSERT(object.get_meta_id().has_value());

  ir_object.id = object.get_meta_id().value();
  ir_object.type = object.get_type();

  ir_object.pos = object.get_pos();
  ir_object.size = object.get_size();

  ir_object.tag = object.get_tag();
  ir_object.visible = object.is_visible();

  ir_object.name = object.get_ctx().name();
  convert_context(object, component_index, ir_object.context);
}

void convert_layer(const Layer& layer,
                   usize index,
                   const ComponentIndex* component_index,
                   LayerIR& ir_layer);

void convert_object_layer(const ObjectLayer& object_layer,
                          const ComponentIndex* component_index,
                          ObjectLayerIR& ir_object_layer)
{
  ir_object_layer.objects.reserve(object_layer.object_count());
  for (const auto& [object_id, object]: object_layer) {
    auto& ir_object = ir_object_layer.objects.emplace_back();
    convert_object(*object, component_index, ir_object);
  }
}

void convert_group_layer(const GroupLayer& group,
                         const ComponentIndex* component_index,
                         GroupLayerIR& ir_group)
{
  ir_group.children.reserve(group.get_storage().size());

  usize index {0};
  for (const auto& immediate_child: group.get_storage()) {
    TACTILE_ASSERT(immediate_child->get_parent() == group.get_uuid());

    auto& layer_data = ir_group.children.emplace_back(std::make_unique<LayerIR>());
    convert_layer(*immediate_child, index, component_index, *layer_data);
    ++index;
  }
}

void convert_layer(const Layer& layer,
                   const usize index,
                   const ComponentIndex* component_index,
                   LayerIR& ir_layer)
{
  TACTILE_ASSERT(layer.get_meta_id().has_value());
  ir_layer.index = index;

  ir_layer.id = layer.get_meta_id().value();
  ir_layer.type = layer.get_type();
  ir_layer.opacity = layer.get_opacity();
  ir_layer.visible = layer.is_visible();

  ir_layer.name = layer.get_ctx().name();

  switch (layer.get_type()) {
    case LayerType::TileLayer: {
      const auto& tile_layer = dynamic_cast<const TileLayer&>(layer);

      auto& ir_tile_layer = ir_layer.data.emplace<TileLayerIR>();
      ir_tile_layer.tiles = tile_layer.get_tiles();
      ir_tile_layer.extent.rows = tile_layer.row_count();
      ir_tile_layer.extent.cols = tile_layer.column_count();

      break;
    }
    case LayerType::ObjectLayer: {
      const auto& object_layer = dynamic_cast<const ObjectLayer&>(layer);

      auto& ir_object_layer = ir_layer.data.emplace<ObjectLayerIR>();
      convert_object_layer(object_layer, component_index, ir_object_layer);
      break;
    }
    case LayerType::GroupLayer: {
      const auto& group_layer = dynamic_cast<const GroupLayer&>(layer);

      auto& ir_group_layer = ir_layer.data.emplace<GroupLayerIR>();
      convert_group_layer(group_layer, component_index, ir_group_layer);
      break;
    }
  }

  convert_context(layer, component_index, ir_layer.context);
}

void convert_layers(const MapDocument& document,
                    const ComponentIndex* component_index,
                    MapIR& ir_map)
{
  usize index = 0;

  const auto& root = document.get_map().get_invisible_root();
  root.each([&](const Layer& layer) {
    // Only iterate top-level layers, and convert them recursively
    if (!layer.get_parent()) {
      auto& layer_data = ir_map.layers.emplace_back();
      convert_layer(layer, index, component_index, layer_data);
      ++index;
    }
  });
}

void convert_fancy_tile_animation(const TileAnimation& animation, TileIR& ir_tile)
{
  ir_tile.frames.reserve(animation.size());

  for (const auto& frame: animation) {
    auto& ir_frame = ir_tile.frames.emplace_back();
    ir_frame.tile_index = frame.tile;
    ir_frame.duration_ms = static_cast<uint64>(frame.duration.count());
  }
}

void convert_fancy_tiles(const Tileset& tileset,
                         const ComponentIndex* component_index,
                         TilesetIR& ir_tileset)
{
  for (const auto& [tile_id, tile]: tileset) {
    const auto is_animated = tile->is_animated();
    const auto has_objects = tile->object_count() != 0;

    const auto& tile_ctx = tile->get_ctx();
    const auto has_props = tile_ctx.property_count() > 0;
    const auto has_comps = tile_ctx.component_count() > 0;

    if (is_animated || has_objects || has_props || has_comps) {
      auto& ir_tile = ir_tileset.fancy_tiles[tile->get_index()];

      if (is_animated) {
        convert_fancy_tile_animation(tile->get_animation(), ir_tile);
      }

      if (has_props || has_comps) {
        convert_context(*tile, component_index, ir_tile.context);
      }

      if (has_objects) {
        for (const auto& [object_id, object]: tile->get_objects()) {
          auto& ir_object = ir_tile.objects.emplace_back();
          convert_object(*object, component_index, ir_object);
        }
      }
    }
  }
}

void convert_tilesets(const MapDocument& document,
                      const ComponentIndex* component_index,
                      MapIR& ir_map)
{
  const auto& map = document.get_map();
  for (const auto& [tileset_id, tileset_ref]: map.get_tileset_bundle()) {
    const auto& tileset = tileset_ref.get_tileset();

    auto& ir_tileset = ir_map.tilesets.emplace_back();
    ir_tileset.name = tileset.get_ctx().name();

    ir_tileset.first_tile = tileset_ref.get_first_tile();
    ir_tileset.tile_size = tileset.tile_size();
    ir_tileset.column_count = tileset.column_count();
    ir_tileset.tile_count = tileset.tile_count();

    const auto& texture = tileset.texture();
    ir_tileset.image_path = texture.get_path();
    ir_tileset.image_size = texture.get_size();

    convert_fancy_tiles(tileset, component_index, ir_tileset);
    convert_context(tileset, component_index, ir_tileset.context);
  }
}

void convert_component_definitions(const ComponentIndex& component_index, MapIR& ir_map)
{
  for (const auto& [component_id, component_def]: component_index) {
    auto& attributes = ir_map.component_definitions[component_def.get_name()];
    for (const auto& [attr_name, attr_value]: component_def) {
      attributes[attr_name] = attr_value;
    }
  }
}

void convert_tile_format(const TileFormat& format, TileFormatIR& ir_format)
{
  ir_format.compression = format.compression();
  ir_format.encoding = format.encoding();
  ir_format.zlib_compression_level = format.zlib_compression_level();
  ir_format.zstd_compression_level = format.zstd_compression_level();
}

}  // namespace

auto convert_map_document_to_ir(const MapDocument& document) -> MapIR
{
  TACTILE_DEBUG_PROFILE_START
  const auto& map = document.get_map();

  MapIR ir_map;
  ir_map.extent = map.get_extent();
  ir_map.tile_size = map.get_tile_size();
  ir_map.next_object_id = map.next_object_id();
  ir_map.next_layer_id = map.next_layer_id();

  convert_tile_format(map.get_tile_format(), ir_map.tile_format);

  const auto* components = document.find_component_index();

  if (components) {
    convert_component_definitions(*components, ir_map);
  }

  convert_tilesets(document, components, ir_map);
  convert_layers(document, components, ir_map);
  convert_context(map, components, ir_map.context);

  TACTILE_DEBUG_PROFILE_END("Converted document to IR")
  return ir_map;
}

}  // namespace tactile