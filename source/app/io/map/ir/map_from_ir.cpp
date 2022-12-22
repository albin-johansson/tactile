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

#include "map_from_ir.hpp"

#include <utility>  // move

#include <entt/entity/registry.hpp>

#include "core/component/component_index.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tile_pos.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/tile/tileset_info.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/util/functional.hpp"
#include "core/uuid.hpp"
#include "debug/assert.hpp"
#include "io/load_texture.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/parse_result.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/document/document.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"

namespace tactile::io {
namespace {

void restore_context_no_register(Document& document,
                                 const Shared<Context>& context,
                                 const ir::ContextData& source)
{
  auto& properties = context->get_ctx().props();
  auto& components = context->get_ctx().comps();

  for (const auto& [property_name, property_value]: source.properties) {
    properties.add(property_name, property_value);
  }

  if (auto index = document.get_component_index()) {
    for (const auto& [type, attributes]: source.components) {
      const auto& definition = index->with_name(type);

      auto component = definition.instantiate();
      for (const auto& [attr_name, attr_value]: attributes) {
        component.update(attr_name, attr_value);
      }

      components.add(std::move(component));
    }
  }
}

void restore_context(Document& document,
                     const Shared<Context>& context,
                     const ir::ContextData& source)
{
  restore_context_no_register(document, context, source);
  document.get_contexts().add_context(context);
}

auto restore_object(Document& document, const ir::ObjectData& object_data)
    -> Shared<Object>
{
  auto object = std::make_shared<Object>();
  object->set_type(object_data.type);

  object->set_pos(object_data.pos);
  object->set_size(object_data.size);

  object->set_tag(object_data.tag);
  object->set_visible(object_data.visible);

  object->get_ctx().set_name(object_data.name);
  object->set_meta_id(object_data.id);

  restore_context(document, object, object_data.context);

  return object;
}

void restore_object_layer(MapDocument& document,
                          const UUID& layer_id,
                          const ir::ObjectLayerData& object_layer_data)
{
  auto& map = document.get_map();

  auto& layer = map.invisible_root().get_object_layer(layer_id);
  layer.reserve_objects(object_layer_data.objects.size());

  for (const auto& object_data: object_layer_data.objects) {
    layer.add_object(restore_object(document, object_data));
  }
}

auto restore_layer(MapDocument& document,
                   const ir::LayerData& layer_data,
                   const Maybe<UUID>& parent = nothing) -> UUID
{
  auto& map = document.get_map();
  auto& root = map.invisible_root();
  // TODO respect layerData.index?

  UUID layer_id;
  switch (layer_data.type) {
    case LayerType::TileLayer: {
      layer_id = map.add_tile_layer(parent);

      auto& layer = root.get_tile_layer(layer_id);
      const auto& tile_data = layer_data.as_tile_layer();

      invoke_mn(tile_data.row_count,
                tile_data.col_count,
                [&](const usize r, const usize c) {
                  layer.set_tile(TilePos::from(r, c), tile_data.tiles[r][c]);
                });

      break;
    }
    case LayerType::ObjectLayer: {
      layer_id = map.add_object_layer(parent);
      restore_object_layer(document, layer_id, layer_data.as_object_layer());
      break;
    }
    case LayerType::GroupLayer: {
      layer_id = map.add_group_layer(parent);

      const auto& group = layer_data.as_group_layer();
      for (const auto& child_layer_data: group.children) {
        restore_layer(document, *child_layer_data, layer_id);
      }

      break;
    }
    default:
      throw TactileError {"Invalid layer type!"};
  }

  auto& layer = root.get_layer(layer_id);
  layer.get_ctx().set_name(layer_data.name);
  layer.set_opacity(layer_data.opacity);
  layer.set_visible(layer_data.visible);
  layer.set_meta_id(layer_data.id);

  restore_context(document, root.get_layer_ptr(layer_id), layer_data.context);

  return layer_id;
}

void restore_layers(MapDocument& document, const ir::MapData& map_data)
{
  for (const auto& layer_data: map_data.layers) {
    restore_layer(document, layer_data);
  }
}

void restore_tile_animation(Tile& tile, const ir::MetaTileData& tile_data)
{
  TileAnimation animation;
  animation.reserve_frames(tile_data.frames.size());

  for (const auto& frame_data: tile_data.frames) {
    animation.add_frame(frame_data.tile_index, ms_t {frame_data.duration_ms});
  }

  tile.set_animation(std::move(animation));
}

void restore_fancy_tile_objects(TilesetDocument& document,
                                Tile& tile,
                                const ir::MetaTileData& tile_data)
{
  tile.reserve_objects(tile_data.objects.size());

  for (const auto& object_data: tile_data.objects) {
    tile.add_object(restore_object(document, object_data));
  }
}

void restore_fancy_tiles(TilesetDocument& document, const ir::TilesetData& tileset_data)
{
  auto& tileset = document.view_tileset();
  for (const auto& [index, tile_data]: tileset_data.fancy_tiles) {
    auto tile = tileset.get_tile_ptr(index);
    TACTILE_ASSERT(tile->get_index() == index);

    if (!tile_data.frames.empty()) {
      restore_tile_animation(*tile, tile_data);
    }

    if (!tile_data.objects.empty()) {
      restore_fancy_tile_objects(document, *tile, tile_data);
    }

    restore_context(document, tile, tile_data.context);
  }
}

void restore_tileset(DocumentModel& model,
                     const Shared<ComponentIndex>& index,
                     const ir::TilesetData& tileset_data)
{
  TACTILE_ASSERT(model.active_document_id().has_value());

  // TODO compare tileset document absolute paths to recognize the same tileset being
  // loaded multiple times

  TilesetInfo info {
      .texture = io::load_texture(tileset_data.image_path),
      .tile_size = tileset_data.tile_size,
  };

  // This will automatically attach the tileset to the active map
  const auto tileset_id = model.restore_tileset(tileset_data.first_tile, info);

  auto tileset_document = model.get_tileset(tileset_id);
  tileset_document->set_name(tileset_data.name);
  tileset_document->set_component_index(index);

  restore_fancy_tiles(*tileset_document, tileset_data);
  restore_context_no_register(*tileset_document,
                              tileset_document->get_tileset(),
                              tileset_data.context);
}

void restore_tilesets(DocumentModel& model,
                      const Shared<ComponentIndex>& index,
                      const ir::MapData& map_data)
{
  for (const auto& tileset_data: map_data.tilesets) {
    restore_tileset(model, index, tileset_data);
  }

  auto& document = model.require_active_map();
  auto& map = document.get_map();
  auto& tilesets = map.tileset_bundle();

  if (!tilesets.empty()) {
    map.tileset_bundle().select_tileset(tilesets.begin()->first);
  }
}

void restore_component_definitions(MapDocument& document, const ir::MapData& map_data)
{
  auto index = document.get_component_index();
  for (const auto& [name, attributes]: map_data.component_definitions) {
    const auto id = index->define(name);
    auto& def = index->at(id);
    for (const auto& [attr_name, attr_value]: attributes) {
      def.add(attr_name, attr_value);
    }
  }
}

void restore_tile_format(TileFormat& format, const ir::TileFormatData& data)
{
  format.set_encoding(data.encoding);
  format.set_compression(data.compression);

  if (data.zlib_compression_level) {
    format.set_zlib_compression_level(*data.zlib_compression_level);
  }

  if (data.zstd_compression_level) {
    format.set_zstd_compression_level(*data.zstd_compression_level);
  }
}

}  // namespace

void map_from_ir(const ParseResult& result, DocumentModel& model)
{
  const auto& map_data = result.data();

  const auto map_id =
      model.add_map(map_data.tile_size, map_data.row_count, map_data.col_count);
  model.select_document(map_id);

  auto components = std::make_shared<ComponentIndex>();

  auto document = model.get_map(map_id);
  document->set_component_index(components);

  auto& map = document->get_map();

  const auto path = fs::absolute(result.path());
  document->set_path(path);
  document->set_name(path.filename().string());

  map.set_tile_size(map_data.tile_size);
  map.set_next_layer_id(map_data.next_layer_id);
  map.set_next_object_id(map_data.next_object_id);
  map.resize(map_data.row_count, map_data.col_count);

  restore_tile_format(map.tile_format(), map_data.tile_format);
  restore_component_definitions(*document, map_data);
  restore_tilesets(model, document->get_component_index(), map_data);
  restore_layers(*document, map_data);

  restore_context_no_register(*document, document->get_map_ptr(), map_data.context);

  document->get_history().clear();
}

}  // namespace tactile::io
