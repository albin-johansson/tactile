#pragma once

#include "aliases/maybe.hpp"
#include "core/properties/property.hpp"

namespace Tactile {

[[nodiscard]] auto FloatPropertyWidget(const Property& property) -> Maybe<float>;

}  // namespace Tactile
