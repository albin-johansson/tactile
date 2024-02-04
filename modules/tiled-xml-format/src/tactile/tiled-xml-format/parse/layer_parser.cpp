// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/parse/layer_parser.hpp"

#include <cstring>  // strcmp
#include <utility>  // move

#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/io/tile_matrix_encoding.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"
#include "tactile/foundation/misc/string_conversion.hpp"
#include "tactile/tiled-xml-format/common.hpp"
#include "tactile/tiled-xml-format/parse/meta_parser.hpp"
#include "tactile/tiled-xml-format/parse/object_parser.hpp"

using namespace std::string_literals;

namespace tactile::tiled::tmx {
namespace {

[[nodiscard]]
auto _collect_layer_nodes(const pugi::xml_node parent_node) -> Vector<pugi::xml_node>
{
  Vector<pugi::xml_node> layer_nodes {};

  for (const auto node : parent_node.children()) {
    const auto* node_name = node.name();
    if (std::strcmp(node_name, "layer") == 0 ||
        std::strcmp(node_name, "objectgroup") == 0 ||
        std::strcmp(node_name, "group") == 0) {
      layer_nodes.push_back(node);
    }
  }

  return layer_nodes;
}

}  // namespace

auto parse_layer_type(const pugi::xml_node layer_node) -> Result<LayerType>
{
  const auto* layer_node_name = layer_node.name();

  if (std::strcmp(layer_node_name, "layer") == 0) {
    return LayerType::kTileLayer;
  }
  else if (std::strcmp(layer_node_name, "objectgroup") == 0) {
    return LayerType::kObjectLayer;
  }
  else if (std::strcmp(layer_node_name, "group") == 0) {
    return LayerType::kGroupLayer;
  }

  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedLayerType));
}

auto parse_tile_layer_csv_data_node(const pugi::xml_node data_node,
                                    ir::Layer& layer,
                                    ir::TileFormat& tile_format) -> Result<void>
{
  tile_format.encoding = TileEncoding::kPlainText;
  tile_format.compression = CompressionMode::kNone;

  auto tile_matrix = make_tile_matrix(layer.row_count, layer.col_count);

  const auto* tile_data_text = data_node.text().get();
  const auto tile_tokens = str_split(tile_data_text, ',');

  usize tile_index = 0;
  const auto column_count = as_unsigned(layer.col_count);

  for (const auto& tile_token : tile_tokens) {
    if (const auto tile_id = str_to<TileID::value_type>(tile_token)) {
      const auto [row, col] = to_matrix_index(tile_index, column_count);
      tile_matrix[row][col] = TileID {*tile_id};

      ++tile_index;
    }
    else {
      return unexpected(make_save_format_error(SaveFormatError::kBadTileLayerData));
    }
  }

  layer.tiles = std::move(tile_matrix);

  return kOK;
}

auto parse_tile_layer_base64_data_node(const pugi::xml_node data_node,
                                       ir::Layer& layer,
                                       ir::TileFormat& tile_format) -> Result<void>
{
  tile_format.encoding = TileEncoding::kBase64;

  const auto* compression_name = data_node.attribute("compression").as_string(nullptr);
  if (compression_name == nullptr || std::strcmp(compression_name, "") == 0) {
    tile_format.compression = CompressionMode::kNone;
  }
  else if (std::strcmp(compression_name, "zlib") == 0) {
    tile_format.compression = CompressionMode::kZlib;
  }
  else if (std::strcmp(compression_name, "zstd") == 0) {
    tile_format.compression = CompressionMode::kZstd;
  }
  else {
    return unexpected(make_save_format_error(SaveFormatError::kUnsupportedCompression));
  }

  const auto* tile_data_text = data_node.text().get();
  return base64_decode_tile_matrix(tile_data_text,
                                   MatrixExtent {layer.row_count, layer.col_count},
                                   tile_format.compression,
                                   TileEncodingFormat::kTiled)
      .and_then([&](TileMatrix&& tile_matrix) {
        layer.tiles = std::move(tile_matrix);
        return kOK;
      });
}

auto parse_tile_layer_verbose_data_node(const pugi::xml_node data_node,
                                        ir::Layer& layer,
                                        ir::TileFormat& tile_format) -> Result<void>
{
  tile_format.encoding = TileEncoding::kPlainText;
  tile_format.compression = CompressionMode::kNone;

  auto tile_matrix = make_tile_matrix(MatrixExtent {layer.row_count, layer.col_count});

  usize tile_index = 0;
  const auto column_count = as_unsigned(layer.col_count);

  const auto tile_node_range = data_node.children("tile");
  for (const auto tile_node : tile_node_range) {
    const auto tile_id = tile_node.attribute("gid").as_int(kEmptyTile.value);

    const auto [row, col] = to_matrix_index(tile_index, column_count);
    tile_matrix[row][col] = TileID {tile_id};

    ++tile_index;
  }

  layer.tiles = std::move(tile_matrix);
  return kOK;
}

