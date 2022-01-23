#pragma once

#include <string>  // string

#include "core/attribute_value.hpp"

namespace Tactile {

struct Property final
{
  std::string name;
  attribute_value value;
};

}  // namespace Tactile
