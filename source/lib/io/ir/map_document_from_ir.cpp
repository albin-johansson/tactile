// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "map_document_from_ir.hpp"

#include <algorithm>  // max
#include <utility>    // move

#include <entt/entity/registry.hpp>

#include "common/util/functional.hpp"
#include "core/component/component_index.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tile_pos.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/tile/tileset_info.hpp"
#include "io/ir/map/map_ir.hpp"
#include "io/load_texture.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/document/document.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {
namespace {

void restore_context_no_register(Document& document,
                                 const Shared<Context>& context,
                                 const ContextIR& ir_context)
{
  auto& ctx = context->get_ctx();

  for (const auto& [property_name, property_value]: ir_context.properties) {
    ctx.add_property(property_name, property_value);
  }

  if (auto component_index = document.get_component_index_ptr()) {
    for (const auto& [type, attributes]: ir_context.components) {
      const auto& component_def = component_index->get_comp(type);

      auto component = component_def.instantiate();
      for (const auto& [attr_name, attr_value]: attributes) {
        component.update_attr(attr_name, attr_value);
      }

      ctx.attach_component(std::move(component));
    }
  }
}

void restore_context(Document& document,
                     const Shared<Context>& context,
                     const ContextIR& ir_context)
{
  restore_context_no_register(document, context, ir_context);
  document.get_contexts().add_context(context);
}

auto restore_object(Document& document, const ObjectIR& ir_object) -> Shared<Object>
{
  auto object = std::make_shared<Object>();
  object->set_type(ir_object.type);

  object->set_pos(ir_object.pos);
  object->set_size(ir_object.size);

  object->set_tag(ir_object.tag);
  object->set_visible(ir_object.visible);

  object->get_ctx().set_name(ir_object.name);
  object->set_meta_id(ir_object.id);

  restore_context(document, object, ir_object.context);

  return object;
}

void restore_object_layer(MapDocument& document,
                          const UUID& layer_id,
                          const ObjectLayerIR& ir_object_layer)
{
  auto& map = document.get_map();

  auto& layer = map.get_invisible_root().get_object_layer(layer_id);
  layer.reserve_objects(ir_object_layer.objects.size());

  for (const auto& ir_object: ir_object_layer.objects) {
    layer.add_object(restore_object(document, ir_object));
  }
}

auto restore_layer(MapDocument& document,
                   const LayerIR& ir_layer,
                   const Maybe<UUID>& parent = nothing) -> UUID
{
  auto& map = document.get_map();
  auto& root_layer = map.get_invisible_root();

  // TODO respect layerData.index?

  UUID layer_id;
  switch (ir_layer.type) {
    case LayerType::TileLayer: {
      layer_id = map.add_tile_layer(parent);

      auto& layer = root_layer.get_tile_layer(layer_id);
      const auto& ir_tile_layer = ir_layer.as_tile_layer();

      invoke_mn(ir_tile_layer.extent.rows,
                ir_tile_layer.extent.cols,
                [&](const usize row, const usize col) {
                  layer.set_tile(TilePos::from(row, col), ir_tile_layer.tiles[row][col]);
                });

      break;
    }
    case LayerType::ObjectLayer: {
      layer_id = map.add_object_layer(parent);
      restore_object_layer(document, layer_id, ir_layer.as_object_layer());
      break;
    }
    case LayerType::GroupLayer: {
      layer_id = map.add_group_layer(parent);

      const auto& ir_group = ir_layer.as_group_layer();
      for (const auto& ir_child_layer: ir_group.children) {
        restore_layer(document, *ir_child_layer, layer_id);
      }

      break;
    }
    default:
      throw Exception {"Invalid layer type!"};
  }

  auto& layer = root_layer.get_layer(layer_id);
  layer.get_ctx().set_name(ir_layer.name);
  layer.set_opacity(ir_layer.opacity);
  layer.set_visible(ir_layer.visible);
  layer.set_meta_id(ir_layer.id);

  restore_context(document, root_layer.find_shared_layer(layer_id), ir_layer.context);

  return layer_id;
}

void restore_layers(MapDocument& document, const MapIR& ir_map)
{
  for (const auto& ir_layer: ir_map.layers) {
    restore_layer(document, ir_layer);
  }
}

void restore_tile_animation(Tile& tile, const TileIR& ir_tile)
{
  TileAnimation animation;
  animation.reserve_frames(ir_tile.frames.size());

  for (const auto& ir_frame: ir_tile.frames) {
    animation.add_frame(ir_frame.tile_index, Milliseconds {ir_frame.duration_ms});
  }

  tile.set_animation(std::move(animation));
}

void restore_fancy_tile_objects(TilesetDocument& document,
                                Tile& tile,
                                const TileIR& ir_tile)
{
  tile.reserve_objects(ir_tile.objects.size());

  for (const auto& ir_object: ir_tile.objects) {
    tile.add_object(restore_object(document, ir_object));
  }
}

void restore_fancy_tiles(TilesetDocument& document, const TilesetIR& ir_tileset)
{
  auto& tileset = document.get_tileset();
  for (const auto& [index, ir_tile]: ir_tileset.fancy_tiles) {
    auto tile = tileset.get_tile_ptr(index);
    TACTILE_ASSERT(tile->get_index() == index);

    if (!ir_tile.frames.empty()) {
      restore_tile_animation(*tile, ir_tile);
    }

    if (!ir_tile.objects.empty()) {
      restore_fancy_tile_objects(document, *tile, ir_tile);
    }

    restore_context(document, tile, ir_tile.context);
  }
}

void restore_tileset(DocumentModel& model,
                     const Shared<ComponentIndex>& component_index,
                     const TilesetIR& ir_tileset)
{
  TACTILE_ASSERT(model.get_active_document_id().has_value());

  // TODO compare tileset document absolute paths to recognize the same tileset being
  // loaded multiple times

  const TilesetInfo info {
      .texture = load_texture(ir_tileset.image_path),
      .tile_size = ir_tileset.tile_size,
  };

  // This will automatically attach the tileset to the active map
  const auto tileset_id = model.restore_tileset(ir_tileset.first_tile, info);

  auto tileset_document = model.get_tileset_document_ptr(tileset_id);
  tileset_document->set_name(ir_tileset.name);
  tileset_document->set_component_index(component_index);

  restore_fancy_tiles(*tileset_document, ir_tileset);
  restore_context_no_register(*tileset_document,
                              tileset_document->get_tileset_ptr(),
                              ir_tileset.context);
}

void restore_tilesets(DocumentModel& model,
                      const Shared<ComponentIndex>& component_index,
                      const MapIR& ir_map)
{
  for (const auto& tileset_data: ir_map.tilesets) {
    restore_tileset(model, component_index, tileset_data);
  }

  auto& map = model.require_active_map_document().get_map();
  auto& tileset_bundle = map.get_tileset_bundle();

  // Determine the next available tile identifier
  TileID next_tile_id {1};
  for (const auto& [tileset_id, tileset_ref]: tileset_bundle) {
    next_tile_id = std::max(next_tile_id, tileset_ref.get_last_tile() + 1);
  }
  tileset_bundle.set_next_tile_id(next_tile_id);

  if (!tileset_bundle.empty()) {
    tileset_bundle.select_tileset(tileset_bundle.begin()->first);
  }
}

void restore_component_definitions(MapDocument& document, const MapIR& ir_map)
{
  auto component_index = document.get_component_index_ptr();
  for (const auto& [name, attributes]: ir_map.component_definitions) {
    const auto component_id = component_index->define_comp(name);
    auto& component_def = component_index->get_comp(component_id);
    for (const auto& [attr_name, attr_value]: attributes) {
      component_def.add_attr(attr_name, attr_value);
    }
  }
}

void restore_tile_format(TileFormat& format, const TileFormatIR& ir_format)
{
  format.set_encoding(ir_format.encoding);
  format.set_compression(ir_format.compression);

  if (ir_format.zlib_compression_level) {
    format.set_zlib_compression_level(*ir_format.zlib_compression_level);
  }

  if (ir_format.zstd_compression_level) {
    format.set_zstd_compression_level(*ir_format.zstd_compression_level);
  }
}

}  // namespace

void create_map_document_from_ir(const MapIR& ir_map,
                                 const Path& document_path,
                                 DocumentModel& model)
{
  const auto map_id = model.create_map_document(ir_map.tile_size, ir_map.extent);
  model.select_document(map_id);

  auto component_index = std::make_shared<ComponentIndex>();

  auto map_document = model.get_map_document_ptr(map_id);
  map_document->set_component_index(component_index);

  auto& map = map_document->get_map();

  const auto absolute_document_path = fs::absolute(document_path);
  map_document->set_path(absolute_document_path);
  map_document->set_name(absolute_document_path.filename().string());

  map.set_tile_size(ir_map.tile_size);
  map.set_next_layer_id(ir_map.next_layer_id);
  map.set_next_object_id(ir_map.next_object_id);
  map.resize(ir_map.extent);

  restore_tile_format(map.get_tile_format(), ir_map.tile_format);
  restore_component_definitions(*map_document, ir_map);
  restore_tilesets(model, map_document->get_component_index_ptr(), ir_map);
  restore_layers(*map_document, ir_map);

  restore_context_no_register(*map_document, map_document->get_map_ptr(), ir_map.context);

  map_document->get_history().clear();
}

}  // namespace tactile
