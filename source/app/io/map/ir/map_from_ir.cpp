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
#include <variant>  // get

#include <entt/entity/registry.hpp>

#include "core/cmd/command_stack.hpp"
#include "core/common/filesystem.hpp"
#include "core/common/functional.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/comp/component_index.hpp"
#include "core/document/document.hpp"
#include "core/document/map_document.hpp"
#include "core/document/tileset_document.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/model.hpp"
#include "core/tile_pos.hpp"
#include "core/tileset/tileset_info.hpp"
#include "core/util/texture_manager.hpp"
#include "io/map/ir/ir.hpp"
#include "io/map/parse/parse_data.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

void restore_context_no_register(ADocument&              document,
                                 const Shared<IContext>& context,
                                 const ir::ContextData&  source)
{
  auto& properties = context->get_props();
  auto& components = context->get_comps();

  for (const auto& [propertyName, propertyValue] : source.properties) {
    properties.add(propertyName, propertyValue);
  }

  if (auto index = document.get_component_index()) {
    for (const auto& [type, attributes] : source.components) {
      const auto& definition = index->with_name(type);

      auto component = definition.instantiate();
      for (const auto& [attrName, attrValue] : attributes) {
        component.update_attr(attrName, attrValue);
      }

      components.add(std::move(component));
    }
  }
}

void restore_context(ADocument&              document,
                     const Shared<IContext>& context,
                     const ir::ContextData&  source)
{
  restore_context_no_register(document, context, source);
  document.get_contexts().add_context(context);
}

auto restore_object(ADocument& document, const ir::ObjectData& object_data)
    -> Shared<Object>
{
  auto object = std::make_shared<Object>();
  object->set_type(object_data.type);

  object->set_pos(object_data.pos);
  object->set_size(object_data.size);

  object->set_tag(object_data.tag);
  object->set_visible(object_data.visible);

  object->set_name(object_data.name);
  object->set_meta_id(object_data.id);

  restore_context(document, object, object_data.context);

  return object;
}

void restore_object_layer(MapDocument&               document,
                          const UUID&                layer_id,
                          const ir::ObjectLayerData& object_layer_data)
{
  auto& map = document.get_map();

  auto& layer = map.view_object_layer(layer_id);
  layer.reserve_objects(object_layer_data.objects.size());

  for (const auto& object_data : object_layer_data.objects) {
    layer.add_object(restore_object(document, object_data));
  }
}

auto restore_layer(MapDocument&         document,
                   const ir::LayerData& layer_data,
                   const Maybe<UUID>&   parent = nothing) -> UUID
{
  auto& map = document.get_map();
  // TODO respect layerData.index?

  UUID layer_id;
  switch (layer_data.type) {
    case LayerType::TileLayer: {
      const auto& data = std::get<ir::TileLayerData>(layer_data.data);

      layer_id = map.add_tile_layer(parent);

      auto& layer = map.view_tile_layer(layer_id);
      invoke_mn(data.row_count, data.col_count, [&](const usize r, const usize c) {
        layer.set_tile(TilePos::from(r, c), data.tiles[r][c]);
      });

      break;
    }
    case LayerType::ObjectLayer: {
      const auto& data = std::get<ir::ObjectLayerData>(layer_data.data);

      layer_id = map.add_object_layer(parent);
      restore_object_layer(document, layer_id, data);
      break;
    }
    case LayerType::GroupLayer: {
      const auto& data = std::get<ir::GroupLayerData>(layer_data.data);

      layer_id = map.add_group_layer(parent);
      for (const auto& child_layer_data : data.children) {
        restore_layer(document, *child_layer_data, layer_id);
      }

      break;
    }
    default:
      throw TactileError {"Invalid layer type!"};
  }

  auto& layer = map.view_layer(layer_id);
  layer.set_name(layer_data.name);
  layer.set_opacity(layer_data.opacity);
  layer.set_visible(layer_data.visible);
  layer.set_meta_id(layer_data.id);

  restore_context(document, map.get_layer(layer_id), layer_data.context);

  return layer_id;
}

