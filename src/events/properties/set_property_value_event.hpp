#pragma once

#include <string_view>  // string_view

#include "core/properties/property.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SetPropertyValueEvent final
{
  std::string_view name;
  Property property;
};

/// \} End of group events

}  // namespace Tactile
