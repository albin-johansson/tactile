#pragma once

#include "common/maybe.hpp"
#include "common/object_ref.hpp"
#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto ObjectPropertyWidget(const PropertyValue& property)
    -> Maybe<ObjectRef>;

}  // namespace Tactile
