#pragma once

#include <string>  // string

#include "aliases/maybe.hpp"
#include "core/properties/property.hpp"

namespace Tactile {

[[nodiscard]] auto StringPropertyWidget(const Property& property)
    -> Maybe<std::string>;

}  // namespace Tactile
