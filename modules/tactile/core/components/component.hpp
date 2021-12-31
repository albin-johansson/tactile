#pragma once

#include <functional>  // less
#include <map>         // map
#include <string>      // string
#include <vector>      // vector

#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/property_value.hpp"

namespace Tactile {

/// Identifier for component definition types (not persistent).
using ComponentID = int32;

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
struct ComponentDef final {
  ComponentID id{};  ///< Unique ID for the component definition.
  std::string name;  ///< The human-readable component name, also unique.

  std::map<std::string, PropertyType, std::less<>> attributes;  ///< Attribute types.
  std::map<std::string, PropertyValue, std::less<>> defaults;   ///< Default values.
};

/**
 * Represents a user-defined component that can be attached to property context entities.
 */
struct Component final {
  ComponentID type;  ///< The ID of the component definition type.
  std::map<std::string, PropertyValue, std::less<>> values;  ///< Current values.
};

/**
 * This component tracks all of the components attached to an entity.
 */
struct ComponentBundle final {
  std::vector<entt::entity> components;
};

}  // namespace Tactile