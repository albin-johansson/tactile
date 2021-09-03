#pragma once

#include "aliases/maybe.hpp"
#include "core/properties/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto FloatPropertyWidget(const PropertyValue& property)
    -> Maybe<float>;

}  // namespace Tactile
