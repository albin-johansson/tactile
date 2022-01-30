#include "json_emitter.hpp"

#include <fstream>  // ofstream
#include <iomanip>  // setw
#include <ios>      // ios
#include <utility>  // move
#include <variant>  // get

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "emit_info.hpp"
#include "io/maps/json_utils.hpp"
#include "io/preferences.hpp"
#include "logging.hpp"
#include "tactile_stdlib.hpp"

namespace tactile::emitter {
namespace {

constexpr c_str tiled_version = "1.7.0";  // TODO check
constexpr c_str format_version = "1.6";   // TODO check

void _write_json(const std::filesystem::path& path, const nlohmann::json& json)
{
  std::ofstream stream{path, std::ios::out};

  if (prefs::GetIndentOutput()) {
    stream << std::setw(2);
  }

  stream << json;
}

[[nodiscard]] auto _emit_properties(const ir::attribute_context_data& data)
    -> nlohmann::json
{
  auto array = nlohmann::json::array();

  for (const auto& [name, value] : data.properties) {
    auto json = nlohmann::json::object();

    json["name"] = name;
    json["type"] = value.type();
    json["value"] = value;

    array += json;
  }

  return array;
}

[[nodiscard]] auto _emit_object(const ir::object_data& data) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["id"] = data.id;
  json["name"] = data.name;
  json["x"] = data.x;
  json["y"] = data.y;
  json["width"] = data.width;
  json["height"] = data.height;
  json["visible"] = data.visible;
  json["type"] = data.tag;
  json["rotation"] = 0;

