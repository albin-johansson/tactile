#pragma once

#include <string>  // string

#include "core/properties/property.hpp"

namespace Tactile {

struct PropertyComp final
{
  std::string name;
  Property value;
};

}  // namespace Tactile
