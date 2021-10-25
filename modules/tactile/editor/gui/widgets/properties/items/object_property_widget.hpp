#pragma once

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

[[nodiscard]] auto ObjectPropertyWidget(const PropertyValue& property)
    -> Maybe<ObjectRef>;

}  // namespace Tactile
