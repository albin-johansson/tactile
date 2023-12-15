// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_attached_component_attribute(
    const YAML::Node& attached_component_node,
    const ir::Component& prototype) -> Result<ir::NamedAttribute>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_attached_component_attributes(
    const YAML::Node& attached_component_node,
    const ir::Component& prototype) -> Result<Vector<ir::NamedAttribute>>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_attached_component(
    const YAML::Node& attached_component_node,
    const ir::Map& map) -> Result<ir::AttachedComponent>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_attached_components(const YAML::Node& context_node,
                                                      const ir::Map& map)
    -> Result<Vector<ir::AttachedComponent>>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_component_attribute(const YAML::Node& attribute_node)
    -> Result<ir::NamedAttribute>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_component_attributes(const YAML::Node& component_node)
    -> Result<Vector<ir::NamedAttribute>>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_component(const YAML::Node& component_node)
    -> Result<ir::Component>;

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_components(const YAML::Node& root_node)
    -> Result<Vector<ir::Component>>;

}  // namespace tactile::yml_format
