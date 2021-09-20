#pragma once

#include "aliases/maybe.hpp"
#include "core/properties/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto BoolPropertyWidget(const PropertyValue& property) -> Maybe<bool>;

}  // namespace Tactile
