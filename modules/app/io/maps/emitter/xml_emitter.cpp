#include "xml_emitter.hpp"

#include <filesystem>  // path, relative
#include <fstream>     // ofstream
#include <ios>         // ios
#include <sstream>     // stringstream
#include <variant>     // get

#include <fmt/format.h>
#include <pugixml.hpp>

#include "core/utils/strings.hpp"
#include "emit_info.hpp"
#include "io/maps/tiled_info.hpp"
#include "io/persistence/preferences.hpp"
#include "logging.hpp"
#include "throw.hpp"

namespace tactile::emitter {
namespace {

void _append_properties(pugi::xml_node node,
                        const ir::attribute_context_data& contextData)
{
  if (contextData.properties.empty()) {
    return;
  }

  auto collection = node.append_child("properties");

  for (const auto& [propertyName, propertyValue] : contextData.properties) {
    auto propertyNode = collection.append_child("property");
    propertyNode.append_attribute("name").set_value(propertyName.c_str());

    const auto type = propertyValue.type();

    /* Properties with no type attribute are assumed to be string properties */
    if (type != attribute_type::string) {
      propertyNode.append_attribute("type").set_value(stringify(type));
    }

    auto valueAttr = propertyNode.append_attribute("value");

    switch (type) {
      case attribute_type::string:
        valueAttr.set_value(propertyValue.as_string().c_str());
        break;

      case attribute_type::integer:
        valueAttr.set_value(propertyValue.as_int());
        break;

      case attribute_type::floating:
        valueAttr.set_value(propertyValue.as_float());
        break;

      case attribute_type::boolean:
        valueAttr.set_value(propertyValue.as_bool());
        break;

      case attribute_type::file: {
        const auto str = convert_to_forward_slashes(propertyValue.as_file());
        valueAttr.set_value(str.c_str());
        break;
      }
      case attribute_type::color:
        valueAttr.set_value(propertyValue.as_color().as_argb().c_str());
        break;

      case attribute_type::object:
        valueAttr.set_value(propertyValue.as_object());
        break;
    }
  }
}

void _append_object(pugi::xml_node node, const ir::object_data& objectData)
{
  auto objectNode = node.append_child("object");
  objectNode.append_attribute("id").set_value(objectData.id);

  if (!objectData.name.empty()) {
    objectNode.append_attribute("name").set_value(objectData.name.c_str());
  }

  if (!objectData.tag.empty()) {
    objectNode.append_attribute("type").set_value(objectData.tag.c_str());
  }

  if (objectData.x != 0.0f) {
    objectNode.append_attribute("x").set_value(objectData.x);
  }

  if (objectData.y != 0.0f) {
    objectNode.append_attribute("y").set_value(objectData.y);
  }

  if (objectData.width != 0.0f) {
    objectNode.append_attribute("width").set_value(objectData.width);
  }

  if (objectData.height != 0.0f) {
    objectNode.append_attribute("height").set_value(objectData.height);
  }

  if (!objectData.visible) {
    objectNode.append_attribute("visible").set_value(0);
  }

  /* Objects are assumed to be rectangles unless explicitly told otherwise */
  if (objectData.type == object_type::point) {
    objectNode.append_child("point");
  }
  else if (objectData.type == object_type::ellipse) {
    objectNode.append_child("ellipse");
  }

  _append_properties(objectNode, objectData.context);
}

void _append_common_layer_attributes(pugi::xml_node node, const ir::layer_data& layerData)
{
  node.append_attribute("id").set_value(layerData.id);
  node.append_attribute("name").set_value(layerData.name.c_str());

  if (layerData.opacity != 1.0f) {
    node.append_attribute("opacity").set_value(layerData.opacity);
  }

  if (!layerData.visible) {
    node.append_attribute("visible").set_value(0);
  }
}

void _append_tile_layer(pugi::xml_node root, const ir::layer_data& layerData)
{
  const auto& tileLayerData = std::get<ir::tile_layer_data>(layerData.data);

  auto node = root.append_child("layer");
  _append_common_layer_attributes(node, layerData);

  node.append_attribute("width").set_value(tileLayerData.col_count);
  node.append_attribute("height").set_value(tileLayerData.row_count);

  _append_properties(node, layerData.context);

  auto dataNode = node.append_child("data");
  dataNode.append_attribute("encoding").set_value("csv");

  const auto count = tileLayerData.row_count * tileLayerData.col_count;

  std::stringstream stream;
  usize index = 0;

  const bool foldTileData = get_preferences().fold_tile_data();

  for (usize row = 0; row < tileLayerData.row_count; ++row) {
    for (usize col = 0; col < tileLayerData.col_count; ++col) {
      if (foldTileData && index == 0) {
        stream << '\n';
      }

      stream << tileLayerData.tiles[row][col];
      if (index < count - 1u) {
        stream << ',';
      }

      if (foldTileData && (index + 1) % tileLayerData.col_count == 0) {
        stream << '\n';
      }

      ++index;
    }
  }

  dataNode.text().set(stream.str().c_str());
}

void _append_object_layer(pugi::xml_node root, const ir::layer_data& layerData)
{
  const auto& objectLayerData = std::get<ir::object_layer_data>(layerData.data);

  auto node = root.append_child("objectgroup");
  _append_common_layer_attributes(node, layerData);
  _append_properties(node, layerData.context);

  for (const auto& objectData : objectLayerData.objects) {
    _append_object(node, objectData);
  }
}

void _append_layer(pugi::xml_node root, const ir::layer_data& layerData)
{
  switch (layerData.type) {
    case layer_type::tile_layer:
      _append_tile_layer(root, layerData);
      break;

    case layer_type::object_layer:
      _append_object_layer(root, layerData);
      break;

    case layer_type::group_layer: {
      const auto& groupLayerData = std::get<ir::group_layer_data>(layerData.data);

      auto collection = root.append_child("group");
      _append_common_layer_attributes(collection, layerData);

      for (const auto& childLayerData : groupLayerData.children) {
        _append_layer(collection, *childLayerData);
      }

      break;
    }

    default:
      throw_traced(tactile_error{"Invalid layer type!"});
  }
}

void _append_fancy_tiles(pugi::xml_node node, const ir::tileset_data& tilesetData)
{
  for (const auto& [id, tileData] : tilesetData.fancy_tiles) {
    auto tileNode = node.append_child("tile");
    tileNode.append_attribute("id").set_value(id);

    if (!tileData.frames.empty()) {
      auto animationNode = tileNode.append_child("animation");

      for (const auto& frameData : tileData.frames) {
        auto frameNode = animationNode.append_child("frame");
        frameNode.append_attribute("tileid").set_value(frameData.local_id);
        frameNode.append_attribute("duration").set_value(frameData.duration_ms);
      }
    }

    if (!tileData.objects.empty()) {
      auto objectLayerNode = tileNode.append_child("objectgroup");
      objectLayerNode.append_attribute("draworder").set_value("index");

      for (const auto& objectData : tileData.objects) {
        _append_object(objectLayerNode, objectData);
      }
    }

    _append_properties(tileNode, tileData.context);
  }
}

void _append_common_tileset_attributes(pugi::xml_node node,
                                       const ir::tileset_data& tilesetData,
                                       const std::filesystem::path& dir)
{
  node.append_attribute("name").set_value(tilesetData.name.c_str());

  node.append_attribute("tilewidth").set_value(tilesetData.tile_width);
  node.append_attribute("tileheight").set_value(tilesetData.tile_height);

  node.append_attribute("tilecount").set_value(tilesetData.tile_count);
  node.append_attribute("columns").set_value(tilesetData.column_count);

  {
    auto imageNode = node.append_child("image");

    const auto source = std::filesystem::relative(tilesetData.image_path, dir);
    imageNode.append_attribute("source").set_value(source.c_str());

    imageNode.append_attribute("width").set_value(tilesetData.image_width);
    imageNode.append_attribute("height").set_value(tilesetData.image_height);
  }

  _append_fancy_tiles(node, tilesetData);
  _append_properties(node, tilesetData.context);
}

void _append_embedded_tileset(pugi::xml_node root,
                              const ir::tileset_data& tilesetData,
                              const std::filesystem::path& dir)
{
  auto node = root.append_child("tileset");
  node.append_attribute("firstgid").set_value(tilesetData.first_tile);

  _append_common_tileset_attributes(node, tilesetData, dir);
}

void _append_external_tileset(pugi::xml_node root,
                              const ir::tileset_data& tilesetData,
                              const std::string& filename)
{
  auto node = root.append_child("tileset");
  node.append_attribute("firstgid").set_value(tilesetData.first_tile);
  node.append_attribute("source").set_value(filename.c_str());
}

void _emit_external_tileset_file(const std::filesystem::path& path,
                                 const ir::tileset_data& tilesetData,
                                 const std::filesystem::path& dir)
{
  pugi::xml_document document;

  auto root = document.append_child("tileset");
  root.append_attribute("version").set_value(tiled_xml_format_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  _append_common_tileset_attributes(root, tilesetData, dir);

  std::ofstream stream{path, std::ios::out};
  document.save(stream);
}

void _append_tileset(pugi::xml_node root,
                     const ir::tileset_data& tilesetData,
                     const std::filesystem::path& dir)
{
  const auto& prefs = get_preferences();
  if (prefs.embed_tilesets()) {
    _append_embedded_tileset(root, tilesetData, dir);
  }
  else {
    const auto filename = fmt::format("{}.tsx", tilesetData.name);
    const auto source = dir / filename;
    _emit_external_tileset_file(source, tilesetData, dir);
    _append_external_tileset(root, tilesetData, filename);
  }
}

void _append_root(pugi::xml_document& document, const emit_info& info)
{
  const auto& mapData = info.data();
  auto root = document.append_child("map");

  root.append_attribute("version").set_value(tiled_xml_format_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  root.append_attribute("orientation").set_value("orthogonal");
  root.append_attribute("renderorder").set_value("right-down");
  root.append_attribute("infinite").set_value(0);

  root.append_attribute("tilewidth").set_value(mapData.tile_width);
  root.append_attribute("tileheight").set_value(mapData.tile_height);

  root.append_attribute("width").set_value(mapData.col_count);
  root.append_attribute("height").set_value(mapData.row_count);

  root.append_attribute("nextlayerid").set_value(mapData.next_layer_id);
  root.append_attribute("nextobjectid").set_value(mapData.next_object_id);

  _append_properties(root, mapData.context);

  for (const auto& tilesetData : mapData.tilesets) {
    _append_tileset(root, tilesetData, info.destination_dir());
  }

  for (const auto& layerData : mapData.layers) {
    _append_layer(root, layerData);
  }
}

}  // namespace

void emit_xml_map(const emit_info& info)
{
  if (!info.data().component_definitions.empty()) {
    log_warning("Component data will be ignored when saving the map as XML!");
  }

  pugi::xml_document document;
  _append_root(document, info);

  std::ofstream stream{info.destination_file(), std::ios::out};
  document.save(stream, " ");
}

}  // namespace tactile::emitter