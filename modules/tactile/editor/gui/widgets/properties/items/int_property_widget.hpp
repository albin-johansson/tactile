#pragma once

#include <tactile-base/tactile_std.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto IntPropertyWidget(const PropertyValue& property) -> Maybe<int>;

}  // namespace Tactile
