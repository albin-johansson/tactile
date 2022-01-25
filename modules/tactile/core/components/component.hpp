#pragma once

#include <functional>  // less
#include <map>         // map
#include <string>      // string
#include <vector>      // vector

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/attribute_value.hpp"

namespace tactile {

using ComponentAttributeMap = std::map<std::string, attribute_value, std::less<>>;

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
struct ComponentDef final
{
  ComponentID id{};  ///< Unique ID for the component definition.
  std::string name;  ///< The human-readable component name, also unique.
  ComponentAttributeMap attributes;
};

/**
 * Represents a user-defined component that can be attached to attribute context entities.
 */
struct Component final
{
  ComponentID type;              ///< The ID of the component definition type.
  ComponentAttributeMap values;  ///< Current values.
};

}  // namespace tactile