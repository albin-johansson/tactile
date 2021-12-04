#pragma once

#include <tactile_def.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto BoolPropertyWidget(const PropertyValue& property) -> Maybe<bool>;

}  // namespace Tactile
