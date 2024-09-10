// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj_format/tmj_format_layer_parser.hpp"

#include <cstddef>   // size_t
#include <iterator>  // distance
#include <utility>   // move

#include <cppcodec/base64_default_rfc4648.hpp>

#include "tactile/base/io/compress/compressor.hpp"
#include "tactile/base/io/tile_io.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/runtime.hpp"
#include "tactile/base/util/matrix_index.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj_format/tmj_format_attribute_parser.hpp"
#include "tactile/tiled_tmj_format/tmj_format_object_parser.hpp"

namespace tactile {
namespace tmj_format_layer_parser {

[[nodiscard]]
auto parse_type(const nlohmann::json& layer_json)
    -> std::expected<LayerType, SaveFormatParseError>
{
  const auto type_iter = layer_json.find("type");
  if (type_iter == layer_json.end()) {
    return std::unexpected {SaveFormatParseError::kNoLayerType};
  }

  const auto* type_name = type_iter->get_ptr<const std::string*>();
  if (type_name == nullptr) {
    return std::unexpected {SaveFormatParseError::kBadLayerType};
  }

  if (*type_name == "tilelayer") {
    return LayerType::kTileLayer;
  }

  if (*type_name == "objectgroup") {
    return LayerType::kObjectLayer;
  }

  if (*type_name == "group") {
    return LayerType::kGroupLayer;
  }

  return std::unexpected {SaveFormatParseError::kBadLayerType};
}

[[nodiscard]]
auto parse_base64_tile_data(const IRuntime& runtime,
                            const nlohmann::json& data_json,
                            const std::optional<CompressionFormat> compression,
                            ir::Layer& layer) -> SaveFormatParseResult<void>
{
  const auto& encoded_tile_data = data_json.get_ref<const std::string&>();
  auto decoded_bytes = base64::decode(encoded_tile_data);

  if (compression.has_value()) {
    const auto* compression_format = runtime.get_compression_format(*compression);
    if (!compression_format) {
      return std::unexpected {SaveFormatParseError::kNoSuchCompressionFormat};
    }

    auto decompressed_bytes = compression_format->decompress(decoded_bytes);
    if (!decompressed_bytes.has_value()) {
      log(LogLevel::kError,
          "Could not decompress tile data: {}",
          decompressed_bytes.error().message());
      return std::unexpected {SaveFormatParseError::kBadTileLayerData};
    }

    decoded_bytes = std::move(*decompressed_bytes);
  }

  auto tile_matrix = parse_raw_tile_matrix(decoded_bytes, layer.extent, TileIdFormat::kTiled);
  if (!tile_matrix.has_value()) {
    return std::unexpected {SaveFormatParseError::kBadTileLayerData};
  }

  layer.tiles = std::move(*tile_matrix);
  return kParseResultOK;
}

[[nodiscard]]
auto parse_csv_tile_data(const nlohmann::json& data_json,
                         ir::Layer& layer) -> SaveFormatParseResult<void>
{
  if (!data_json.is_array()) {
    return std::unexpected {SaveFormatParseError::kBadTileLayerData};
  }

  const auto tile_items = data_json.items();
  const auto real_tile_count = std::distance(tile_items.begin(), tile_items.end());
  const auto expected_tile_count = layer.extent.rows * layer.extent.cols;

  if (real_tile_count != expected_tile_count) {
    log(LogLevel::kError,
        "Bad tile layer tile count, expected {} but got {}",
        expected_tile_count,
        real_tile_count);
    return std::unexpected {SaveFormatParseError::kBadTileLayerData};
  }

  MatrixIndex::value_type index = 0;
  for (const auto& [_, tile_json] : tile_items) {
    const auto [row, col] = make_matrix_index(index, layer.extent.cols);
    tile_json.get_to(
        layer.tiles[saturate_cast<std::size_t>(row)][saturate_cast<std::size_t>(col)]);

    ++index;
  }

  return kParseResultOK;
}

[[nodiscard]]
auto parse_tile_layer(const IRuntime& runtime,
                      const nlohmann::json& layer_json,
                      ir::Layer& layer) -> SaveFormatParseResult<void>
{
  if (const auto width_iter = layer_json.find("width"); width_iter != layer_json.end()) {
    width_iter->get_to(layer.extent.cols);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTileLayerWidth};
  }

  if (const auto height_iter = layer_json.find("height"); height_iter != layer_json.end()) {
    height_iter->get_to(layer.extent.rows);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoTileLayerHeight};
  }

  std::string encoding {"csv"};
  if (const auto encoding_iter = layer_json.find("encoding");
      encoding_iter != layer_json.end()) {
    encoding_iter->get_to(encoding);
  }

