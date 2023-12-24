// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tiled-xml-format/emit/layer_emitter.hpp"

#include <sstream>  // stringstream

#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/io/tile_matrix_encoding.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/misc/conversion.hpp"
#include "tactile/tiled-xml-format/emit/object_emitter.hpp"
#include "tactile/tiled-xml-format/emit/property_emitter.hpp"

namespace tactile::tiled::tmx {
namespace {

using int_literals::operator""_uz;

void _append_common_layer_attributes(pugi::xml_node layer_node, const ir::Layer& layer)
{
  layer_node.append_attribute("id").set_value(layer.id);
  layer_node.append_attribute("name").set_value(layer.name.c_str());

  if (layer.opacity != 1.0f) {
    layer_node.append_attribute("opacity").set_value(layer.opacity);
  }

  if (!layer.visible) {
    layer_node.append_attribute("visible").set_value(0);
  }
}

void _append_tile_layer_base64_data_node(pugi::xml_node layer_node,
                                         const ir::Layer& layer,
                                         const ir::TileFormat& tile_format)
{
  auto data_node = layer_node.append_child("data");
  data_node.append_attribute("encoding").set_value("base64");

  switch (tile_format.compression) {
    case CompressionMode::kNone:
      // Do nothing
      break;

    case CompressionMode::kZlib:
      data_node.append_attribute("compression").set_value("zlib");
      break;

    case CompressionMode::kZstd:
      data_node.append_attribute("compression").set_value("zstd");
      break;

    default: throw Exception {"Invalid compression mode"};
  }

  if (const auto encoded_tile_data =
          base64_encode_tile_matrix(layer.tiles, tile_format.compression)) {
    data_node.text().set(encoded_tile_data->c_str());
  }
  else {
    TACTILE_LOG_ERROR("Could not emit Base64 tile matrix data: {}",
                      encoded_tile_data.error().message());
  }
}

void _append_tile_layer_csv_data_node(pugi::xml_node layer_node,
                                      const ir::Layer& layer,
                                      const SaveFormatWriteOptions& options)
{
  auto data_node = layer_node.append_child("data");
  data_node.append_attribute("encoding").set_value("csv");

  std::stringstream stream;

  if (options.fold_tile_layer_data) {
    stream << '\n';
  }

  const auto row_count = as_unsigned(layer.row_count);
  const auto col_count = as_unsigned(layer.col_count);
  for (auto row = 0_uz; row < row_count; ++row) {
    for (auto col = 0_uz; col < col_count; ++col) {
      if (col != 0_uz) {
        stream << ',';
      }

      stream << layer.tiles[row][col].value;
    }

    if (row < (row_count - 1_uz)) {
      stream << ',';
      if (options.fold_tile_layer_data) {
        stream << '\n';
      }
    }
  }

  data_node.text().set(stream.str().c_str());
}

}  // namespace

void append_tile_layer_node(pugi::xml_node map_node,
                            const ir::Layer& layer,
                            const ir::TileFormat& tile_format,
                            const SaveFormatWriteOptions& options)
{
  auto layer_node = map_node.append_child("layer");
  _append_common_layer_attributes(layer_node, layer);

  layer_node.append_attribute("width").set_value(layer.col_count);
  layer_node.append_attribute("height").set_value(layer.row_count);

  append_properties_node(layer_node, layer.meta.properties);

  switch (tile_format.encoding) {
    case TileEncoding::kPlainText:
      _append_tile_layer_csv_data_node(layer_node, layer, options);
      break;

    case TileEncoding::kBase64:
      _append_tile_layer_base64_data_node(layer_node, layer, tile_format);
      break;

    default: throw Exception {"Invalid tile encoding"};
  }
}

void append_object_layer_node(pugi::xml_node map_node, const ir::Layer& layer)
{
  auto layer_node = map_node.append_child("objectgroup");
  _append_common_layer_attributes(layer_node, layer);
  append_properties_node(layer_node, layer.meta.properties);

  for (const auto& object : layer.objects) {
    append_object_node(layer_node, object);
  }
}

void append_group_layer_node(pugi::xml_node map_node,
                             const ir::Map& map,
                             const ir::Layer& layer,
                             const SaveFormatWriteOptions& options)
{
  auto layer_node = map_node.append_child("group");
  _append_common_layer_attributes(layer_node, layer);
  append_properties_node(layer_node, layer.meta.properties);

  for (const auto& sublayer : layer.layers) {
    append_layer_node(layer_node, map, sublayer, options);
  }
}

void append_layer_node(pugi::xml_node map_node,
                       const ir::Map& map,
                       const ir::Layer& layer,
                       const SaveFormatWriteOptions& options)
{
  switch (layer.type) {
    case LayerType::kTileLayer: {
      append_tile_layer_node(map_node, layer, map.tile_format, options);
      break;
    }
    case LayerType::kObjectLayer: {
      append_object_layer_node(map_node, layer);
      break;
    }
    case LayerType::kGroupLayer: {
      append_group_layer_node(map_node, map, layer, options);
      break;
    }
    default: throw Exception {"Invalid layer type"};
  }
}

}  // namespace tactile::tiled::tmx
