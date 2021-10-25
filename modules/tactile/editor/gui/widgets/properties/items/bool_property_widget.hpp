#pragma once

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

[[nodiscard]] auto BoolPropertyWidget(const PropertyValue& property) -> Maybe<bool>;

}  // namespace Tactile