  std::optional<CompressionFormat> compression {};
  if (const auto compression_iter = layer_json.find("compression");
      compression_iter != layer_json.end()) {
    const auto& compression_name = compression_iter->get_ref<const std::string&>();
    if (compression_name == "zlib") {
      compression = CompressionFormat::kZlib;
    }
    else if (compression_name == "zstd") {
      compression = CompressionFormat::kZstd;
    }
    else {
      log(LogLevel::kError, "Invalid tile compression format: {}", compression_name);
      return std::unexpected {SaveFormatParseError::kNoSuchCompressionFormat};
    }
  }

  const auto data_iter = layer_json.find("data");
  if (data_iter == layer_json.end()) {
    return std::unexpected {SaveFormatParseError::kNoTileLayerData};
  }

  layer.tiles = make_tile_matrix(layer.extent);

  if (encoding == "csv") {
    const auto parse_tiles_result = parse_csv_tile_data(*data_iter, layer);
    if (!parse_tiles_result.has_value()) {
      return std::unexpected {parse_tiles_result.error()};
    }
  }
  else if (encoding == "base64") {
    const auto parse_tiles_result =
        parse_base64_tile_data(runtime, *data_iter, compression, layer);
    if (!parse_tiles_result.has_value()) {
      return std::unexpected {parse_tiles_result.error()};
    }
  }
  else {
    log(LogLevel::kError, "Invalid tile layer encoding: {}", encoding);
    return std::unexpected {SaveFormatParseError::kBadTileLayerEncoding};
  }

  return kParseResultOK;
}

[[nodiscard]]
auto parse_group_layer(const IRuntime& runtime,
                       const nlohmann::json& layer_json,
                       ir::Layer& layer) -> SaveFormatParseResult<void>
{
  const auto layers_iter = layer_json.find("layers");
  if (layers_iter == layer_json.end()) {
    return std::unexpected {SaveFormatParseError::kNoGroupLayerData};
  }

  layer.layers.reserve(layers_iter->size());

  for (const auto& [_, sublayer_json] : layers_iter->items()) {
    if (auto sublayer = parse_tiled_tmj_layer(runtime, sublayer_json)) {
      layer.layers.push_back(std::move(*sublayer));
    }
    else {
      return std::unexpected {sublayer.error()};
    }
  }

  return kParseResultOK;
}

}  // namespace tmj_format_layer_parser

auto parse_tiled_tmj_object_layer(const nlohmann::json& layer_json,
                                  ir::Layer& layer) -> SaveFormatParseResult<void>
{
  const auto objects_iter = layer_json.find("objects");

  if (objects_iter != layer_json.end()) {
    layer.objects.reserve(objects_iter->size());

    for (const auto& [_, object_json] : objects_iter->items()) {
      if (auto object = parse_tiled_tmj_object(object_json)) {
        layer.objects.push_back(std::move(*object));
      }
      else {
        return std::unexpected {object.error()};
      }
    }
  }

  return kParseResultOK;
}

auto parse_tiled_tmj_layer(const IRuntime& runtime, const nlohmann::json& layer_json)
    -> SaveFormatParseResult<ir::Layer>
{
  ir::Layer layer {};

  if (auto metadata = parse_tiled_tmj_metadata(layer_json)) {
    layer.meta = std::move(*metadata);
  }
  else {
    return std::unexpected {metadata.error()};
  }

  if (!layer_json.contains("name")) {
    return std::unexpected {SaveFormatParseError::kNoLayerName};
  }

  if (const auto id_iter = layer_json.find("id"); id_iter != layer_json.end()) {
    id_iter->get_to(layer.id);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoLayerId};
  }

  if (const auto opacity_iter = layer_json.find("opacity"); opacity_iter != layer_json.end()) {
    opacity_iter->get_to(layer.opacity);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoLayerOpacity};
  }

  if (const auto visible_iter = layer_json.find("visible"); visible_iter != layer_json.end()) {
    visible_iter->get_to(layer.visible);
  }
  else {
    return std::unexpected {SaveFormatParseError::kNoLayerVisibility};
  }

  const auto layer_type = tmj_format_layer_parser::parse_type(layer_json);
  if (!layer_type.has_value()) {
    return std::unexpected {layer_type.error()};
  }

  layer.type = *layer_type;

  switch (layer.type) {
    case LayerType::kTileLayer: {
      const auto result =
          tmj_format_layer_parser::parse_tile_layer(runtime, layer_json, layer);

      if (!result.has_value()) {
        return std::unexpected {result.error()};
      }

      break;
    }
    case LayerType::kObjectLayer: {
      const auto result = parse_tiled_tmj_object_layer(layer_json, layer);

      if (!result.has_value()) {
        return std::unexpected {result.error()};
      }

      break;
    }
    case LayerType::kGroupLayer: {
      const auto result =
          tmj_format_layer_parser::parse_group_layer(runtime, layer_json, layer);

      if (!result.has_value()) {
        return std::unexpected {result.error()};
      }

      break;
    }
  }

  return layer;
}

}  // namespace tactile
