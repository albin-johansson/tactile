#pragma once

#include "aliases/maybe.hpp"
#include "core/properties/property.hpp"

namespace Tactile {

[[nodiscard]] auto BoolPropertyWidget(const Property& property) -> Maybe<bool>;

}  // namespace Tactile
