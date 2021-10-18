#pragma once

#include <string>  // string

#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile {

[[nodiscard]] auto StringPropertyWidget(const PropertyValue& property)
    -> Maybe<std::string>;

}  // namespace Tactile
