#pragma once

#include <filesystem>  // path

#include "common/maybe.hpp"
#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto FilePropertyWidget(const PropertyValue& property)
    -> Maybe<std::filesystem::path>;

}  // namespace Tactile
