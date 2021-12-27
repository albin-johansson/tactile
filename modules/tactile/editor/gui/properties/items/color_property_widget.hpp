#pragma once

#include <tactile_def.hpp>

#include <centurion.hpp>  // color

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ColorPropertyWidget(const PropertyValue& property)
    -> Maybe<cen::Color>;

}  // namespace Tactile
