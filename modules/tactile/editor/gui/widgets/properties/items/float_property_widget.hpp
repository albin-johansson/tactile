#pragma once

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

[[nodiscard]] auto FloatPropertyWidget(const PropertyValue& property) -> Maybe<float>;

}  // namespace Tactile
