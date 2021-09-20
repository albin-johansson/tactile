#pragma once

#include <string>  // string

#include "aliases/maybe.hpp"
#include "core/properties/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto StringPropertyWidget(const PropertyValue& property)
    -> Maybe<std::string>;

}  // namespace Tactile
