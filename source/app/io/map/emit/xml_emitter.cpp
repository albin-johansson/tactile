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

#include <fstream>  // ofstream
#include <ios>      // ios
#include <sstream>  // stringstream
#include <variant>  // get

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "core/common/fs.hpp"
#include "core/util/filesystem.hpp"
#include "core/util/functional.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/map/tiled_info.hpp"
#include "io/persist/preferences.hpp"
#include "io/util/xml.hpp"
#include "misc/panic.hpp"

namespace tactile::io {
namespace {

void append_properties(XMLNode node, const ir::ContextData& context_data)
{
  if (context_data.properties.empty()) {
    return;
  }

  auto collection = node.append_child("properties");

  for (const auto& [propertyName, propertyValue] : context_data.properties) {
    auto property_node = collection.append_child("property");
    property_node.append_attribute("name").set_value(propertyName.c_str());

    const auto type = propertyValue.type();

    // Properties with no type attribute are assumed to be string properties
    if (type != AttributeType::String) {
      property_node.append_attribute("type").set_value(stringify(type));
    }

    auto value_attr = property_node.append_attribute("value");

    switch (type) {
      case AttributeType::String:
        value_attr.set_value(propertyValue.as_string().c_str());
        break;

      case AttributeType::Int:
        value_attr.set_value(propertyValue.as_int());
        break;

      case AttributeType::Float:
        value_attr.set_value(propertyValue.as_float());
        break;

      case AttributeType::Bool:
        value_attr.set_value(propertyValue.as_bool());
        break;

      case AttributeType::Path: {
        const auto str = convert_to_forward_slashes(propertyValue.as_path());
        value_attr.set_value(str.c_str());
        break;
      }
      case AttributeType::Color:
        value_attr.set_value(propertyValue.as_color().as_argb().c_str());
        break;

      case AttributeType::Object:
        value_attr.set_value(propertyValue.as_object());
        break;
    }
  }
}

void append_object(XMLNode node, const ir::ObjectData& object_data)
{
  auto object_node = node.append_child("object");
  object_node.append_attribute("id").set_value(object_data.id);

  if (!object_data.name.empty()) {
    object_node.append_attribute("name").set_value(object_data.name.c_str());
  }

  if (!object_data.tag.empty()) {
    object_node.append_attribute("type").set_value(object_data.tag.c_str());
  }

  if (object_data.pos.x != 0.0f) {
    object_node.append_attribute("x").set_value(object_data.pos.x);
  }

  if (object_data.pos.y != 0.0f) {
    object_node.append_attribute("y").set_value(object_data.pos.y);
  }

  if (object_data.size.x != 0.0f) {
    object_node.append_attribute("width").set_value(object_data.size.x);
  }

  if (object_data.size.y != 0.0f) {
    object_node.append_attribute("height").set_value(object_data.size.y);
  }

  if (!object_data.visible) {
    object_node.append_attribute("visible").set_value(0);
  }

  // Objects are assumed to be rectangles unless explicitly told otherwise
  if (object_data.type == ObjectType::Point) {
    object_node.append_child("point");
  }
  else if (object_data.type == ObjectType::Ellipse) {
    object_node.append_child("ellipse");
  }

  append_properties(object_node, object_data.context);
}

void append_common_layer_attributes(XMLNode node, const ir::LayerData& layer_data)
{
  node.append_attribute("id").set_value(layer_data.id);
  node.append_attribute("name").set_value(layer_data.name.c_str());

  if (layer_data.opacity != 1.0f) {
    node.append_attribute("opacity").set_value(layer_data.opacity);
  }

  if (!layer_data.visible) {
    node.append_attribute("visible").set_value(0);
  }
}

void append_tile_layer(XMLNode root, const ir::LayerData& layer_data)
{
  const auto& prefs = get_preferences();
  const auto& tile_layer_data = std::get<ir::TileLayerData>(layer_data.data);

  auto node = root.append_child("layer");
  append_common_layer_attributes(node, layer_data);

  node.append_attribute("width").set_value(tile_layer_data.col_count);
  node.append_attribute("height").set_value(tile_layer_data.row_count);

  append_properties(node, layer_data.context);

  auto data_node = node.append_child("data");
  data_node.append_attribute("encoding").set_value("csv");

  const auto count = tile_layer_data.row_count * tile_layer_data.col_count;

  std::stringstream stream;

  invoke_mn(tile_layer_data.row_count,
            tile_layer_data.col_count,
            [&, index = 0ull](const usize row, const usize col) mutable {
              if (prefs.fold_tile_data && index == 0) {
                stream << '\n';
              }

              stream << tile_layer_data.tiles[row][col];
              if (index < count - 1u) {
                stream << ',';
              }

              if (prefs.fold_tile_data && (index + 1) % tile_layer_data.col_count == 0) {
                stream << '\n';
              }

              ++index;
            });

  data_node.text().set(stream.str().c_str());
}

void append_object_layer(XMLNode root, const ir::LayerData& layer_data)
{
  const auto& object_layer_data = std::get<ir::ObjectLayerData>(layer_data.data);

  auto node = root.append_child("objectgroup");
  append_common_layer_attributes(node, layer_data);
  append_properties(node, layer_data.context);

  for (const auto& object_data : object_layer_data.objects) {
    append_object(node, object_data);
  }
}

void append_layer(XMLNode root, const ir::LayerData& layer_data)
{
  switch (layer_data.type) {
    case LayerType::TileLayer:
      append_tile_layer(root, layer_data);
      break;

    case LayerType::ObjectLayer:
      append_object_layer(root, layer_data);
      break;

    case LayerType::GroupLayer: {
      const auto& group_layer_data = std::get<ir::GroupLayerData>(layer_data.data);

      auto collection = root.append_child("group");
      append_common_layer_attributes(collection, layer_data);
      append_properties(collection, layer_data.context);

      for (const auto& child_layer_data : group_layer_data.children) {
        append_layer(collection, *child_layer_data);
      }

      break;
    }

    default:
      throw TactileError("Invalid layer type!");
  }
}

void append_fancy_tiles(XMLNode node, const ir::TilesetData& tileset_data)
{
  for (const auto& [id, tileData] : tileset_data.fancy_tiles) {
    auto tile_node = node.append_child("tile");
    tile_node.append_attribute("id").set_value(id);

    if (!tileData.frames.empty()) {
      auto animation_node = tile_node.append_child("animation");

      for (const auto& frame_data : tileData.frames) {
        auto frame_node = animation_node.append_child("frame");
        frame_node.append_attribute("tileid").set_value(frame_data.local_id);
        frame_node.append_attribute("duration").set_value(frame_data.duration_ms);
      }
    }

    if (!tileData.objects.empty()) {
      auto object_layer_node = tile_node.append_child("objectgroup");
      object_layer_node.append_attribute("draworder").set_value("index");

      for (const auto& object_data : tileData.objects) {
        append_object(object_layer_node, object_data);
      }
    }

    append_properties(tile_node, tileData.context);
  }
}

void append_common_tileset_attributes(XMLNode                node,
                                      const ir::TilesetData& tileset_data,
                                      const fs::path&        dir)
{
  node.append_attribute("name").set_value(tileset_data.name.c_str());

  node.append_attribute("tilewidth").set_value(tileset_data.tile_size.x);
  node.append_attribute("tileheight").set_value(tileset_data.tile_size.y);

  node.append_attribute("tilecount").set_value(tileset_data.tile_count);
  node.append_attribute("columns").set_value(tileset_data.column_count);

  {
    auto image_node = node.append_child("image");

    const auto source =
        convert_to_forward_slashes(fs::relative(tileset_data.image_path, dir));
    image_node.append_attribute("source").set_value(source.c_str());

    image_node.append_attribute("width").set_value(tileset_data.image_size.x);
    image_node.append_attribute("height").set_value(tileset_data.image_size.y);
  }

  append_fancy_tiles(node, tileset_data);
  append_properties(node, tileset_data.context);
}

void append_embedded_tileset(XMLNode                root,
                             const ir::TilesetData& tileset_data,
                             const fs::path&        dir)
{
  auto node = root.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset_data.first_tile);

