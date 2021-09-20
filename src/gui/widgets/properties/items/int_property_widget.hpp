#pragma once

#include "aliases/maybe.hpp"
#include "core/properties/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto IntPropertyWidget(const PropertyValue& property) -> Maybe<int>;

}  // namespace Tactile
