// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tmj-format/load/tmj_layer_parser.hpp"

#include <utility>  // move

#include "tactile/core/container/hash_map.hpp"
#include "tactile/core/container/lookup.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/io/save/save_format_error.hpp"
#include "tactile/core/io/tile_matrix_encoding.hpp"
#include "tactile/tmj-format/load/tmj_meta_parser.hpp"

namespace tactile::tmj {
namespace {

inline const HashMap<StringView, LayerType> kLayerTypeNames = {
  {"tilelayer", LayerType::kTileLayer},
  {"objectgroup", LayerType::kObjectLayer},
  {"group", LayerType::kGroupLayer},
};

[[nodiscard]]
auto _parse_layer_type(const JSON& layer_json, LayerType& layer_type) -> Result<void>
{
  String type_name {};
  return parse(layer_json, "type", type_name).and_then([&] -> Result<void> {
    if (const auto* found_type = find_in(kLayerTypeNames, type_name)) {
      layer_type = *found_type;
      return kSuccess;
    }

    return error(SaveFormatError::kUnsupportedLayerType);
  });
}

[[nodiscard]]
auto _parse_tile_encoding(const JSON& layer_json, ir::TileFormat& tile_format)
    -> Result<void>
{
  String encoding_name {};
  return parse(layer_json, "encoding", encoding_name, "csv")
      .and_then([&] -> Result<void> {
        if (encoding_name == "csv") {
          tile_format.encoding = TileEncoding::kPlainText;
        }
        else if (encoding_name == "base64") {
          tile_format.encoding = TileEncoding::kBase64;
        }
        else {
          TACTILE_LOG_ERROR("[TMJ] Parsed unsupported tile format encoding '{}'",
                            encoding_name);
          return error(SaveFormatError::kUnsupportedTileEncoding);
        }

        return kSuccess;
      });
}

[[nodiscard]]
auto _parse_compression_mode(const JSON& layer_json, ir::TileFormat& tile_format)
    -> Result<void>
{
  String compression_name {};
  return parse(layer_json, "compression", compression_name)  //
      .and_then([&] -> Result<void> {
        if (compression_name.empty()) {
          tile_format.compression = CompressionMode::kNone;
        }
        else if (compression_name == "zlib") {
          tile_format.compression = CompressionMode::kZlib;
        }
        else if (compression_name == "zstd") {
          tile_format.compression = CompressionMode::kZstd;
        }
        else {
          return error(SaveFormatError::kBadCompressionMode);
        }

        return kSuccess;
      });
}

[[nodiscard]]
auto _parse_tile_layer_csv_data(const JSON& tile_data_json, const MatrixExtent& extent)
    -> Result<TileMatrix>
{
  TACTILE_ASSERT(tile_data_json.is_array());

  auto tile_matrix = make_tile_matrix(extent);

  usize tile_index = 0;
  for (const auto& [_, value_json] : tile_data_json.items()) {
    if (value_json.is_number_integer()) {
      const auto [row, col] = to_matrix_index(tile_index, extent.col_count);
      tile_matrix[row][col] = value_json.get<TileID>();

      ++tile_index;
    }
    else {
      return error(SaveFormatError::kBadTileLayerData);
    }
  }

  return tile_matrix;
}

[[nodiscard]]
auto _parse_tile_layer_base64_data(const CompressionMode compression_mode,
                                   const String& tile_data,
                                   const MatrixExtent& extent) -> Result<TileMatrix>
{
  return base64_decode_tile_matrix(tile_data,
                                   extent,
                                   compression_mode,
                                   TileEncodingFormat::kTiled);
}

[[nodiscard]]
auto _parse_tile_layer_data(const JSON& layer_json,
                            ir::TileFormat& tile_format,
                            ir::Layer& layer) -> Result<void>
{
  JSON tile_data_json {};

  return _parse_tile_encoding(layer_json, tile_format)
      .and_then([&] { return _parse_compression_mode(layer_json, tile_format); })
      .and_then([&] { return parse(layer_json, "width", layer.width); })
      .and_then([&] { return parse(layer_json, "height", layer.height); })
      .and_then([&] { return parse(layer_json, "data", tile_data_json); })
      .and_then([&] -> Result<TileMatrix> {
        const MatrixExtent layer_extent {static_cast<usize>(layer.height),
                                         static_cast<usize>(layer.width)};

        switch (tile_format.encoding) {
          case TileEncoding::kPlainText:
            return _parse_tile_layer_csv_data(tile_data_json, layer_extent);

          case TileEncoding::kBase64:
            return _parse_tile_layer_base64_data(tile_format.compression,
                                                 tile_data_json.get<String>(),
                                                 layer_extent);
        }

        return error(SaveFormatError::kUnsupportedTileEncoding);
      })
      .and_then([&](TileMatrix tile_matrix) {
        layer.tiles = std::move(tile_matrix);
        return kSuccess;
      });
}

[[nodiscard]]
auto _parse_object_layer_data(const JSON& layer_json, ir::Layer& object_layer)
    -> Result<void>
{
  const auto objects_iter = layer_json.find("objects");

  if (objects_iter != layer_json.end()) {
    object_layer.objects.reserve(objects_iter->size());

    for (const auto& [_, object_json] : objects_iter->items()) {
      if (auto object = parse_object(object_json)) {
        object_layer.objects.push_back(std::move(*object));
      }
      else {
        return propagate_unexpected(object);
      }
    }
  }

  return kSuccess;
}

[[nodiscard]]
auto _parse_group_layer_data(const JSON& layer_json, ir::Map& map, ir::Layer& group_layer)
    -> Result<void>
{
  const auto layers_iter = layer_json.find("layers");
  if (layers_iter == layer_json.end()) {
    TACTILE_LOG_WARN("[TMJ] Detected group layer with no 'layers' attribute");
    return kSuccess;
  }

  for (const auto& [_, sublayer_json] : layers_iter->items()) {
    if (auto sublayer = parse_layer(sublayer_json, map)) {
      group_layer.layers.push_back(std::move(*sublayer));
    }
    else {
      return propagate_unexpected(sublayer);
    }
  }

  return kSuccess;
}

}  // namespace

auto parse_object(const JSON& object_json) -> Result<ir::Object>
{
  ir::Object object {};

  if (object_json.contains("point")) {
    object.type = ObjectType::kPoint;
  }
  else if (object_json.contains("ellipse")) {
    object.type = ObjectType::kEllipse;
  }
  else {
    object.type = ObjectType::kRect;
  }

  return parse(object_json, "id", object.id)
      .and_then([&] { return parse(object_json, "name", object.meta.name, ""); })
      .and_then([&] { return parse(object_json, "type", object.tag, ""); })
      .and_then([&] { return parse(object_json, "x", object.x, 0.0f); })
      .and_then([&] { return parse(object_json, "y", object.y, 0.0f); })
      .and_then([&] { return parse(object_json, "width", object.width, 0.0f); })
      .and_then([&] { return parse(object_json, "height", object.height, 0.0f); })
      .and_then([&] { return parse(object_json, "visible", object.visible, true); })
      .and_then([&] { return parse_metadata(object_json, object.meta); })
      .transform([&] { return std::move(object); });
}

auto parse_layer(const JSON& layer_json, ir::Map& map) -> Result<ir::Layer>
{
  ir::Layer layer {};

  return parse(layer_json, "id", layer.id)
      .and_then([&] { return parse(layer_json, "name", layer.meta.name, "Layer"); })
      .and_then([&] { return parse(layer_json, "opacity", layer.opacity, 1.0f); })
      .and_then([&] { return parse(layer_json, "visible", layer.visible, true); })
      .and_then([&] { return _parse_layer_type(layer_json, layer.type); })
      .and_then([&] {
        switch (layer.type) {
          case LayerType::kTileLayer:
            return _parse_tile_layer_data(layer_json, map.tile_format, layer);

          case LayerType::kObjectLayer:
            return _parse_object_layer_data(layer_json, layer);

          case LayerType::kGroupLayer:
            return _parse_group_layer_data(layer_json, map, layer);
        }

        TACTILE_LOG_WARN("[TMJ] Could not recognize type of layer {}", layer.id);
        return kSuccess;
      })
      .and_then([&] { return parse_metadata(layer_json, layer.meta); })
      .transform([&] { return std::move(layer); });
}

auto parse_layers(const JSON& map_json, ir::Map& map) -> Result<void>
{
  const auto layers_iter = map_json.find("layers");
  if (layers_iter == map_json.end()) {
    // TODO strict mode?
    TACTILE_LOG_WARN("[TMJ] Map is missing required 'layers' attribute");
    return kSuccess;
  }

  map.layers.reserve(layers_iter->size());

  for (const auto& [_, layer_json] : layers_iter->items()) {
    if (auto layer = parse_layer(layer_json, map)) {
      map.layers.push_back(std::move(*layer));
    }
    else {
      return propagate_unexpected(layer);
    }
  }

  return kSuccess;
}

}  // namespace tactile::tmj