  append_common_tileset_attributes(node, tileset_data, dir);
}

void append_external_tileset(XMLNode                root,
                             const ir::TilesetData& tileset_data,
                             const std::string&     filename)
{
  auto node = root.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset_data.first_tile);
  node.append_attribute("source").set_value(filename.c_str());
}

void emit_external_tileset_file(const fs::path&        path,
                                const ir::TilesetData& tileset_data,
                                const fs::path&        dir)
{
  pugi::xml_document document;

  auto root = document.append_child("tileset");
  root.append_attribute("version").set_value(tiled_xml_format_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  append_common_tileset_attributes(root, tileset_data, dir);

  std::ofstream stream {path, std::ios::out};
  document.save(stream);
}

void append_tileset(XMLNode                root,
                    const ir::TilesetData& tileset_data,
                    const fs::path&        dir)
{
  const auto& prefs = get_preferences();
  if (prefs.embed_tilesets) {
    append_embedded_tileset(root, tileset_data, dir);
  }
  else {
    const auto filename = fmt::format("{}.tsx", tileset_data.name);
    const auto source = dir / filename;
    emit_external_tileset_file(source, tileset_data, dir);
    append_external_tileset(root, tileset_data, filename);
  }
}

void append_root(pugi::xml_document& document, const EmitInfo& info)
{
  const auto& map_data = info.data();
  auto        root = document.append_child("map");

  root.append_attribute("version").set_value(tiled_xml_format_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  root.append_attribute("orientation").set_value("orthogonal");
  root.append_attribute("renderorder").set_value("right-down");
  root.append_attribute("infinite").set_value(0);

  root.append_attribute("tilewidth").set_value(map_data.tile_size.x);
  root.append_attribute("tileheight").set_value(map_data.tile_size.y);

  root.append_attribute("width").set_value(map_data.col_count);
  root.append_attribute("height").set_value(map_data.row_count);

  root.append_attribute("nextlayerid").set_value(map_data.next_layer_id);
  root.append_attribute("nextobjectid").set_value(map_data.next_object_id);

  append_properties(root, map_data.context);

  for (const auto& tileset_data : map_data.tilesets) {
    append_tileset(root, tileset_data, info.destination_dir());
  }

  for (const auto& layer_data : map_data.layers) {
    append_layer(root, layer_data);
  }
}

}  // namespace

void emit_xml_map(const EmitInfo& info)
{
  if (!info.data().component_definitions.empty()) {
    spdlog::warn("Component data will be ignored when saving the map as XML!");
  }

  pugi::xml_document document;
  append_root(document, info);

  std::ofstream stream {info.destination_file(), std::ios::out | std::ios::trunc};
  document.save(stream, " ");
}

}  // namespace tactile::io