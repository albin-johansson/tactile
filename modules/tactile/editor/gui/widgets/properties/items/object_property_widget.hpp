#pragma once

#include <tactile-base/tactile_std.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ObjectPropertyWidget(const PropertyValue& property)
    -> Maybe<ObjectRef>;

}  // namespace Tactile
