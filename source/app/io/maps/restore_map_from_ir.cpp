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

#include "restore_map_from_ir.hpp"

#include <filesystem>  // absolute
#include <utility>     // move
#include <variant>     // get

#include <entt/entity/registry.hpp>

#include "core/commands/command_stack.hpp"
#include "core/common/functional.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/components/component_definition.hpp"
#include "core/components/component_index.hpp"
#include "core/documents/document.hpp"
#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/layers/group_layer.hpp"
#include "core/layers/object_layer.hpp"
#include "core/layers/tile_layer.hpp"
#include "core/model.hpp"
#include "core/tile_pos.hpp"
#include "core/tilesets/tileset_info.hpp"
#include "core/utils/texture_manager.hpp"
#include "io/maps/ir.hpp"
#include "io/maps/parser/parse_data.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

void _restore_context(ADocument&             document,
                      core::IContext&        context,
                      const ir::ContextData& source)
{
  auto& properties = context.get_props();
  auto& components = context.get_comps();

  for (const auto& [propertyName, propertyValue] : source.properties) {
    properties.add(propertyName, propertyValue);
  }

  if (auto index = document.get_component_index()) {
    for (const auto& [type, attributes] : source.components) {
      const auto& definition = index->with_name(type);
      auto        component = definition.instantiate();
      for (const auto& [attrName, attrValue] : attributes) {
        component.add_attr(attrName, attrValue);
      }
    }
  }
}

auto _restore_object(ADocument& document, const ir::ObjectData& objectData)
    -> core::Object
{
  core::Object object;
  object.set_type(objectData.type);

  object.set_pos(objectData.pos);
  object.set_size(objectData.size);

  object.set_tag(objectData.tag);
  object.set_visible(objectData.visible);

  // TODO name, id

  _restore_context(document, object, objectData.context);

  return object;
}

void _restore_object_layer(MapDocument&               document,
                           const UUID&                layerId,
                           const ir::ObjectLayerData& objectLayerData)
{
  auto& map = document.get_map();

  auto& layer = map.view_object_layer(layerId);
  layer.reserve_objects(objectLayerData.objects.size());

  for (const auto& objectData : objectLayerData.objects) {
    layer.add_object(_restore_object(document, objectData));
  }
}

auto _restore_layer(MapDocument&         document,
                    const ir::LayerData& layerData,
                    const Maybe<UUID>&   parent = nothing) -> UUID
{
  auto& map = document.get_map();
  //  node.index = layerData.index;

  UUID layerId;
  switch (layerData.type) {
    case LayerType::TileLayer: {
      const auto& data = std::get<ir::TileLayerData>(layerData.data);

      layerId = map.add_tile_layer(parent);

      auto& layer = map.view_tile_layer(layerId);
      invoke_mn(data.row_count, data.col_count, [&](const usize r, const usize c) {
        layer.set_tile(TilePos::from(r, c), data.tiles[r][c]);
      });

      break;
    }
    case LayerType::ObjectLayer: {
      const auto& data = std::get<ir::ObjectLayerData>(layerData.data);

      layerId = map.add_object_layer(parent);
      _restore_object_layer(document, layerId, data);
      break;
    }
    case LayerType::GroupLayer: {
      const auto& data = std::get<ir::GroupLayerData>(layerData.data);

      layerId = map.add_group_layer(parent);
      for (const auto& childLayerData : data.children) {
        _restore_layer(document, *childLayerData, layerId);
      }

      break;
    }
    default:
      throw TactileError{"Invalid layer type!"};
  }

  auto& layer = map.view_layer(layerId);
  layer.set_name(layerData.name);
  layer.set_opacity(layerData.opacity);
  layer.set_visible(layerData.visible);

  _restore_context(document, layer, layerData.context);

  return layerId;
}

void _restore_layers(MapDocument& document, const ir::MapData& mapData)
{
  auto& map = document.get_map();

  for (const auto& layerData : mapData.layers) {
    _restore_layer(document, layerData);
  }

  // sys::sort_layers(registry);

  /*if (!registry.storage<comp::LayerTreeNode>().empty()) {
    auto& active = ctx_get<comp::ActiveState>(registry);
    active.layer = registry.view<comp::LayerTreeNode>().front();
  }*/
}

