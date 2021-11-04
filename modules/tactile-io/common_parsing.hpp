#pragma once

#include <optional>     // optional
#include <string_view>  // string_view

#include "ir_common.hpp"

namespace Tactile::IO {

[[nodiscard]] auto ParseColorRGB(std::string_view rgb) -> std::optional<Color>;

[[nodiscard]] auto ParseColorRGBA(std::string_view rgba) -> std::optional<Color>;

[[nodiscard]] auto ParseColorARGB(std::string_view argb) -> std::optional<Color>;

}  // namespace Tactile::IO