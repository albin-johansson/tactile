#pragma once

#include "aliases/maybe.hpp"
#include "aliases/object_ref.hpp"
#include "core/properties/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ObjectPropertyWidget(const PropertyValue& property)
    -> Maybe<ObjectRef>;

}  // namespace Tactile
