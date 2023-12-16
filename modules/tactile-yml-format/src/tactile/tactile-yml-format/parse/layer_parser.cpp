// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/parse/layer_parser.hpp"

#include <algorithm>  // replace
#include <utility>    // move

#include "tactile/foundation/io/tile_matrix_encoding.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/parse/common.hpp"
#include "tactile/tactile-yml-format/parse/meta_parser.hpp"
#include "tactile/tactile-yml-format/parse/object_parser.hpp"

namespace tactile::yml_format {

auto parse_layer_type(const StringView type_name) -> Result<LayerType>
{
  if (type_name == "tile-layer") {
    return LayerType::kTileLayer;
  }
  else if (type_name == "object-layer") {
    return LayerType::kObjectLayer;
  }
  else if (type_name == "group-layer") {
    return LayerType::kGroupLayer;
  }

  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedLayerType));
}

auto parse_plain_text_tile_matrix(String tile_data, const MatrixExtent matrix_extent)
    -> Result<TileMatrix>
{
  // TODO performance: version of str_split that returns string views?
  std::replace(tile_data.begin(), tile_data.end(), '\n', ' ');
  const auto tile_tokens = str_split(tile_data, ' ');

  auto tile_matrix = make_tile_matrix(matrix_extent.row_count, matrix_extent.col_count);

  usize index = 0;
  for (const auto& tile_token : tile_tokens) {
    if (const auto tile_id = str_to_i32(tile_token)) {
      const auto [row, col] = to_matrix_index(index, matrix_extent.col_count);
      tile_matrix[row][col] = TileID {*tile_id};
    }
    else {
      return unexpected(make_save_format_error(SaveFormatError::kBadTileLayerData));
    }

    ++index;
  }

  return tile_matrix;
}

auto parse_tile_matrix(const ir::TileFormat& tile_format,
                       String&& tile_data,
                       ir::Layer& layer) -> Result<TileMatrix>
{
  const MatrixExtent matrix_extent {layer.height, layer.width};

  if (tile_format.encoding == TileEncoding::kPlainText) {
    return parse_plain_text_tile_matrix(std::move(tile_data), matrix_extent);
  }
  else if (tile_format.encoding == TileEncoding::kBase64) {
    return base64_decode_tile_matrix(tile_data,
                                     matrix_extent,
                                     tile_format.compression,
                                     TileEncodingFormat::kTactile);
  }

  return unexpected(make_save_format_error(SaveFormatError::kUnsupportedTileEncoding));
}

auto parse_tile_layer_data(const YAML::Node& layer_node,
                           const ir::Map& map,
                           ir::Layer& layer) -> Result<void>
{
  layer.width = map.col_count;
  layer.height = map.row_count;

  return parse<String>(layer_node, "data")
      .and_then([&](String&& tile_data) {
        return parse_tile_matrix(map.tile_format, std::move(tile_data), layer);
      })
      .and_then([&](TileMatrix&& tile_matrix) {
        layer.tiles = std::move(tile_matrix);
        return kOK;
      });
}

auto parse_object_layer_data(const YAML::Node& layer_node,
                             const ir::Map& map,
                             ir::Layer& layer) -> Result<void>
{
  return parse_objects(layer_node, map).and_then([&](Vector<ir::Object>&& objects) {
    layer.objects = std::move(objects);
    return kOK;
  });
}

auto parse_group_layer_data(const YAML::Node& layer_node,
                            const ir::Map& map,
                            ir::Layer& layer) -> Result<void>
{
  if (const auto layer_seq = layer_node["layers"]) {
    layer.layers.reserve(layer_seq.size());

    for (const auto& sublayer_node : layer_seq) {
      if (auto sublayer = parse_layer(sublayer_node, map)) {
        layer.layers.push_back(std::move(*sublayer));
      }
      else {
        return propagate_unexpected(sublayer);
      }
    }
  }

  return kOK;
}

auto parse_layer(const YAML::Node& layer_node, const ir::Map& map) -> Result<ir::Layer>
{
  TACTILE_LOG_TRACE("Parsing layer node at {}:{}",
                    layer_node.Mark().line,
                    layer_node.Mark().column);

  ir::Layer layer {};

  return parse_to(layer_node, "id", layer.id)
      .and_then([&] {
        layer.name = try_parse<String>(layer_node, "name").value_or("Layer");
        return kOK;
      })
      .and_then([&] {
        layer.opacity = try_parse<float>(layer_node, "opacity").value_or(1.0f);
        return kOK;
      })
      .and_then([&] {
        layer.visible = try_parse<bool>(layer_node, "visible").value_or(true);
        return kOK;
      })
      .and_then([&] { return parse<String>(layer_node, "type"); })
      .and_then([&](const String& type_name) { return parse_layer_type(type_name); })
      .and_then([&](const LayerType type) {
        layer.type = type;

        switch (type) {
          case LayerType::kTileLayer:
            return parse_tile_layer_data(layer_node, map, layer);

          case LayerType::kObjectLayer:
            return parse_object_layer_data(layer_node, map, layer);

          case LayerType::kGroupLayer:
            return parse_group_layer_data(layer_node, map, layer);
        }

        return kOK;
      })
      .and_then([&] { return parse_metadata(layer_node, map, layer.meta); })
      .transform([&] { return std::move(layer); });
}

auto parse_layers(const YAML::Node& root_node, const ir::Map& map)
    -> Result<Vector<ir::Layer>>
{
  Vector<ir::Layer> layers {};

  if (const auto layer_seq = root_node["layers"]) {
    for (const auto& layer_node : layer_seq) {
      if (auto layer = parse_layer(layer_node, map)) {
        layers.push_back(std::move(*layer));
      }
      else {
        return propagate_unexpected(layer);
      }
    }
  }

  return layers;
}

}  // namespace tactile::yml_format
