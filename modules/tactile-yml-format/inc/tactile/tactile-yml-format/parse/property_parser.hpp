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
TACTILE_YML_FORMAT_API auto parse_property_type(StringView type_name)
    -> Result<AttributeType>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_property_value(const YAML::Node& node,
                                                 AttributeType attr_type)
    -> Result<Attribute>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_property(const YAML::Node& node)
    -> Result<ir::NamedAttribute>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_properties(const YAML::Node& node)
    -> Result<Vector<ir::NamedAttribute>>;

}  // namespace tactile::yml_format
