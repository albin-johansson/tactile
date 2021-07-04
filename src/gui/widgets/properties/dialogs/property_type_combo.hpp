#pragma once

#include "core/properties/property_type.hpp"

namespace Tactile {

auto PropertyTypeCombo(int* index) -> bool;

[[nodiscard]] auto GetPropertyTypeFromComboIndex(int index) -> PropertyType;

}  // namespace Tactile
