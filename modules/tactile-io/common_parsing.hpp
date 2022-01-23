#pragma once

#include <optional>     // optional
#include <string_view>  // string_view

#include "ir_common.hpp"

namespace tactile::IO {

[[nodiscard]] auto ParseColorRGB(std::string_view rgb) -> std::optional<Color>;

[[nodiscard]] auto ParseColorRGBA(std::string_view rgba) -> std::optional<Color>;

[[nodiscard]] auto ParseColorARGB(std::string_view argb) -> std::optional<Color>;

[[nodiscard]] auto ParseAttributeType(std::string_view type)
    -> std::optional<PropertyType>;

}  // namespace tactile::IO