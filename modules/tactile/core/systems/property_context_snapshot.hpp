#pragma once

#include <functional>  // less
#include <map>         // map
#include <string>      // string

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile::Sys {

struct PropertyContextSnapshot final
{
  ContextID id;
  std::string name;
  std::map<std::string, PropertyValue, std::less<>> properties;
};

}  // namespace Tactile::Sys
