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

#include "ir_restoration.hpp"

#include <algorithm>  // max
#include <utility>    // move

#include "cmd/command_stack.hpp"
#include "common/debug/assert.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/uuid.hpp"
#include "common/util/functional.hpp"
#include "core/component.hpp"
#include "core/context.hpp"
#include "core/layer.hpp"
#include "core/map.hpp"
#include "core/object.hpp"
#include "core/tile.hpp"
#include "core/tile_format.hpp"
#include "core/tileset.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/texture_loader.hpp"
#include "model/document.hpp"
#include "model/model.hpp"
#include "model/systems/component_system.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/layer_system.hpp"

namespace tactile {
namespace {

void restore_context(Model& model,
                     const Entity component_set_entity,
                     const Entity context_entity,
                     const ContextIR& ir_context)
{
  auto& context = model.add<Context>(context_entity);
  context.props.reserve(ir_context.properties.size());
  context.comps.reserve(ir_context.components.size());

  for (const auto& [property_name, property_value]: ir_context.properties) {
    context.props[property_name] = property_value;
  }

  if (auto* component_set = model.try_get<ComponentSet>(component_set_entity)) {
    for (const auto& [component_type_name, attributes]: ir_context.components) {
      const auto component_definition_entity =
          sys::find_component_definition(model, *component_set, component_type_name);
      TACTILE_ASSERT(component_definition_entity != kNullEntity);

      const auto component_entity =
          sys::create_component(model, component_definition_entity);
      context.comps.push_back(component_entity);

      auto& component = model.get<Component>(component_entity);
      component.attributes = attributes;
    }
  }
}

auto restore_object(Model& model,
                    const Entity component_set_entity,
                    const ObjectIR& ir_object) -> Entity
{
  const auto object_entity = model.create_entity();

  auto& object = model.add<Object>(object_entity);
  object.type = ir_object.type;
  object.position = ir_object.pos;
  object.size = ir_object.size;
  object.tag = ir_object.tag;
  object.visible = ir_object.visible;
  object.meta_id = ir_object.id;

  restore_context(model, component_set_entity, object_entity, ir_object.context);

  auto& object_context = model.get<Context>(object_entity);
  object_context.name = ir_object.name;

  return object_entity;
}

void restore_object_layer(Model& model,
                          const Entity component_set_entity,
                          const Entity layer_entity,
                          const ObjectLayerIR& ir_object_layer)
{
  auto& object_layer = model.get<ObjectLayer>(layer_entity);
  object_layer.objects.reserve(ir_object_layer.objects.size());

  for (const auto& ir_object: ir_object_layer.objects) {
    object_layer.objects.push_back(
        restore_object(model, component_set_entity, ir_object));
  }
}

auto restore_layer(Model& model,
                   const Entity component_set_entity,
                   const Entity map_entity,
                   const LayerIR& ir_layer,
                   const Entity parent_layer_entity = kNullEntity) -> Entity
{
  auto& map = model.get<Map>(map_entity);

  // TODO respect layerData.index?

  Entity layer_entity = kNullEntity;

  switch (ir_layer.type) {
    case LayerType::TileLayer: {
      const auto& ir_tile_layer = ir_layer.as_tile_layer();
      layer_entity = sys::create_tile_layer(model, ir_layer.id, ir_tile_layer.extent);

      auto& tile_layer = model.get<TileLayer>(layer_entity);
      tile_layer.tiles = ir_tile_layer.tiles;

      break;
    }
    case LayerType::ObjectLayer: {
      layer_entity = sys::create_object_layer(model, ir_layer.id);
      restore_object_layer(model,
                           component_set_entity,
                           layer_entity,
                           ir_layer.as_object_layer());
      break;
    }
    case LayerType::GroupLayer: {
      layer_entity = sys::create_group_layer(model, ir_layer.id);

      const auto& ir_group = ir_layer.as_group_layer();
      for (const auto& ir_child_layer: ir_group.children) {
        restore_layer(model,
                      component_set_entity,
                      map_entity,
                      *ir_child_layer,
                      layer_entity);
      }

      break;
    }
    default:
      throw TactileError {"Invalid layer type!"};
  }

  if (parent_layer_entity != kNullEntity) {
    auto& parent_layer = model.get<CGroupLayer>(parent_layer_entity);
    parent_layer.children.push_back(layer_entity);
  }

  auto& layer = model.get<Layer>(layer_entity);
  layer.opacity = ir_layer.opacity;
  layer.visible = ir_layer.visible;

  restore_context(model, component_set_entity, layer_entity, ir_layer.context);

  auto& layer_context = model.get<Context>(layer_entity);
  layer_context.name = ir_layer.name;

  return layer_entity;
}

void restore_layers(Model& model,
                    const Entity component_set_entity,
                    const Entity map_entity,
                    const MapIR& ir_map)
{
  for (const auto& ir_layer: ir_map.layers) {
    restore_layer(model, component_set_entity, map_entity, ir_layer);
  }
}

void restore_tile_animation(Model& model, Tile& tile, const TileIR& ir_tile)
{
  tile.animation = model.create_entity();

  auto& animation = model.add<TileAnimation>(tile.animation);
  animation.frames.reserve(ir_tile.frames.size());

  for (const auto& ir_frame: ir_tile.frames) {
    auto& frame = animation.frames.emplace_back();
    frame.tile_index = ir_frame.tile_index;
    frame.duration = ms_t {ir_frame.duration_ms};
  }
}

void restore_fancy_tile_objects(Model& model,
                                Tile& tile,
                                const Entity component_set_entity,
                                const TileIR& ir_tile)
{
  tile.objects.reserve(ir_tile.objects.size());

  for (const auto& ir_object: ir_tile.objects) {
    tile.objects.push_back(restore_object(model, component_set_entity, ir_object));
  }
}

void restore_fancy_tiles(Model& model,
                         const Entity component_set_entity,
                         const Entity tileset_entity,
                         const TilesetIR& ir_tileset)
{
  auto& tileset = model.get<Tileset>(tileset_entity);

  for (const auto& [index, ir_tile]: ir_tileset.fancy_tiles) {
    const auto [row, col] = to_matrix_coords(index, ir_tileset.column_count);
    const auto tile_entity = model.create_entity();

    auto& tile_context = model.add<Context>(tile_entity);

    auto& tile = model.add<Tile>(tile_entity);
    tile.index = index;
    tile.source = {Int2 {col * ir_tileset.tile_size.x, row * ir_tileset.tile_size.y},
                   ir_tileset.tile_size};

    if (!ir_tile.frames.empty()) {
      restore_tile_animation(model, tile, ir_tile);
    }

    if (!ir_tile.objects.empty()) {
      restore_fancy_tile_objects(model, tile, component_set_entity, ir_tile);
    }

    restore_context(model, component_set_entity, tile_entity, ir_tile.context);
  }
}

auto restore_tileset_document(Model& model,
                              const Entity component_set_entity,
                              const TilesetIR& ir_tileset) -> Entity
{
  //  TACTILE_ASSERT(model.get_active_document_id().has_value());

  // TODO compare tileset document absolute paths to recognize the same tileset being
  // loaded multiple times

  const TilesetInfo info {
      .texture = load_texture(ir_tileset.image_path),
      .tile_size = ir_tileset.tile_size,
  };

  const auto document_entity = model.create_entity();

  auto& document = model.add<Document>(document_entity);
  document.type = DocumentType::Tileset;
  document.component_set = component_set_entity;

  auto& tileset_document = model.add<TilesetDocument>(document_entity);
  tileset_document.tileset = model.create_entity();

  {
    auto& tileset_context = model.add<Context>(tileset_document.tileset);
    tileset_context.name = ir_tileset.name;

    auto& tileset = model.add<Tileset>(tileset_document.tileset);
    tileset.tile_size = ir_tileset.tile_size;
    tileset.uv_size = Float2 {tileset.tile_size} / Float2 {ir_tileset.image_size};
    tileset.row_count = ir_tileset.image_size.y / tileset.tile_size.y;
    tileset.column_count = ir_tileset.column_count;
    tileset.texture = kNullEntity;  // FIXME;
  }

  restore_fancy_tiles(model, component_set_entity, document_entity, ir_tileset);

  restore_context(model,
                  component_set_entity,
                  tileset_document.tileset,
                  ir_tileset.context);

  return document_entity;
}

void attach_tileset_to_map(Model& model,
                           Map& map,
                           const Entity tileset_entity,
                           const TilesetIR& ir_tileset)
{
  const auto attached_tileset_entity = model.create_entity();
  map.attached_tilesets.push_back(attached_tileset_entity);

  auto& attached_tileset = model.add<AttachedTileset>(attached_tileset_entity);
  attached_tileset.tileset = tileset_entity;
  attached_tileset.first_tile = ir_tileset.first_tile;
  attached_tileset.last_tile = ir_tileset.first_tile + ir_tileset.tile_count;
  attached_tileset.embedded = false;

  auto& tileset_viewport = model.add<Viewport>(attached_tileset_entity);
  tileset_viewport.offset = Float2 {0, 0};
  tileset_viewport.tile_size = ir_tileset.tile_size;
}

void restore_tileset_documents(Model& model,
                               Map& map,
                               const Entity component_set_entity,
                               const MapIR& ir_map)
{
  for (const auto& ir_tileset: ir_map.tilesets) {
    const auto tileset_document_entity =
        restore_tileset_document(model, component_set_entity, ir_tileset);

    const auto& tileset_document = model.get<TilesetDocument>(tileset_document_entity);
    attach_tileset_to_map(model, map, tileset_document.tileset, ir_tileset);
  }

  // Determine the next available tile identifier
  map.next_tile_id = 1;
  for (const auto attached_tileset_entity: map.attached_tilesets) {
    const auto& attached_tileset = model.get<AttachedTileset>(attached_tileset_entity);
    map.next_tile_id = std::max(map.next_tile_id, attached_tileset.last_tile + 1);
  }

  if (!map.attached_tilesets.empty()) {
    map.active_tileset = map.attached_tilesets.at(0);
  }
}

void restore_component_definitions(Model& model,
                                   ComponentSet& component_set,
                                   const MapIR& ir_map)
{
  for (const auto& [name, attributes]: ir_map.component_definitions) {
    const auto component_def_entity = model.create_entity();
    component_set.definitions.push_back(component_def_entity);

    auto& component_def = model.add<ComponentDefinition>(component_def_entity);
    for (const auto& [attr_name, attr_value]: attributes) {
      component_def.attributes[attr_name] = attr_value;
    }
  }
}

void restore_tile_format(CTileFormat& format, const TileFormatIR& ir_format)
{
  format.encoding = ir_format.encoding;
  format.compression = ir_format.compression;

  if (ir_format.zlib_compression_level) {
    format.zlib_compression_level = *ir_format.zlib_compression_level;
  }

  if (ir_format.zstd_compression_level) {
    format.zstd_compression_level = *ir_format.zstd_compression_level;
  }
}

}  // namespace

// TODO rename to "restore_ir_map"
void create_map_document_from_ir(const MapIR& ir_map,
                                 const Path& document_path,
                                 Model& model)
{
  const auto absolute_document_path = fs::absolute(document_path);
  const auto document_entity =
      sys::create_map_document(model, ir_map.extent, ir_map.tile_size);

  sys::select_document(model, document_entity);

  auto& document = model.get<Document>(document_entity);
  document.component_set = model.create_entity();
  document.path = absolute_document_path;

  auto& component_set = model.add<ComponentSet>(document.component_set);

  auto& map_document = model.get<MapDocument>(document_entity);

  auto& map_context = model.get<Context>(map_document.map);
  map_context.name = absolute_document_path.filename().string();

  auto& map = model.get<Map>(map_document.map);
  map.extent = ir_map.extent;
  map.tile_size = ir_map.tile_size;
  map.next_layer_id = ir_map.next_layer_id;
  map.next_object_id = ir_map.next_object_id;

  auto& tile_format = model.get<CTileFormat>(map_document.map);

  restore_tile_format(tile_format, ir_map.tile_format);
  restore_component_definitions(model, component_set, ir_map);
  restore_tileset_documents(model, map, document.component_set, ir_map);
  restore_layers(model, document.component_set, map_document.map, ir_map);
  restore_context(model, document.component_set, map_document.map, ir_map.context);
}

}  // namespace tactile