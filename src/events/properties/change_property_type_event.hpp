#pragma once

#include <string>  // string

#include "core/properties/property_type.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct ChangePropertyTypeEvent final
{
  std::string name;
  PropertyType type;
};

/// \} End of group events

}  // namespace Tactile
