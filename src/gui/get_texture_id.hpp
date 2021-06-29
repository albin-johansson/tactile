#pragma once

#include "aliases/ints.hpp"
#include "imgui.h"

namespace Tactile {

[[nodiscard]] auto GetTextureID(uint texture) -> ImTextureID;

}  // namespace Tactile
