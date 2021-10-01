#pragma once

#include <centurion.hpp>  // color

#include "common/maybe.hpp"
#include "core/properties/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ColorPropertyWidget(const PropertyValue& property)
    -> Maybe<cen::color>;

}  // namespace Tactile
