// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_map_parser.hpp"

#include <utility>  // move

#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj/tmj_format_attribute_parser.hpp"
#include "tactile/tiled_tmj/tmj_format_layer_parser.hpp"
#include "tactile/tiled_tmj/tmj_format_tileset_parser.hpp"

namespace tactile {
namespace {

void _deduce_tile_format_from_layer(const nlohmann::json& layer_json,
                                    ir::TileFormat& tile_format)
{
  if (const auto encoding_iter = layer_json.find("encoding");
      encoding_iter != layer_json.end()) {
    const auto& encoding = encoding_iter->get_ref<const std::string&>();

    if (encoding == "base64") {
      tile_format.encoding = TileEncoding::kBase64;
    }
    else {
      tile_format.encoding = TileEncoding::kPlainText;
    }
  }

  if (const auto compression_iter = layer_json.find("compression");
      compression_iter != layer_json.end()) {
    const auto& compression_name = compression_iter->get_ref<const std::string&>();

    if (compression_name == "zlib") {
      tile_format.compression = CompressionFormatId::kZlib;
    }
    else if (compression_name == "zstd") {
      tile_format.compression = CompressionFormatId::kZstd;
    }
    else {
      tile_format.compression = std::nullopt;
    }
  }
}

}  // namespace

auto parse_tiled_tmj_map(const IRuntime& runtime,
                         const nlohmann::json& map_json,
                         const SaveFormatReadOptions& options)
    -> std::expected<ir::Map, ErrorCode>
{
  ir::Map map {};

  if (const auto orientation_iter = map_json.find("orientation");
      orientation_iter != map_json.end()) {
    const auto& orientation = orientation_iter->get_ref<const std::string&>();
    if (orientation != "orthogonal") {
      runtime::log(LogLevel::kError, "Unsupported map orientation");
      return std::unexpected {ErrorCode::kNotSupported};
    }
  }
  else {
    runtime::log(LogLevel::kError, "Could not parse map orientation");
    return std::unexpected {ErrorCode::kParseError};
  }

  if (auto metadata = parse_tiled_tmj_metadata(map_json)) {
    map.meta = std::move(*metadata);
  }
  else {
    return std::unexpected {metadata.error()};
  }

  if (const auto width_iter = map_json.find("width"); width_iter != map_json.end()) {
    width_iter->get_to(map.extent.cols);
  }
  else {
    runtime::log(LogLevel::kError, "Could not parse map width");
    return std::unexpected {ErrorCode::kParseError};
  }

  if (const auto height_iter = map_json.find("height"); height_iter != map_json.end()) {
    height_iter->get_to(map.extent.rows);
  }
  else {
    runtime::log(LogLevel::kError, "Could not parse map height");
    return std::unexpected {ErrorCode::kParseError};
  }

  if (const auto tile_width_iter = map_json.find("tilewidth");
      tile_width_iter != map_json.end()) {
    tile_width_iter->get_to(map.tile_size[0]);
  }
  else {
    runtime::log(LogLevel::kError, "Could not parse map tile width");
    return std::unexpected {ErrorCode::kParseError};
  }

  if (const auto tile_height_iter = map_json.find("tileheight");
      tile_height_iter != map_json.end()) {
    tile_height_iter->get_to(map.tile_size[1]);
  }
  else {
    runtime::log(LogLevel::kError, "Could not parse map tile height");
    return std::unexpected {ErrorCode::kParseError};
  }

  if (const auto next_layer_id_iter = map_json.find("nextlayerid");
      next_layer_id_iter != map_json.end()) {
    next_layer_id_iter->get_to(map.next_layer_id);
  }
  else {
    runtime::log(LogLevel::kError, "Could not parse map next layer identifier");
    return std::unexpected {ErrorCode::kParseError};
  }

  if (const auto next_object_id_iter = map_json.find("nextobjectid");
      next_object_id_iter != map_json.end()) {
    next_object_id_iter->get_to(map.next_object_id);
  }
  else {
    runtime::log(LogLevel::kError, "Could not parse map next object identifier");
    return std::unexpected {ErrorCode::kParseError};
  }

  if (const auto tilesets_iter = map_json.find("tilesets"); tilesets_iter != map_json.end()) {
    map.tilesets.reserve(tilesets_iter->size());

    for (const auto& [_, tileset_json] : tilesets_iter->items()) {
      if (auto tileset = parse_tiled_tmj_tileset(tileset_json, options)) {
        map.tilesets.push_back(std::move(*tileset));
      }
      else {
        return std::unexpected {tileset.error()};
      }
    }
  }

  if (const auto layers_iter = map_json.find("layers"); layers_iter != map_json.end()) {
    map.layers.reserve(layers_iter->size());

    for (const auto& [_, layer_json] : layers_iter->items()) {
      _deduce_tile_format_from_layer(layer_json, map.tile_format);

      if (auto layer = parse_tiled_tmj_layer(runtime, layer_json)) {
        map.layers.push_back(std::move(*layer));
      }
      else {
        return std::unexpected {layer.error()};
      }
    }
  }

  return map;
}

}  // namespace tactile
