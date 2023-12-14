// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_layer_type(StringView type_name) -> Result<LayerType>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_plain_text_tile_matrix(String tile_data,
                                                         MatrixExtent matrix_extent)
    -> Result<TileMatrix>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_tile_matrix(const ir::TileFormat& tile_format,
                                              String&& tile_data,
                                              ir::Layer& layer) -> Result<TileMatrix>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_tile_layer_data(const YAML::Node& layer_node,
                                                  const ir::Map& map,
                                                  ir::Layer& layer) -> Result<void>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_object_layer_data(const YAML::Node& layer_node,
                                                    const ir::Map& map,
                                                    ir::Layer& layer) -> Result<void>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_group_layer_data(const YAML::Node& layer_node,
                                                   const ir::Map& map,
                                                   ir::Layer& layer) -> Result<void>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_layer(const YAML::Node& layer_node, const ir::Map& map)
    -> Result<ir::Layer>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_layers(const YAML::Node& root_node, const ir::Map& map)
    -> Result<Vector<ir::Layer>>;

}  // namespace tactile::yml_format
