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
#include "model/documents/document_components.hpp"
#include "model/entity_validation.hpp"
#include "model/layers/layer_components.hpp"
#include "model/objects/object_components.hpp"
#include "model/textures/texture_components.hpp"

namespace tactile {

void convert_context_to_ir(const Model& model,
                           const Entity component_set_entity,
                           const Context& context,
                           ContextIR& ir_context)
{
  ir_context.properties = context.props;

  if (component_set_entity != kNullEntity) {
    const auto& component_set = model.get<ComponentSet>(component_set_entity);

    for (const auto component_entity: component_set.definitions) {
      const auto& component = model.get<Component>(component_entity);
      ir_context.components[component.name] = component.attributes;
    }
  }
}

void convert_object_to_ir(const Model& model,
                          const Entity component_set_entity,
                          const Entity object_entity,
                          ObjectIR& ir_object)
{
  TACTILE_ASSERT(sys::is_object_entity(model, object_entity));

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

  convert_context_to_ir(model, component_set_entity, object_context, ir_object.context);
}

void convert_tile_layer_to_ir(const Model& model,
                              const Entity layer_entity,
                              TileLayerIR& ir_tile_layer)
{
  const auto& tile_layer = model.get<TileLayer>(layer_entity);

  ir_tile_layer.tiles = tile_layer.tiles;
  ir_tile_layer.extent.rows = tile_layer.tiles.size();
  ir_tile_layer.extent.cols = tile_layer.tiles.at(0).size();
}

void convert_object_layer_to_ir(const Model& model,
                                const Entity component_set_entity,
                                const Entity layer_entity,
                                ObjectLayerIR& ir_object_layer)
{
  TACTILE_ASSERT(sys::is_layer_entity(model, layer_entity));

  const auto& object_layer = model.get<ObjectLayer>(layer_entity);
  ir_object_layer.objects.reserve(object_layer.objects.size());

  for (const auto object_entity: object_layer.objects) {
    auto& ir_object = ir_object_layer.objects.emplace_back();
    convert_object_to_ir(model, component_set_entity, object_entity, ir_object);
  }
}

void convert_group_layer_to_ir(const Model& model,
                               const Entity component_set_entity,
                               const Entity layer_entity,
                               GroupLayerIR& ir_group)
{
  TACTILE_ASSERT(sys::is_layer_entity(model, layer_entity));

  const auto& group_layer = model.get<GroupLayer>(layer_entity);
  ir_group.children.reserve(group_layer.children.size());

  usize index {0};
  for (const auto child_layer_entity: group_layer.children) {
    auto& ir_layer = ir_group.children.emplace_back(std::make_unique<LayerIR>());
    convert_layer_to_ir(model,
                        component_set_entity,
                        child_layer_entity,
                        index,
                        *ir_layer);
    ++index;
  }
}

void convert_layer_to_ir(const Model& model,
                         const Entity component_set_entity,
                         const Entity layer_entity,
                         const usize index,
                         LayerIR& ir_layer)
{
  TACTILE_ASSERT(sys::is_layer_entity(model, layer_entity));

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
      auto& ir_tile_layer = ir_layer.data.emplace<TileLayerIR>();
      convert_tile_layer_to_ir(model, layer_entity, ir_tile_layer);
      break;
    }
    case LayerType::ObjectLayer: {
      auto& ir_object_layer = ir_layer.data.emplace<ObjectLayerIR>();
      convert_object_layer_to_ir(model,
                                 component_set_entity,
                                 layer_entity,
                                 ir_object_layer);
      break;
    }
    case LayerType::GroupLayer: {
      auto& ir_group = ir_layer.data.emplace<GroupLayerIR>();
      convert_group_layer_to_ir(model, component_set_entity, layer_entity, ir_group);
      break;
    }
  }

  convert_context_to_ir(model, component_set_entity, layer_context, ir_layer.context);
}

void convert_layers_to_ir(const Model& model,
                          const Entity component_set_entity,
                          const Map& map,
                          MapIR& ir_map)
{
  const auto& root_layer = model.get<GroupLayer>(map.root_layer);
  usize index = 0;

  // Here we only iterate top-level layers, and convert them recursively
  for (const auto layer_entity: root_layer.children) {
    auto& ir_layer = ir_map.layers.emplace_back();
    convert_layer_to_ir(model, component_set_entity, layer_entity, index, ir_layer);

    ++index;
  }
}

