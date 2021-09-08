#pragma once

#include <functional>  // less
#include <map>         // map
#include <string>      // string

#include "core/properties/property_value.hpp"

namespace Tactile::Sys {

struct PropertyContextSnapshot final
{
  std::string name;
  std::map<std::string, PropertyValue, std::less<>> properties;
};

}  // namespace Tactile::Sys
