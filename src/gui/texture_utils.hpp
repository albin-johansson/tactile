#pragma once

#include <imgui.h>

#include "aliases/ints.hpp"

namespace Tactile {

[[nodiscard]] auto ToTextureID(uint texture) -> ImTextureID;

}  // namespace Tactile