void convert_tile_animation_to_ir(const TileAnimation& animation, TileIR& ir_tile)
{
  ir_tile.frames.reserve(animation.frames.size());

  for (const auto& frame: animation.frames) {
    auto& ir_frame = ir_tile.frames.emplace_back();
    ir_frame.tile_index = frame.tile_index;
    ir_frame.duration_ms = static_cast<uint64>(frame.duration.count());
  }
}

void convert_tiles_to_ir(const Model& model,
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
        convert_tile_animation_to_ir(animation, ir_tile);
      }

      if (has_props || has_comps) {
        convert_context_to_ir(model, component_set_entity, tile_context, ir_tile.context);
      }

      for (const auto object_entity: tile.objects) {
        auto& ir_object = ir_tile.objects.emplace_back();
        convert_object_to_ir(model, component_set_entity, object_entity, ir_object);
      }
    }
  }
}

void convert_tileset_to_ir(const Model& model,
                           const Entity component_set_entity,
                           const Entity attached_tileset_entity,
                           TilesetIR& ir_tileset)
{
  TACTILE_ASSERT(sys::is_attached_tileset_entity(model, attached_tileset_entity));
  const auto& attached_tileset = model.get<AttachedTileset>(attached_tileset_entity);

  const auto tileset_entity = attached_tileset.tileset;
  const auto& tileset = model.get<Tileset>(tileset_entity);
  const auto& tileset_context = model.get<Context>(tileset_entity);

  ir_tileset.name = tileset_context.name;

  ir_tileset.first_tile = attached_tileset.first_tile;
  ir_tileset.tile_size = tileset.tile_size;
  ir_tileset.column_count = tileset.column_count;
  ir_tileset.tile_count = static_cast<int32>(tileset.tiles.size());

  const auto& texture = model.get<Texture>(tileset.texture);
  ir_tileset.image_path = texture.path;
  ir_tileset.image_size = texture.size;

  convert_tiles_to_ir(model, component_set_entity, tileset, ir_tileset);
  convert_context_to_ir(model, component_set_entity, tileset_context, ir_tileset.context);
}

void convert_tilesets_to_ir(const Model& model,
                            const Entity component_set_entity,
                            const Map& map,
                            MapIR& ir_map)
{
  for (const auto attached_tileset_entity: map.attached_tilesets) {
    TilesetIR ir_tileset;
    convert_tileset_to_ir(model,
                          component_set_entity,
                          attached_tileset_entity,
                          ir_tileset);
    ir_map.tilesets.push_back(std::move(ir_tileset));
  }
}

void convert_components_to_ir(const Model& model,
                              const ComponentSet& component_set,
                              ComponentMap& ir_components)
{
  for (const auto component_entity: component_set.definitions) {
    const auto& component = model.get<Component>(component_entity);
    ir_components[component.name] = component.attributes;
  }
}

void convert_tile_format_to_ir(const TileFormat& format, TileFormatIR& ir_format)
{
  ir_format.compression = format.compression;
  ir_format.encoding = format.encoding;
  ir_format.zlib_compression_level = format.zlib_compression_level;
  ir_format.zstd_compression_level = format.zstd_compression_level;
}

auto convert_map_document_to_ir(const Model& model, const Entity map_document_entity)
    -> MapIR
{
  TACTILE_DEBUG_PROFILE_START

  const auto& document = model.get<Document>(map_document_entity);
  const auto& map_document = model.get<MapDocument>(map_document_entity);
  const auto& map = model.get<Map>(map_document.map);
  const auto& map_context = model.get<Context>(map_document.map);
  const auto& map_identifiers = model.get<MapIdentifiers>(map_document.map);
  const auto& tile_format = model.get<TileFormat>(map_document.map);

  MapIR ir_map;
  ir_map.extent = map.extent;
  ir_map.tile_size = map.tile_size;
  ir_map.next_object_id = map_identifiers.next_object_id;
  ir_map.next_layer_id = map_identifiers.next_layer_id;

  convert_tile_format_to_ir(tile_format, ir_map.tile_format);

  if (const auto* component_set = model.try_get<ComponentSet>(document.component_set)) {
    convert_components_to_ir(model, *component_set, ir_map.component_definitions);
  }

  convert_tilesets_to_ir(model, document.component_set, map, ir_map);
  convert_layers_to_ir(model, document.component_set, map, ir_map);

  convert_context_to_ir(model, document.component_set, map_context, ir_map.context);

  TACTILE_DEBUG_PROFILE_END("Converted document to IR")
  return ir_map;
}

}  // namespace tactile