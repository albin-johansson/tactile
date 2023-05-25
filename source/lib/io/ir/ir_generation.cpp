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

#include "ir_generation.hpp"

#include "common/debug/assert.hpp"
#include "common/debug/profile.hpp"
#include "components/component.hpp"
#include "components/context.hpp"
#include "components/document.hpp"
#include "components/layer.hpp"
#include "components/map.hpp"
#include "components/object.hpp"
#include "components/texture.hpp"
#include "components/tile.hpp"
#include "components/tile_format.hpp"
#include "components/tileset.hpp"
#include "model/model.hpp"

namespace tactile {
namespace {

void _convert_layer(const Model& model,
                    Entity component_set_entity,
                    Entity layer_entity,
                    usize index,
                    LayerIR& ir_layer);

void _convert_context(const Model& model,
                      const Entity component_set_entity,
                      const Entity context_entity,
                      ContextIR& ir_context)
{
  const auto& context = model.get<Context>(context_entity);
  ir_context.properties = context.props;

  if (component_set_entity != kNullEntity) {
    const auto& component_set = model.get<ComponentSet>(component_set_entity);
    for (const auto definition_entity: component_set.definitions) {
      const auto& definition = model.get<ComponentDefinition>(definition_entity);
      ir_context.components[definition.name] = definition.attributes;
    }
  }
}

void _convert_object(const Model& model,
                     const Entity component_set_entity,
                     const Entity object_entity,
                     ObjectIR& ir_object)
{
  const auto& object = model.get<Object>(object_entity);
  TACTILE_ASSERT(object.meta_id.has_value());

  ir_object.id = object.meta_id.value();
  ir_object.type = object.type;

  ir_object.pos = object.position;
  ir_object.size = object.size;

  ir_object.tag = object.tag;
  ir_object.visible = object.visible;

  const auto& object_context = model.get<Context>(object_entity);
  ir_object.name = object_context.name;

  _convert_context(model, component_set_entity, object_entity, ir_object.context);
}

void _convert_object_layer(const Model& model,
                           const Entity component_set_entity,
                           const Entity layer_entity,
                           ObjectLayerIR& ir_object_layer)
{
  const auto& object_layer = model.get<ObjectLayer>(layer_entity);
  ir_object_layer.objects.reserve(object_layer.objects.size());

  for (const auto object_entity: object_layer.objects) {
    auto& ir_object = ir_object_layer.objects.emplace_back();
    _convert_object(model, component_set_entity, object_entity, ir_object);
  }
}

void _convert_group_layer(const Model& model,
                          const Entity component_set_entity,
                          const Entity layer_entity,
                          GroupLayerIR& ir_group)
{
  const auto& group_layer = model.get<GroupLayer>(layer_entity);
  ir_group.children.reserve(group_layer.children.size());

  usize index {0};
  for (const auto child_layer_entity: group_layer.children) {
    auto& ir_layer = ir_group.children.emplace_back(std::make_unique<LayerIR>());
    _convert_layer(model, component_set_entity, child_layer_entity, index, *ir_layer);
    ++index;
  }
}

void _convert_layer(const Model& model,
                    const Entity component_set_entity,
                    const Entity layer_entity,
                    const usize index,
                    LayerIR& ir_layer)
{
  const auto& layer = model.get<Layer>(layer_entity);
  const auto& layer_context = model.get<Context>(layer_entity);

  ir_layer.index = index;
  ir_layer.id = layer.id;
  ir_layer.type = layer.type;
  ir_layer.opacity = layer.opacity;
  ir_layer.visible = layer.visible;
  ir_layer.name = layer_context.name;

  switch (layer.type) {
    case LayerType::TileLayer: {
      const auto& tile_layer = model.get<TileLayer>(layer_entity);

      auto& ir_tile_layer = ir_layer.data.emplace<TileLayerIR>();
      ir_tile_layer.tiles = tile_layer.tiles;
      ir_tile_layer.extent.rows = tile_layer.tiles.size();
      ir_tile_layer.extent.cols = tile_layer.tiles.at(0).size();

      break;
    }
    case LayerType::ObjectLayer: {
      auto& ir_object_layer = ir_layer.data.emplace<ObjectLayerIR>();
      _convert_object_layer(model, component_set_entity, layer_entity, ir_object_layer);

      break;
    }
    case LayerType::GroupLayer: {
      auto& ir_group_layer = ir_layer.data.emplace<GroupLayerIR>();
      _convert_group_layer(model, component_set_entity, layer_entity, ir_group_layer);

      break;
    }
  }

  _convert_context(model, component_set_entity, layer_entity, ir_layer.context);
}

void _convert_layers(const Model& model,
                     const Entity component_set_entity,
                     const Map& map,
                     MapIR& ir_map)
{
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);
  usize index = 0;

