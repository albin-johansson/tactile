// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/io/save/save_format_options.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_map(String map_name,
                                      const YAML::Node& root_node,
                                      const SaveFormatReadOptions& options)
    -> Result<ir::Map>;

}  // namespace tactile::yml_format
