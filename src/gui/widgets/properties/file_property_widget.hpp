#pragma once

#include <filesystem>  // path

#include "aliases/maybe.hpp"
#include "core/properties/property.hpp"

namespace Tactile {

[[nodiscard]] auto FilePropertyWidget(const Property& property)
    -> Maybe<std::filesystem::path>;

}  // namespace Tactile