  // Here we only iterate top-level layers, and convert them recursively
  for (const auto layer_entity: root_layer.children) {
    auto& ir_layer = ir_map.layers.emplace_back();
    _convert_layer(model, component_set_entity, layer_entity, index, ir_layer);

    ++index;
  }
}

void _convert_fancy_tile_animation(const TileAnimation& animation, TileIR& ir_tile)
{
  ir_tile.frames.reserve(animation.frames.size());

  for (const auto& frame: animation.frames) {
    auto& ir_frame = ir_tile.frames.emplace_back();
    ir_frame.tile_index = frame.tile_index;
    ir_frame.duration_ms = static_cast<uint64>(frame.duration.count());
  }
}

void _convert_fancy_tiles(const Model& model,
                          const Entity component_set_entity,
                          const Tileset& tileset,
                          TilesetIR& ir_tileset)
{
  for (const auto tile_entity: tileset.tiles) {
    const auto& tile = model.get<Tile>(tile_entity);
    const auto& tile_context = model.get<Context>(tile_entity);

    const auto is_animated = model.has<TileAnimation>(tile_entity);
    const auto has_objects = !tile.objects.empty();

    const auto has_props = !tile_context.props.empty();
    const auto has_comps = !tile_context.comps.empty();

    if (is_animated || has_objects || has_props || has_comps) {
      auto& ir_tile = ir_tileset.fancy_tiles[tile.index];

      if (is_animated) {
        const auto& animation = model.get<TileAnimation>(tile_entity);
        _convert_fancy_tile_animation(animation, ir_tile);
      }

      if (has_props || has_comps) {
        _convert_context(model, component_set_entity, tile_entity, ir_tile.context);
      }

      for (const auto object_entity: tile.objects) {
        auto& ir_object = ir_tile.objects.emplace_back();
        _convert_object(model, component_set_entity, object_entity, ir_object);
      }
    }
  }
}

void _convert_tilesets(const Model& model,
                       const Entity component_set_entity,
                       const Map& map,
                       MapIR& ir_map)
{
  for (const auto attached_tileset_entity: map.attached_tilesets) {
    const auto& attached_tileset = model.get<AttachedTileset>(attached_tileset_entity);

    const auto tileset_entity = attached_tileset.tileset;
    const auto& tileset = model.get<Tileset>(tileset_entity);
    const auto& tileset_context = model.get<Context>(tileset_entity);

    auto& ir_tileset = ir_map.tilesets.emplace_back();
    ir_tileset.name = tileset_context.name;

    ir_tileset.first_tile = attached_tileset.first_tile;
    ir_tileset.tile_size = tileset.tile_size;
    ir_tileset.column_count = tileset.column_count;
    ir_tileset.tile_count = static_cast<int32>(tileset.tiles.size());

    const auto& texture = model.get<Texture>(tileset.texture);
    ir_tileset.image_path = texture.path;
    ir_tileset.image_size = texture.size;

    _convert_fancy_tiles(model, component_set_entity, tileset, ir_tileset);
    _convert_context(model, component_set_entity, tileset_entity, ir_tileset.context);
  }
}

void _convert_component_definitions(const Model& model,
                                    const ComponentSet& component_set,
                                    MapIR& ir_map)
{
  for (const auto definition_entity: component_set.definitions) {
    const auto& definition = model.get<ComponentDefinition>(definition_entity);
    ir_map.component_definitions[definition.name] = definition.attributes;
  }
}

void _convert_tile_format(const TileFormat& format, TileFormatIR& ir_format)
{
  ir_format.compression = format.compression;
  ir_format.encoding = format.encoding;
  ir_format.zlib_compression_level = format.zlib_compression_level;
  ir_format.zstd_compression_level = format.zstd_compression_level;
}

}  // namespace

auto convert_map_document_to_ir(const Model& model, const Entity map_document_entity)
    -> MapIR
{
  TACTILE_DEBUG_PROFILE_START

  const auto& document = model.get<Document>(map_document_entity);
  const auto& map_document = model.get<MapDocument>(map_document_entity);
  const auto& map = model.get<Map>(map_document.map);
  const auto& map_identifiers = model.get<MapIdentifiers>(map_document.map);
  const auto& tile_format = model.get<TileFormat>(map_document.map);

  MapIR ir_map;
  ir_map.extent = map.extent;
  ir_map.tile_size = map.tile_size;
  ir_map.next_object_id = map_identifiers.next_object_id;
  ir_map.next_layer_id = map_identifiers.next_layer_id;

  _convert_tile_format(tile_format, ir_map.tile_format);

  if (const auto* component_set = model.try_get<ComponentSet>(document.component_set)) {
    _convert_component_definitions(model, *component_set, ir_map);
  }

  _convert_tilesets(model, document.component_set, map, ir_map);
  _convert_layers(model, document.component_set, map, ir_map);

  _convert_context(model, document.component_set, map_document.map, ir_map.context);

  TACTILE_DEBUG_PROFILE_END("Converted document to IR")
  return ir_map;
}

}  // namespace tactile