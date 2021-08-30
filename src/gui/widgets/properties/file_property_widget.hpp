#pragma once

#include <filesystem>  // path

#include "aliases/maybe.hpp"
#include "core/properties/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto FilePropertyWidget(const PropertyValue& property)
    -> Maybe<std::filesystem::path>;

}  // namespace Tactile
