#pragma once

#include <functional>  // less
#include <map>         // map
#include <string>      // string
#include <vector>      // vector

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/attribute_value.hpp"

namespace tactile {

using component_attribute_map = std::map<std::string, attribute_value, std::less<>>;

namespace comp {

/**
 * Describes the structure of a component.
 *
 * Component definitions are unique to each map (for now).
 *
 * \todo A possible use case for projects could be to provide common component
 * definitions.
 *
 * \todo Should it be possible to import component definitions from existing maps?
 */
struct component_def final
{
  ComponentID id{};  ///< Unique ID for the component definition.
  std::string name;  ///< The human-readable component name, also unique.
  component_attribute_map attributes;
};

/**
 * Represents a user-defined component that can be attached to attribute context entities.
 */
struct component final
{
  ComponentID type;                ///< The ID of the component definition type.
  component_attribute_map values;  ///< Current values.
};

}  // namespace comp
}  // namespace tactile