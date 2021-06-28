#pragma once

#include "aliases/maybe.hpp"
#include "core/properties/property.hpp"

namespace tactile {

[[nodiscard]] auto IntPropertyWidget(const Property& property) -> Maybe<int>;

}  // namespace tactile
