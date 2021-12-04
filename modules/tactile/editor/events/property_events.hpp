#pragma once

#include <string>  // string

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // entity, null

#include "core/property_value.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct AddPropertyEvent final {
  std::string name;   ///< The chosen name.
  PropertyType type;  ///< The chosen type.
};

struct RemovePropertyEvent final {
  std::string name;  ///< Name of the property in the current context.
};

struct RenamePropertyEvent final {
  std::string old_name;  ///< Current property name.
  std::string new_name;  ///< Requested new property name.
};

struct UpdatePropertyEvent final {
  std::string name;     ///< Name of property to modify.
  PropertyValue value;  ///< Updated value of the property.
};

struct ChangePropertyTypeEvent final {
  std::string name;   ///< Name of property to modify.
  PropertyType type;  ///< Requested new property type.
};

struct SetPropertyContextEvent final {
  entt::entity entity{entt::null};  ///< Entity that features `PropertyContext` component.
};

/// \} End of group events

}  // namespace Tactile
