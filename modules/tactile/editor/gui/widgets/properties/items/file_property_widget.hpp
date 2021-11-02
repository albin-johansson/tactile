#pragma once

#include <filesystem>  // path

#include <tactile-base/tactile_std.hpp>

#include "core/property_value.hpp"

namespace Tactile {

[[nodiscard]] auto FilePropertyWidget(const PropertyValue& property)
    -> Maybe<std::filesystem::path>;

}  // namespace Tactile
