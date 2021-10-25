#pragma once

#include <centurion.hpp>  // color

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

[[nodiscard]] auto ColorPropertyWidget(const PropertyValue& property)
    -> Maybe<cen::color>;

}  // namespace Tactile
