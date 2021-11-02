#pragma once

#include <tactile-base/tactile_std.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto BoolPropertyWidget(const PropertyValue& property) -> Maybe<bool>;

}  // namespace Tactile
