#pragma once

#include <centurion.hpp>  // color

#include "common/maybe.hpp"
#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ColorPropertyWidget(const PropertyValue& property)
    -> Maybe<cen::color>;

}  // namespace Tactile
