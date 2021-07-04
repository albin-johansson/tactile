#pragma once

#include <string>  // string

#include "core/properties/property_type.hpp"

namespace Tactile {

struct AddPropertyEvent final
{
  std::string name;
  PropertyType type;
};

}  // namespace Tactile
