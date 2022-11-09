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
#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "meta/profile.hpp"
#include "misc/assert.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"

namespace tactile::io {
namespace {

void convert_context(const Context& context,
                     const ComponentIndex* components,
                     ir::ContextData& data)
{
  for (const auto& [name, property]: context.ctx().props()) {
    data.properties[name] = property;
  }

  if (components) {
    for (const auto& [component_id, component]: context.ctx().comps()) {
      const auto& definition = components->at(component.definition_id());
      const auto& name = definition.name();

      auto& attributes = data.components[name];
      for (const auto& [attr_name, attr_value]: component) {
        attributes[attr_name] = attr_value;
      }
    }
  }
}

void convert_object(const Object& object,
                    const ComponentIndex* components,
                    ir::ObjectData& data)
{
  TACTILE_ASSERT(object.meta_id().has_value());

  data.id = object.meta_id().value();
  data.type = object.type();

  data.pos = object.pos();
  data.size = object.size();

  data.tag = object.tag();
  data.visible = object.visible();

  data.name = object.ctx().name();
  convert_context(object, components, data.context);
}

void convert_layer(const Layer& layer,
                   usize index,
                   const ComponentIndex* components,
                   ir::LayerData& data);

void convert_object_layer(const ObjectLayer& layer,
                          const ComponentIndex* index,
                          ir::ObjectLayerData& data)
{
  data.objects.reserve(layer.object_count());
  for (const auto& [id, object]: layer) {
    auto& object_data = data.objects.emplace_back();
    convert_object(*object, index, object_data);
  }
}

void convert_group_layer(const GroupLayer& layer,
                         const ComponentIndex* components,
                         ir::GroupLayerData& data)
{
  data.children.reserve(layer.storage().size());

  usize index {0};
  for (const auto& immediate_child: layer.storage()) {
    TACTILE_ASSERT(immediate_child->parent() == layer.uuid());

    auto& layer_data = data.children.emplace_back(std::make_unique<ir::LayerData>());
    convert_layer(*immediate_child, index, components, *layer_data);
    ++index;
  }
}

void convert_layer(const Layer& layer,
                   const usize index,
                   const ComponentIndex* components,
                   ir::LayerData& layer_data)
{
  TACTILE_ASSERT(layer.meta_id().has_value());
  layer_data.index = index;

  layer_data.id = layer.meta_id().value();
  layer_data.type = layer.type();
  layer_data.opacity = layer.opacity();
  layer_data.visible = layer.visible();

  layer_data.name = layer.ctx().name();

  switch (layer.type()) {
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
      auto& object_layer_data = layer_data.data.emplace<ir::ObjectLayerData>();
      convert_object_layer(object_layer, components, object_layer_data);
      break;
    }
    case LayerType::GroupLayer: {
      const auto& group_layer = dynamic_cast<const GroupLayer&>(layer);
      auto& group_layer_data = layer_data.data.emplace<ir::GroupLayerData>();
      convert_group_layer(group_layer, components, group_layer_data);
      break;
    }
  }

  convert_context(layer, components, layer_data.context);
}

void convert_layers(const MapDocument& document,
                    const ComponentIndex* components,
                    ir::MapData& data)
{
  usize index = 0;

  const auto& root = document.get_map().invisible_root();
  root.each([&](const Layer* layer) {
    // Only iterate top-level layers, and convert them recursively
    if (!layer->parent()) {
      auto& layer_data = data.layers.emplace_back();
      convert_layer(*layer, index, components, layer_data);
      ++index;
    }
  });
}

void convert_fancy_tile_animation(const TileAnimation& animation, ir::MetaTileData& data)
{
  data.frames.reserve(animation.size());

  for (const auto& frame: animation) {
    auto& frame_data = data.frames.emplace_back();
    frame_data.tile_index = frame.tile;
    frame_data.duration_ms = static_cast<uint64>(frame.duration.count());
  }
}

void convert_fancy_tiles(const Tileset& tileset,
                         const ComponentIndex* components,
                         ir::TilesetData& data)
{
  for (const auto& [id, tile]: tileset) {
    const auto is_animated = tile->is_animated();
    const auto has_objects = tile->object_count() != 0;
    const auto has_props = !tile->ctx().props().empty();
    const auto has_comps = !tile->ctx().comps().empty();

    if (is_animated || has_objects || has_props || has_comps) {
      auto& tile_data = data.fancy_tiles[tile->index()];

      if (is_animated) {
        convert_fancy_tile_animation(tile->animation(), tile_data);
      }

      if (has_props || has_comps) {
        convert_context(*tile, components, tile_data.context);
      }

      if (has_objects) {
        for (const auto& [objectId, object]: tile->get_objects()) {
          auto& object_data = tile_data.objects.emplace_back();
          convert_object(*object, components, object_data);
        }
      }
    }
  }
}

void convert_tilesets(const MapDocument& document,
                      const ComponentIndex* components,
                      ir::MapData& data)
{
  const auto& map = document.get_map();
  for (const auto& [tileset_id, tileset_ref]: map.tileset_bundle()) {
    const auto& tileset = tileset_ref.view_tileset();

    auto& tileset_data = data.tilesets.emplace_back();
    tileset_data.name = tileset.ctx().name();

    tileset_data.first_tile = tileset_ref.first_tile();
    tileset_data.tile_size = tileset.tile_size();
    tileset_data.column_count = tileset.column_count();
    tileset_data.tile_count = tileset.tile_count();

    const auto& texture = tileset.texture();
    tileset_data.image_path = texture.path();
    tileset_data.image_size = texture.size();

    convert_fancy_tiles(tileset, components, tileset_data);
    convert_context(tileset, components, tileset_data.context);
  }
}

void convert_component_definitions(const ComponentIndex& manager, ir::MapData& data)
{
  for (const auto& [id, definition]: manager) {
    auto& attributes = data.component_definitions[definition.name()];
    for (const auto& [attr_name, attr_value]: definition) {
      attributes[attr_name] = attr_value;
    }
  }
}

void convert_tile_format(const TileFormat& format, ir::TileFormatData& data)
{
  data.compression = format.compression();
  data.encoding = format.encoding();
  data.zlib_compression_level = format.zlib_compression_level();
  data.zstd_compression_level = format.zstd_compression_level();
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

  convert_tile_format(map.tile_format(), data.tile_format);

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