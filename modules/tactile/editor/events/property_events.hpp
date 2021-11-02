#pragma once

#include <string>  // string

#include <tactile-base/property_type.hpp>

#include <entt.hpp>  // entity, null

#include "core/property_value.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct AddPropertyEvent final
{
  std::string name;
  PropertyType type;
};

struct RemovePropertyEvent final
{
  std::string name;
};

struct RenamePropertyEvent final
{
  std::string old_name;
  std::string new_name;
};

struct UpdatePropertyEvent final
{
  std::string name;
  PropertyValue value;
};

struct ChangePropertyTypeEvent final
{
  std::string name;
  PropertyType type;
};

struct SetPropertyContextEvent final
{
  entt::entity entity{entt::null};
};

/// \} End of group events

}  // namespace Tactile
