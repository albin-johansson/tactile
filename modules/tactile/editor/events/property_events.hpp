#pragma once

#include <string>  // string

#include <entt/entt.hpp>

#include "core/attribute_value.hpp"
#include "tactile_def.hpp"

namespace tactile {

/// \addtogroup events
/// \{

struct ShowAddPropertyDialogEvent final
{};

struct ShowChangePropertyTypeDialogEvent final
{
  std::string name;
  PropertyType current_type;
};

struct ShowRenamePropertyDialogEvent final
{
  std::string name;
};

struct AddPropertyEvent final
{
  std::string name;   ///< The chosen name.
  PropertyType type;  ///< The chosen type.
};

struct RemovePropertyEvent final
{
  std::string name;  ///< Name of the property in the current context.
};

struct RenamePropertyEvent final
{
  std::string old_name;  ///< Current property name.
  std::string new_name;  ///< Requested new property name.
};

struct UpdatePropertyEvent final
{
  std::string name;       ///< Name of property to modify.
  attribute_value value;  ///< Updated value of the property.
};

struct ChangePropertyTypeEvent final
{
  std::string name;   ///< Name of property to modify.
  PropertyType type;  ///< Requested new property type.
};

struct InspectContextEvent final
{
  entt::entity entity{
      entt::null};  ///< Entity that features `attribute_context` component.
};

/// \} End of group events

}  // namespace tactile
