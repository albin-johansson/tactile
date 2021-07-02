#pragma once

#include <imgui.h>

#include "aliases/ints.hpp"

namespace Tactile {

[[nodiscard]] auto GetTextureID(uint texture) -> ImTextureID;

}  // namespace Tactile
