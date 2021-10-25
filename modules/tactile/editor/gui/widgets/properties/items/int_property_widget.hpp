#pragma once

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

[[nodiscard]] auto IntPropertyWidget(const PropertyValue& property) -> Maybe<int>;

}  // namespace Tactile
