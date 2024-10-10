// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled_tmj/tmj_format_layer_emitter.hpp"

#include <cstddef>  // size_t
#include <utility>  // move

#include <cppcodec/base64_default_rfc4648.hpp>

#include "tactile/base/document/layer_view.hpp"
#include "tactile/base/document/map_view.hpp"
#include "tactile/base/io/compress/compressor.hpp"
#include "tactile/base/io/tile_io.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/runtime/runtime.hpp"
#include "tactile/runtime/logging.hpp"
#include "tactile/tiled_tmj/tmj_format_meta_emitter.hpp"
#include "tactile/tiled_tmj/tmj_format_object_emitter.hpp"

namespace tactile {
namespace tmj_format_layer_emitter {

[[nodiscard]]
auto emit_tile_layer(const IRuntime& runtime,
                     const ILayerView& layer,
                     nlohmann::json& layer_json,
                     ByteStream& tile_bytes) -> std::expected<void, ErrorCode>
{
  const auto tile_encoding = layer.get_tile_encoding();
  const auto tile_compression = layer.get_tile_compression();
  const auto extent = layer.get_extent().value();

  layer_json["type"] = "tilelayer";
  layer_json["width"] = extent.cols;
  layer_json["height"] = extent.rows;

  if (tile_encoding == TileEncoding::kBase64) {
    layer_json["encoding"] = "base64";
  }

  if (tile_compression == CompressionFormat::kZlib) {
    layer_json["compression"] = "zlib";
  }
  else if (tile_compression == CompressionFormat::kZstd) {
    layer_json["compression"] = "zstd";
  }

  if (tile_encoding == TileEncoding::kBase64) {
    tile_bytes.clear();
    layer.write_tile_bytes(tile_bytes);

    if (tile_compression.has_value()) {
      const auto* compression_format = runtime.get_compression_format(*tile_compression);
      if (!compression_format) {
        runtime::log(LogLevel::kError, "Could not find suitable compression format");
        return std::unexpected {ErrorCode::kNotSupported};
      }

      auto compressed_tile_bytes = compression_format->compress(tile_bytes);
      if (!compressed_tile_bytes.has_value()) {
        return std::unexpected {compressed_tile_bytes.error()};
      }

      tile_bytes = std::move(*compressed_tile_bytes);
    }

    auto encoded_bytes = base64::encode(tile_bytes);
    layer_json["data"] = std::move(encoded_bytes);
  }
  else {
    auto tile_array = nlohmann::json::array();
    tile_array.get_ref<nlohmann::json::array_t&>().reserve(
        saturate_cast<std::size_t>(extent.rows * extent.cols));

    for (Extent2D::value_type row = 0; row < extent.rows; ++row) {
      for (Extent2D::value_type col = 0; col < extent.cols; ++col) {
        const Index2D index {.x = col, .y = row};
        const auto tile_id = layer.get_tile(index).value_or(kEmptyTile);
        tile_array.push_back(tile_id);
      }
    }

    layer_json["data"] = std::move(tile_array);
  }

  return {};
}

void emit_object_layer(const ILayerView& layer, nlohmann::json& layer_json)
{
  layer_json["type"] = "objectgroup";

  auto object_array = nlohmann::json::array();
  object_array.get_ref<nlohmann::json::array_t&>().reserve(layer.object_count());

  layer_json["objects"] = std::move(object_array);
}

void emit_group_layer(const ILayerView& layer, nlohmann::json& layer_json)
{
  layer_json["type"] = "group";

  auto layer_array = nlohmann::json::array();
  layer_array.get_ref<nlohmann::json::array_t&>().reserve(layer.layer_count());

  layer_json["layers"] = std::move(layer_array);
}

}  // namespace tmj_format_layer_emitter

auto emit_tiled_tmj_layer(const IRuntime& runtime,
                          const ILayerView& layer,
                          ByteStream& tile_byte_stream)
    -> std::expected<nlohmann::json, ErrorCode>
{
  auto layer_json = nlohmann::json::object();

  layer_json["id"] = layer.get_id();
  layer_json["opacity"] = layer.get_opacity();
  layer_json["visible"] = layer.is_visible();
  layer_json["x"] = 0;
  layer_json["y"] = 0;

  switch (layer.get_type()) {
    case LayerType::kTileLayer: {
      const auto emit_tile_layer_result =
          tmj_format_layer_emitter::emit_tile_layer(runtime,
                                                    layer,
                                                    layer_json,
                                                    tile_byte_stream);

      if (!emit_tile_layer_result) {
        return std::unexpected {emit_tile_layer_result.error()};
      }

      break;
    }
    case LayerType::kObjectLayer: {
      tmj_format_layer_emitter::emit_object_layer(layer, layer_json);
      break;
    }
    case LayerType::kGroupLayer: {
      tmj_format_layer_emitter::emit_group_layer(layer, layer_json);
      break;
    }
  }

  emit_tiled_tmj_metadata(layer.get_meta(), layer_json);

  return layer_json;
}

}  // namespace tactile
