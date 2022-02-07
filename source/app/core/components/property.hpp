#pragma once

#include <string>  // string

#include "core/attribute.hpp"

namespace tactile::comp {

struct property final
{
  std::string name;
  attribute_value value;
};

}  // namespace tactile::comp
