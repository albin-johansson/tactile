#pragma once

#include "core/properties/property_type.hpp"

namespace Tactile {

void PropertyTypeCombo(PropertyType previous, PropertyType& out);

[[deprecated]] auto PropertyTypeCombo(int* index) -> bool;

[[deprecated, nodiscard]] auto GetPropertyTypeFromComboIndex(int index) -> PropertyType;

}  // namespace Tactile