void _restore_tile_animation(core::Tile& tile, const ir::MetaTileData& tileData)
{
  core::TileAnimation animation;
  animation.reserve_frames(tileData.frames.size());

  for (const auto& frameData : tileData.frames) {
    animation.add_frame(frameData.local_id,
                        core::TileAnimation::Millis{frameData.duration_ms});
  }

  tile.set_animation(std::move(animation));
}

void _restore_fancy_tile_objects(TilesetDocument&        document,
                                 core::Tile&             tile,
                                 const ir::MetaTileData& tileData)
{
  tile.reserve_objects(tileData.objects.size());

  for (const auto& objectData : tileData.objects) {
    tile.add_object(_restore_object(document, objectData));
  }
}

void _restore_fancy_tiles(TilesetDocument& document, const ir::TilesetData& tilesetData)
{
  auto& tileset = document.view_tileset();
  for (const auto& [index, tileData] : tilesetData.fancy_tiles) {
    auto& tile = tileset[index];
    TACTILE_ASSERT(tile.index() == index);

    if (!tileData.frames.empty()) {
      _restore_tile_animation(tile, tileData);
    }

    if (!tileData.objects.empty()) {
      _restore_fancy_tile_objects(document, tile, tileData);
    }

    _restore_context(document, tile, tileData.context);
  }
}

void _restore_tileset(DocumentModel&                      model,
                      TextureManager&                     textures,
                      const Shared<core::ComponentIndex>& index,
                      const ir::TilesetData&              tilesetData)
{
  TACTILE_ASSERT(model.active_document_id().has_value());

  // TODO compare tileset document absolute paths to recognize the same tileset being
  // loaded multiple times

  const auto texture = textures.load(tilesetData.image_path).value();

  core::TilesetInfo info;
  info.texture_id = texture.id;
  info.texture_size = texture.size;
  info.texture_path = texture.path;
  info.tile_size = tilesetData.tile_size;

  // This will automatically attach the tileset to the active map
  const auto tilesetId = model.restore_tileset(tilesetData.first_tile, info);

  auto tilesetDocument = model.get_tileset(tilesetId);
  tilesetDocument->set_name(tilesetData.name);
  tilesetDocument->set_component_index(index);

  _restore_fancy_tiles(*tilesetDocument, tilesetData);

  auto& tileset = tilesetDocument->view_tileset();
  _restore_context(*tilesetDocument, tileset, tilesetData.context);
}

void _restore_tilesets(DocumentModel&                      model,
                       TextureManager&                     textures,
                       const Shared<core::ComponentIndex>& index,
                       const ir::MapData&                  mapData)
{
  for (const auto& tilesetData : mapData.tilesets) {
    _restore_tileset(model, textures, index, tilesetData);
  }

  auto& document = model.require_active_map();
  auto& map = document.get_map();
  auto& tilesets = map.get_tilesets();

  if (!tilesets.empty()) {
    map.select_tileset(tilesets.begin()->first);
  }
}

void _restore_component_definitions(MapDocument& document, const ir::MapData& mapData)
{
  auto index = document.get_component_index();
  for (const auto& [name, attributes] : mapData.component_definitions) {
    const auto id = index->define_comp(name);
    auto&      def = index->at(id);
    for (const auto& [attrName, attrValue] : attributes) {
      def.add_attr(attrName, attrValue);
    }
  }
}

}  // namespace

void restore_map_from_ir(const ParseData& data,
                         DocumentModel& model,
                         TextureManager& textures)
{
  const auto& mapData = data.data();

  const auto mapId =
      model.add_map(mapData.tile_size, mapData.row_count, mapData.col_count);
  model.select_document(mapId);

  auto components = std::make_shared<core::ComponentIndex>();

  auto document = model.get_map(mapId);
  document->set_component_index(components);

  auto& map = document->get_map();

  const auto path = std::filesystem::absolute(data.path());
  document->set_path(path);
  document->set_name(path.filename().string());

  map.set_tile_size(mapData.tile_size);
  map.resize(mapData.row_count, mapData.col_count);

  _restore_component_definitions(*document, mapData);

  _restore_tilesets(model, textures, document->get_component_index(), mapData);
  _restore_layers(*document, mapData);

  _restore_context(*document, map, mapData.context);

  document->get_history().clear();
}

}  // namespace tactile::io
