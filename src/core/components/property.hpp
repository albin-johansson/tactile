#pragma once

#include <string>  // string

#include "core/properties/property_value.hpp"

namespace Tactile {

struct Property final
{
  std::string name;
  PropertyValue value;
};

}  // namespace Tactile
