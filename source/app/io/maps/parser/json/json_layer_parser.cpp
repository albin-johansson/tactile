#include "json_layer_parser.hpp"

#include "core/utils/tiles.hpp"
#include "io/maps/json_utils.hpp"
#include "json_attribute_parser.hpp"
#include "logging.hpp"

namespace tactile::parsing {
namespace {

[[nodiscard]] auto _parse_object_layer(const nlohmann::json& json,
                                       ir::layer_data& layerData) -> parse_error
{
  auto& objectLayerData = layerData.data.emplace<ir::object_layer_data>();

  if (const auto iter = json.find("objects"); iter != json.end()) {
    objectLayerData.objects.reserve(iter->size());

    for (const auto& [_, value] : iter->items()) {
      auto& objectData = objectLayerData.objects.emplace_back();
      if (const auto err = parse_object(value, objectData); err != parse_error::none) {
        return err;
      }
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_tile_layer_data(const nlohmann::json& json,
                                          tile_matrix& tiles,
                                          const usize columns) -> parse_error
{
  /* We only support the CSV tile encoding, which is the implicit default */
  if (auto encoding = as_string(json, "encoding")) {
    if (encoding != "csv") {
      return parse_error::unsupported_tile_layer_encoding;
    }
  }

  if (!json.contains("data")) {
    return parse_error::no_tile_layer_data;
  }

  const auto data = json.at("data");
  if (!data.is_array()) {
    return parse_error::corrupt_tile_layer_data;
  }

  usize index = 0;
  for (const auto& [_, value] : data.items()) {
    if (value.is_number_integer()) {
      const auto [row, col] = to_matrix_coords(index, columns);
      tiles[row][col] = value.get<tile_id>();
      ++index;
    }
    else {
      return parse_error::corrupt_tile_layer_data;
    }
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_tile_layer(const nlohmann::json& json,
                                     ir::layer_data& layerData) -> parse_error
{
  auto& tileLayerData = layerData.data.emplace<ir::tile_layer_data>();

  if (const auto width = as_uint(json, "width")) {
    tileLayerData.col_count = *width;
  }
  else {
    return parse_error::no_tile_layer_width;
  }

  if (const auto height = as_uint(json, "height")) {
    tileLayerData.row_count = *height;
  }
  else {
    return parse_error::no_tile_layer_height;
  }

  tileLayerData.tiles =
      make_tile_matrix(tileLayerData.row_count, tileLayerData.col_count);

  if (const auto err =
          _parse_tile_layer_data(json, tileLayerData.tiles, tileLayerData.col_count);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

[[nodiscard]] auto _parse_layer(const nlohmann::json& json,
                                ir::layer_data& layerData,
                                const usize index) -> parse_error
{
  layerData.index = index;

  if (const auto id = as_int(json, "id")) {
    layerData.id = *id;
  }
  else {
    return parse_error::no_layer_id;
  }

  layerData.name = as_string(json, "name").value_or("Layer");
  layerData.opacity = as_float(json, "opacity").value_or(1.0f);
  layerData.visible = as_bool(json, "visible").value_or(true);

  if (auto type = as_string(json, "type")) {
    if (type == "tilelayer") {
      layerData.type = layer_type::tile_layer;
      if (const auto err = _parse_tile_layer(json, layerData); err != parse_error::none) {
        return err;
      }
    }
    else if (type == "objectgroup") {
      layerData.type = layer_type::object_layer;
      if (const auto err = _parse_object_layer(json, layerData);
          err != parse_error::none) {
        return err;
      }
    }
    else if (type == "group") {
      layerData.type = layer_type::group_layer;
      auto& groupLayerData = layerData.data.emplace<ir::group_layer_data>();

      usize childIndex = 0;
      for (const auto& [_, value] : json.at("layers").items()) {
        auto& childLayerData =
            groupLayerData.children.emplace_back(std::make_unique<ir::layer_data>());

        if (const auto err = _parse_layer(value, *childLayerData, childIndex);
            err != parse_error::none) {
          return err;
        }

        ++childIndex;
      }
    }
    else {
      return parse_error::unsupported_layer_type;
    }
  }
  else {
    return parse_error::no_layer_type;
  }

  if (const auto err = parse_properties(json, layerData.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

}  // namespace

auto parse_object(const nlohmann::json& json, ir::object_data& objectData) -> parse_error
{
  if (const auto id = as_int(json, "id")) {
    objectData.id = *id;
  }
  else {
    return parse_error::no_object_id;
  }

  objectData.name = as_string(json, "name").value_or("");
  objectData.tag = as_string(json, "type").value_or("");

  objectData.x = as_float(json, "x").value_or(0.0f);
  objectData.y = as_float(json, "y").value_or(0.0f);
  objectData.width = as_float(json, "width").value_or(0.0f);
  objectData.height = as_float(json, "height").value_or(0.0f);

  objectData.visible = as_bool(json, "visible").value_or(true);

  if (json.contains("point")) {
    objectData.type = object_type::point;
  }
  else if (json.contains("ellipse")) {
    objectData.type = object_type::ellipse;
  }
  else {
    objectData.type = object_type::rect;
  }

  if (const auto err = parse_properties(json, objectData.context);
      err != parse_error::none) {
    return err;
  }

  return parse_error::none;
}

auto parse_layers(const nlohmann::json& json, ir::map_data& mapData) -> parse_error
{
  const auto iter = json.find("layers");

  if (iter == json.end()) {
    log_warning("JSON map has no \"layers\" attribute, which is required!");
    return parse_error::none;
  }

  mapData.layers.reserve(iter->size());

  usize index = 0;
  for (const auto& [key, value] : iter->items()) {
    auto& layerData = mapData.layers.emplace_back();

    if (const auto err = _parse_layer(value, layerData, index);
        err != parse_error::none) {
      return err;
    }

    ++index;
  }

  return parse_error::none;
}

}  // namespace tactile::parsing