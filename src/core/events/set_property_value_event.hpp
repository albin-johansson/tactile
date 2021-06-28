#pragma once

#include <string_view>  // string_view

#include "core/properties/property.hpp"

namespace tactile {

struct SetPropertyValueEvent final
{
  std::string_view name;
  Property property;
};

}  // namespace tactile
