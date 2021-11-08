#pragma once

#include <functional>  // less
#include <map>         // map
#include <string>      // string

#include <tactile_def.hpp>

#include "core/property_value.hpp"

namespace Tactile::Sys {

struct PropertyContextSnapshot final
{
  ContextID id;
  std::string name;
  std::map<std::string, PropertyValue, std::less<>> properties;
};

}  // namespace Tactile::Sys