void restore_layers(MapDocument& document, const ir::MapData& map_data)
{
  for (const auto& layer_data : map_data.layers) {
    restore_layer(document, layer_data);
  }
}

void restore_tile_animation(Tile& tile, const ir::MetaTileData& tile_data)
{
  TileAnimation animation;
  animation.reserve_frames(tile_data.frames.size());

  for (const auto& frame_data : tile_data.frames) {
    animation.add_frame(frame_data.local_id,
                        TileAnimation::Millis {frame_data.duration_ms});
  }

  tile.set_animation(std::move(animation));
}

void restore_fancy_tile_objects(TilesetDocument&        document,
                                Tile&                   tile,
                                const ir::MetaTileData& tile_data)
{
  tile.reserve_objects(tile_data.objects.size());

  for (const auto& object_data : tile_data.objects) {
    tile.add_object(restore_object(document, object_data));
  }
}

void restore_fancy_tiles(TilesetDocument& document, const ir::TilesetData& tileset_data)
{
  auto& tileset = document.view_tileset();
  for (const auto& [index, tileData] : tileset_data.fancy_tiles) {
    auto tile = tileset.get_tile(index);
    TACTILE_ASSERT(tile->index() == index);

    if (!tileData.frames.empty()) {
      restore_tile_animation(*tile, tileData);
    }

    if (!tileData.objects.empty()) {
      restore_fancy_tile_objects(document, *tile, tileData);
    }

    restore_context(document, tile, tileData.context);
  }
}

void restore_tileset(DocumentModel&                model,
                     TextureManager&               textures,
                     const Shared<ComponentIndex>& index,
                     const ir::TilesetData&        tileset_data)
{
  TACTILE_ASSERT(model.active_document_id().has_value());

  // TODO compare tileset document absolute paths to recognize the same tileset being
  // loaded multiple times

  const auto texture = textures.load(tileset_data.image_path).value();

  TilesetInfo info;
  info.texture_id = texture.id;
  info.texture_size = texture.size;
  info.texture_path = texture.path;
  info.tile_size = tileset_data.tile_size;

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

void restore_tilesets(DocumentModel&                model,
                      TextureManager&               textures,
                      const Shared<ComponentIndex>& index,
                      const ir::MapData&            map_data)
{
  for (const auto& tileset_data : map_data.tilesets) {
    restore_tileset(model, textures, index, tileset_data);
  }

  auto& document = model.require_active_map();
  auto& map = document.get_map();
  auto& tilesets = map.get_tilesets();

  if (!tilesets.empty()) {
    map.select_tileset(tilesets.begin()->first);
  }
}

void restore_component_definitions(MapDocument& document, const ir::MapData& map_data)
{
  auto index = document.get_component_index();
  for (const auto& [name, attributes] : map_data.component_definitions) {
    const auto id = index->define_comp(name);
    auto&      def = index->at(id);
    for (const auto& [attr_name, attr_value] : attributes) {
      def.add_attr(attr_name, attr_value);
    }
  }
}

}  // namespace

void map_from_ir(const ParseData& data, DocumentModel& model, TextureManager& textures)
{
  const auto& map_data = data.data();

  const auto map_id =
      model.add_map(map_data.tile_size, map_data.row_count, map_data.col_count);
  model.select_document(map_id);

  auto components = std::make_shared<ComponentIndex>();

  auto document = model.get_map(map_id);
  document->set_component_index(components);

  auto& map = document->get_map();

  const auto path = fs::absolute(data.path());
  document->set_path(path);
  document->set_name(path.filename().string());

  map.set_tile_size(map_data.tile_size);
  map.set_next_layer_id(map_data.next_layer_id);
  map.set_next_object_id(map_data.next_object_id);
  map.resize(map_data.row_count, map_data.col_count);

  restore_component_definitions(*document, map_data);

  restore_tilesets(model, textures, document->get_component_index(), map_data);
  restore_layers(*document, map_data);

  restore_context_no_register(*document, document->get_map_ptr(), map_data.context);

  document->get_history().clear();
}

}  // namespace tactile::io