auto parse_tile_layer_data_node(const pugi::xml_node layer_node,
                                ir::Layer& layer,
                                ir::TileFormat& tile_format) -> Result<void>
{
  const auto data_node = layer_node.child("data");
  if (data_node.empty() || data_node.text().empty()) {
    return unexpected(make_save_format_error(SaveFormatError::kBadTileLayerData));
  }

  if (const auto* encoding_name = data_node.attribute("encoding").as_string(nullptr)) {
    if (std::strcmp(encoding_name, "csv") == 0) {
      return parse_tile_layer_csv_data_node(data_node, layer, tile_format);
    }
    else if (std::strcmp(encoding_name, "base64") == 0) {
      return parse_tile_layer_base64_data_node(data_node, layer, tile_format);
    }
    else {
      return unexpected(
          make_save_format_error(SaveFormatError::kUnsupportedTileEncoding));
    }
  }
  else {
    return parse_tile_layer_verbose_data_node(data_node, layer, tile_format);
  }
}

auto parse_tile_layer_node(const pugi::xml_node layer_node,
                           ir::Layer& layer,
                           ir::TileFormat& tile_format) -> Result<void>
{
  return parse_to(layer_node, "width", layer.col_count)
      .and_then([&] { return parse_to(layer_node, "height", layer.row_count); })
      .and_then(
          [&] { return parse_tile_layer_data_node(layer_node, layer, tile_format); });
}

auto parse_object_layer_node(const pugi::xml_node layer_node, ir::Layer& layer)
    -> Result<void>
{
  if (auto objects = parse_objects(layer_node)) {
    layer.objects = std::move(*objects);
    return kOK;
  }
  else {
    return propagate_unexpected(objects);
  }
}

auto parse_group_layer_node(const pugi::xml_node layer_node,
                            ir::Layer& layer,
                            ir::TileFormat& tile_format,
                            const SaveFormatReadOptions& options) -> Result<void>
{
  const auto sublayer_nodes = _collect_layer_nodes(layer_node);
  layer.layers.reserve(sublayer_nodes.size());

  for (const auto sublayer_node : sublayer_nodes) {
    if (auto sublayer = parse_layer_node(sublayer_node, tile_format, options)) {
      layer.layers.push_back(std::move(*sublayer));
    }
    else {
      return propagate_unexpected(sublayer);
    }
  }

  return kOK;
}

auto parse_layer_node(const pugi::xml_node layer_node,
                      ir::TileFormat& tile_format,
                      const SaveFormatReadOptions& options) -> Result<ir::Layer>
{
  ir::Layer layer {};

  return parse_to(layer_node, "id", layer.id)
      // clang-format off
      .and_then([&] { return try_parse_to(layer_node, "name", layer.name, "Layer"s); })
      .and_then([&] { return try_parse_to(layer_node, "opacity", layer.opacity, 1.0f); })
      .and_then([&] { return try_parse_to(layer_node, "visible", layer.visible, true); })
      .and_then([&] { return parse_layer_type(layer_node); })
      // clang-format on
      .and_then([&](const LayerType type) -> Result<void> {
        layer.type = type;

        switch (layer.type) {
          case LayerType::kTileLayer:
            return parse_tile_layer_node(layer_node, layer, tile_format);

          case LayerType::kObjectLayer: return parse_object_layer_node(layer_node, layer);

          case LayerType::kGroupLayer:
            return parse_group_layer_node(layer_node, layer, tile_format, options);
        }

        return unexpected(make_save_format_error(SaveFormatError::kUnsupportedLayerType));
      })
      .and_then([&] { return parse_metadata(layer_node, layer.meta); })
      .transform([&] { return std::move(layer); });
}

auto parse_layers(const pugi::xml_node map_node,
                  ir::TileFormat& tile_format,
                  const SaveFormatReadOptions& options) -> Result<Vector<ir::Layer>>
{
  Vector<ir::Layer> layers {};

  const auto layer_nodes = _collect_layer_nodes(map_node);
  layers.reserve(layer_nodes.size());

  for (const auto layer_node : layer_nodes) {
    if (auto layer = parse_layer_node(layer_node, tile_format, options)) {
      layers.push_back(std::move(*layer));
    }
    else {
      return propagate_unexpected(layer);
    }
  }

  return layers;
}

}  // namespace tactile::tiled::tmx
