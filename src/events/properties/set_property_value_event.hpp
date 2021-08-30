#pragma once

#include <string_view>  // string_view

#include "core/properties/property_value.hpp"

namespace Tactile {

/// \addtogroup events
/// \{

struct SetPropertyValueEvent final
{
  std::string_view name;
  PropertyValue property;
};

/// \} End of group events

}  // namespace Tactile
