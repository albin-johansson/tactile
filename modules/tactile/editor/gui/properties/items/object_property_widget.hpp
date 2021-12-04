#pragma once

#include <tactile_def.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ObjectPropertyWidget(const PropertyValue& property)
    -> Maybe<ObjectRef>;

}  // namespace Tactile
