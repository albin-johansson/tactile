// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_metadata(const YAML::Node& context_node,
                                           const ir::Map& map,
                                           ir::Metadata& metadata) -> Result<void>;

}  // namespace tactile::yml_format