  switch (data.type) {
    case object_type::rect:
      /* Assumed to be rectangle if no point/ellipse indicators */
      break;

    case object_type::point: {
      json["point"] = true;
      break;
    }
    case object_type::ellipse: {
      json["ellipse"] = true;
      break;
    }
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto _emit_layer(const ir::layer_data& data,
                               const usize rows,
                               const usize columns) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["id"] = data.id;
  json["name"] = data.name;
  json["opacity"] = data.opacity;
  json["visible"] = data.visible;
  json["x"] = 0;
  json["y"] = 0;

  switch (data.type) {
    case layer_type::tile_layer: {
      const auto& tileLayerData = std::get<ir::tile_layer_data>(data.data);

      json["type"] = "tilelayer";
      json["width"] = rows;
      json["height"] = columns;

      auto tiles = nlohmann::json::array();

      for (usize row = 0; row < rows; ++row) {
        for (usize col = 0; col < columns; ++col) {
          tiles += tileLayerData.tiles[row][col];
        }
      }

      json["data"] = std::move(tiles);
      break;
    }
    case layer_type::object_layer: {
      const auto& objectLayerData = std::get<ir::object_layer_data>(data.data);

      json["type"] = "objectgroup";

      auto objects = nlohmann::json::array();

      for (const auto& objectData : objectLayerData.objects) {
        objects += _emit_object(objectData);
      }

      json["objects"] = std::move(objects);
      break;
    }
    case layer_type::group_layer: {
      const auto& groupLayerData = std::get<ir::group_layer_data>(data.data);

      json["type"] = "group";

      auto layers = nlohmann::json::array();

      for (const auto& childLayerData : groupLayerData.children) {
        layers += _emit_layer(*childLayerData, rows, columns);
      }

      json["layers"] = std::move(layers);
      break;
    }
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto _emit_layers(const ir::map_data& data) -> nlohmann::json
{
  auto array = nlohmann::json::array();

  for (const auto& layerData : data.layers) {
    array += _emit_layer(layerData, data.row_count, data.col_count);
  }

  return array;
}

[[nodiscard]] auto _emit_fancy_tile_animation(const ir::fancy_tile_data& data)
    -> nlohmann::json
{
  auto array = nlohmann::json::array();

  for (const auto& frameData : data.frames) {
    auto json = nlohmann::json::object();

    json["tileid"] = frameData.local_id;
    json["duration"] = frameData.duration_ms;

    array += json;
  }

  return array;
}

[[nodiscard]] auto _emit_fancy_tile(const emit_info& info,
                                    const tile_id id,
                                    const ir::fancy_tile_data& data) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["id"] = id;

  if (!data.frames.empty()) {
    json["animation"] = _emit_fancy_tile_animation(data);
  }

  if (!data.objects.empty()) {
    auto dummy = nlohmann::json::object();
    dummy["draworder"] = "index";
    dummy["name"] = "";
    dummy["opacity"] = 1;
    dummy["type"] = "objectgroup";
    dummy["visible"] = true;
    dummy["x"] = 0;
    dummy["y"] = 0;

    auto objects = nlohmann::json::array();
    for (const auto& objectData : data.objects) {
      objects += _emit_object(objectData);
    }

    dummy["objects"] = std::move(objects);
    json["objectgroup"] = std::move(dummy);
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  return json;
}

[[nodiscard]] auto _emit_fancy_tiles(const emit_info& info, const ir::tileset_data& data)
    -> nlohmann::json
{
  auto json = nlohmann::json::array();

  for (const auto& [id, tileData] : data.fancy_tiles) {
    json += _emit_fancy_tile(info, id, tileData);
  }

  return json;
}

void _add_common_tileset_attributes(nlohmann::json& json,
                                    const emit_info& info,
                                    const ir::tileset_data& data)
{
  json["name"] = data.name;
  json["columns"] = data.column_count;

  json["tilewidth"] = data.tile_width;
  json["tileheight"] = data.tile_height;
  json["tilecount"] = data.tile_count;

  const auto imagePath =
      std::filesystem::relative(data.image_path, info.destination_dir());
  json["image"] = ConvertToForwardSlashes(imagePath);

  json["imagewidth"] = data.image_width;
  json["imageheight"] = data.image_height;

  json["margin"] = 0;
  json["spacing"] = 0;

  if (!data.fancy_tiles.empty()) {
    json["tiles"] = _emit_fancy_tiles(info, data);
  }

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }
}

[[nodiscard]] auto _emit_embedded_tileset(const emit_info& info,
                                          const ir::tileset_data& data) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["firstgid"] = data.first_tile;
  _add_common_tileset_attributes(json, info, data);

  return json;
}

[[nodiscard]] auto _emit_external_tileset(const ir::tileset_data& data) -> nlohmann::json
{
  auto json = nlohmann::json::object();

  json["firstgid"] = data.first_tile;
  json["source"] = fmt::format("{}.json", data.name);

  return json;
}

void _create_external_tileset_file(const emit_info& info, const ir::tileset_data& data)
{
  auto json = nlohmann::json::object();
  _add_common_tileset_attributes(json, info, data);

  json["type"] = "tileset";
  json["tiledversion"] = tiled_version;
  json["version"] = format_version;

  const auto name = fmt::format("{}.json", data.name);
  const auto path = info.destination_dir() / name;

  _write_json(path, json);
}

[[nodiscard]] auto _emit_tileset(const emit_info& info, const ir::tileset_data& data)
    -> nlohmann::json
{
  if (prefs::GetEmbedTilesets()) {
    return _emit_embedded_tileset(info, data);
  }
  else {
    _create_external_tileset_file(info, data);
    return _emit_external_tileset(data);
  }
}

[[nodiscard]] auto _emit_tilesets(const emit_info& info) -> nlohmann::json
{
  auto json = nlohmann::json::array();

  const auto& data = info.data();
  for (const auto& tilesetData : data.tilesets) {
    json += _emit_tileset(info, tilesetData);
  }

  return json;
}

}  // namespace

void emit_json_map(const emit_info& info)
{
  auto json = nlohmann::json::object();

  const auto& data = info.data();

  if (!data.component_definitions.empty()) {
    log_warning("Component data is ignored when saving maps as JSON!");
  }

  json["type"] = "map";
  json["width"] = data.col_count;
  json["height"] = data.row_count;

  json["tilewidth"] = data.tile_width;
  json["tileheight"] = data.tile_height;

  json["nextlayerid"] = data.next_layer_id;
  json["nextobjectid"] = data.next_object_id;

  json["infinite"] = false;
  json["orientation"] = "orthogonal";
  json["renderorder"] = "right-down";
  json["compressionlevel"] = -1;
  json["tiledversion"] = tiled_version;
  json["version"] = format_version;

  json["tilesets"] = _emit_tilesets(info);
  json["layers"] = _emit_layers(data);

  if (!data.context.properties.empty()) {
    json["properties"] = _emit_properties(data.context);
  }

  _write_json(info.destination_file(), json);
}

}  // namespace tactile::emitter