// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/misc/object_type.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_object_type(StringView name) -> Result<ObjectType>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_object(const YAML::Node& object_node,
                                         const ir::Map& map) -> Result<ir::Object>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_objects(const YAML::Node& parent_node,
                                          const ir::Map& map)
    -> Result<Vector<ir::Object>>;

}  // namespace tactile::yml_format
