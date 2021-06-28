#pragma once

#include "aliases/ints.hpp"
#include "imgui.h"

namespace tactile {

[[nodiscard]] auto GetTextureID(uint texture) -> ImTextureID;

}  // namespace tactile
