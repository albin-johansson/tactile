#pragma once

#include <string>  // string

#include "core/properties/property_type.hpp"

namespace Tactile {

struct ChangePropertyTypeEvent final
{
  std::string name;
  PropertyType type;
};

}  // namespace Tactile
