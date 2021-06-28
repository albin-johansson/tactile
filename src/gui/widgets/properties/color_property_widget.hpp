#pragma once

#include <centurion.hpp>  // color

#include "aliases/maybe.hpp"
#include "core/properties/property.hpp"

namespace tactile {

[[nodiscard]] auto ColorPropertyWidget(const Property& property)
    -> Maybe<cen::color>;

}  // namespace tactile
