// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-json-format/emit/tmj_layer_emitter.hpp"

#include <utility>  // move

#include "tactile/foundation/io/tile_matrix_encoding.hpp"
#include "tactile/tiled-json-format/emit/tmj_meta_emitter.hpp"
#include "tactile/tiled-json-format/emit/tmj_object_emitter.hpp"

namespace tactile::tiled::tmj {
namespace {

void _emit_object_layer(JSON& layer_json, const ir::Layer& layer)
{
  layer_json["type"] = "objectgroup";
  layer_json["objects"] = emit_object_array(layer.objects);
}

[[nodiscard]]
auto _emit_tile_layer(JSON& layer_json,
                      const ir::Layer& layer,
                      const ir::TileFormat& tile_format) -> Result<void>
{
  layer_json["type"] = "tilelayer";
  layer_json["width"] = layer.width;
  layer_json["height"] = layer.height;

  if (tile_format.compression == CompressionMode::kZlib) {
    layer_json["compression"] = "zlib";
  }
  else if (tile_format.compression == CompressionMode::kZstd) {
    layer_json["compression"] = "zstd";
  }
  else {
    layer_json["compression"] = String {};
  }

  if (tile_format.encoding == TileEncoding::kBase64) {
    layer_json["encoding"] = "base64";

    if (auto encoded_tiles =
            base64_encode_tile_matrix(layer.tiles, tile_format.compression)) {
      layer_json["data"] = std::move(*encoded_tiles);
    }
    else {
      return propagate_unexpected(encoded_tiles);
    }
  }
  else {
    auto tile_data_json = JSON::array();

    for (usize row = 0; row < layer.height; ++row) {
      for (usize col = 0; col < layer.width; ++col) {
        tile_data_json += JSON(layer.tiles[row][col].value);
      }
    }

    layer_json["data"] = std::move(tile_data_json);
  }

  return kSuccess;
}

[[nodiscard]]
auto _emit_group_layer(JSON& layer_json,
                       const ir::Layer& layer,
                       const ir::TileFormat& tile_format) -> Result<void>
{
  layer_json["type"] = "group";

  auto layer_json_array = JSON::array();

  for (const auto& sublayer : layer.layers) {
    if (auto sublayer_json = emit_layer(sublayer, tile_format)) {
      layer_json_array += std::move(*sublayer_json);
    }
    else {
      return propagate_unexpected(sublayer_json);
    }
  }

  layer_json["layers"] = std::move(layer_json_array);

  return kSuccess;
}

}  // namespace

auto emit_layer(const ir::Layer& layer, const ir::TileFormat& tile_format) -> Result<JSON>
{
  auto layer_json = JSON::object();

  layer_json["id"] = layer.id;
  layer_json["name"] = layer.name;
  layer_json["opacity"] = layer.opacity;
  layer_json["visible"] = layer.visible;
  layer_json["x"] = 0;
  layer_json["y"] = 0;
  layer_json["properties"] = emit_property_array(layer.meta);

  switch (layer.type) {
    case LayerType::kTileLayer: {
      const auto result = _emit_tile_layer(layer_json, layer, tile_format);

      if (!result.has_value()) {
        return propagate_unexpected(result);
      }

      break;
    }
    case LayerType::kObjectLayer: {
      _emit_object_layer(layer_json, layer);
      break;
    }
    case LayerType::kGroupLayer: {
      const auto result = _emit_group_layer(layer_json, layer, tile_format);

      if (!result.has_value()) {
        return propagate_unexpected(result);
      }

      break;
    }
  }

  return layer_json;
}

auto emit_layer_array(const ir::Map& map) -> Result<JSON>
{
  auto layer_json_array = JSON::array();

  for (const auto& layer : map.layers) {
    if (auto layer_json = emit_layer(layer, map.tile_format)) {
      layer_json_array += std::move(*layer_json);
    }
    else {
      return propagate_unexpected(layer_json);
    }
  }

  return layer_json_array;
}

}  // namespace tactile::tiled::tmj