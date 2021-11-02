#pragma once

#include <tactile-base/tactile_std.hpp>

#include <centurion.hpp>  // color

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ColorPropertyWidget(const PropertyValue& property)
    -> Maybe<cen::color>;

}  // namespace Tactile
