#pragma once

#include "aliases/maybe.hpp"
#include "core/properties/property.hpp"

namespace tactile {

[[nodiscard]] auto BoolPropertyWidget(const Property& property) -> Maybe<bool>;

}  // namespace tactile
