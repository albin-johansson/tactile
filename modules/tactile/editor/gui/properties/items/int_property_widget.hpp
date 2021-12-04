#pragma once

#include <tactile_def.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto IntPropertyWidget(const PropertyValue& property) -> Maybe<int>;

}  // namespace Tactile
