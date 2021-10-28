#pragma once

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

#include <centurion.hpp>  // color

namespace Tactile {

[[nodiscard]] auto ColorPropertyWidget(const PropertyValue& property)
    -> Maybe<cen::color>;

}  // namespace Tactile
