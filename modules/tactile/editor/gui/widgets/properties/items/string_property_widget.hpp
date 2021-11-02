#pragma once

#include <string>  // string

#include <tactile-base/tactile_std.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto StringPropertyWidget(const PropertyValue& property)
    -> Maybe<std::string>;

}  // namespace Tactile
