// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/tactile-yml-format/emit/layer_emitter.hpp"

#include <sstream>  // stringstream

#include "tactile/foundation/io/tile_matrix_encoding.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tactile-yml-format/emit/meta_emitter.hpp"
#include "tactile/tactile-yml-format/emit/object_emitter.hpp"

namespace tactile::yml_format {

void emit_plain_text_tile_layer_data(YAML::Emitter& emitter,
                                     const ir::Layer& layer,
                                     const SaveFormatWriteOptions& options)
{
  const auto should_fold = options.fold_tile_layer_data;

  emitter << YAML::Key << "data";

  std::stringstream stream;
  for (usize row = 0; row < layer.height; ++row) {
    for (usize col = 0; col < layer.width; ++col) {
      // TODO integration test that tests round trips with different options
      if ((should_fold && col != 0) || (!should_fold && (row != 0 || col != 0))) {
        stream << ' ';
      }

      stream << layer.tiles[row][col].value;
    }

    if (should_fold && row < (layer.height - 1)) {
      stream << '\n';
    }
  }

  emitter << (should_fold ? YAML::Literal : YAML::Value) << stream.str();
}

void emit_tile_layer_data(YAML::Emitter& emitter,
                          const ir::Map& map,
                          const ir::Layer& layer,
                          const SaveFormatWriteOptions& options)
{
  if (map.tile_format.encoding == TileEncoding::kPlainText) {
    emit_plain_text_tile_layer_data(emitter, layer, options);
  }
  else {
    if (const auto encoded_tiles =
            base64_encode_tile_matrix(layer.tiles, map.tile_format.compression)) {
      emitter << YAML::Key << "data";
      emitter << YAML::Value << *encoded_tiles;
    }
    else {
      // TODO return result as well
      TACTILE_LOG_ERROR("Could not emit tile layer data: {}",
                        encoded_tiles.error().message());
    }
  }
}

void emit_object_layer_data(YAML::Emitter& emitter, const ir::Layer& layer)
{
  if (layer.objects.empty()) {
    return;
  }

  emitter << YAML::Key << "objects" << YAML::BeginSeq;

  for (const auto& object : layer.objects) {
    emit_object(emitter, object);
  }

  emitter << YAML::EndSeq;
}

void emit_group_layer_data(YAML::Emitter& emitter,
                           const ir::Map& map,
                           const ir::Layer& layer,
                           const SaveFormatWriteOptions& options)
{
  emitter << YAML::Key << "layers" << YAML::BeginSeq;

  for (const auto& sublayer : layer.layers) {
    emit_layer(emitter, map, sublayer, options);
  }

  emitter << YAML::EndSeq;
}

void emit_layer(YAML::Emitter& emitter,
                const ir::Map& map,
                const ir::Layer& layer,
                const SaveFormatWriteOptions& options)
{
  emitter << YAML::BeginMap;

  emitter << YAML::Key << "name" << YAML::Value << layer.meta.name;
  emitter << YAML::Key << "id" << YAML::Value << layer.id;

  if (layer.opacity != 1.0f) {
    emitter << YAML::Key << "opacity" << YAML::Value << layer.opacity;
  }

  if (!layer.visible) {
    emitter << YAML::Key << "visible" << YAML::Value << layer.visible;
  }

  emitter << YAML::Key << "type";
  switch (layer.type) {
    case LayerType::kTileLayer: {
      emitter << YAML::Value << "tile-layer";
      emit_tile_layer_data(emitter, map, layer, options);
      break;
    }
    case LayerType::kObjectLayer: {
      emitter << YAML::Value << "object-layer";
      emit_object_layer_data(emitter, layer);
      break;
    }
    case LayerType::kGroupLayer: {
      emitter << YAML::Value << "group-layer";
      emit_group_layer_data(emitter, map, layer, options);
      break;
    }
  }

  emit_metadata(emitter, layer.meta);

  emitter << YAML::EndMap;
}

void emit_layer_seq(YAML::Emitter& emitter,
                    const ir::Map& map,
                    const SaveFormatWriteOptions& options)
{
  if (map.layers.empty()) {
    return;
  }

  emitter << YAML::Key << "layers" << YAML::BeginSeq;

  for (const auto& layer : map.layers) {
    emit_layer(emitter, map, layer, options);
  }

  emitter << YAML::EndSeq;
}

}  // namespace tactile::yml_format
