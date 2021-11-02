#pragma once

#include <tactile-base/tactile_std.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto FloatPropertyWidget(const PropertyValue& property) -> Maybe<float>;

}  // namespace Tactile
