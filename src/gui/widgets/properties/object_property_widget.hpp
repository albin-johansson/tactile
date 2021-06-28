#pragma once

#include "aliases/maybe.hpp"
#include "aliases/object_ref.hpp"
#include "core/properties/property.hpp"

namespace tactile {

[[nodiscard]] auto ObjectPropertyWidget(const Property& property)
    -> Maybe<object_ref>;

}  // namespace tactile
