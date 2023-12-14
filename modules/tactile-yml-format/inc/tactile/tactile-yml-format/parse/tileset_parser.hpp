// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_tileset(const YAML::Node& tileset_node,
                                          const ir::Map& map,
                                          const SaveFormatReadOptions& options)
    -> Result<ir::Tileset>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_tileset_ref(const YAML::Node& tileset_ref_node,
                                              const ir::Map& map,
                                              const SaveFormatReadOptions& options)
    -> Result<ir::TilesetRef>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_tileset_refs(const YAML::Node& root_node,
                                               const ir::Map& map,
                                               const SaveFormatReadOptions& options)
    -> Result<Vector<ir::TilesetRef>>;

}  // namespace tactile::yml_format
