#pragma once

#include <imgui.h>

#include "aliases/ints.hpp"

namespace Tactile {

/**
 * \brief Converts an OpenGL texture identifier to a Dear ImGui texture identifier.
 *
 * \ingroup gui
 *
 * \param texture the OpenGL texture identifier that will be converted.
 *
 * \return a corresponding Dear ImGui texture identifier.
 */
[[nodiscard]] auto ToTextureID(uint texture) -> ImTextureID;

}  // namespace Tactile
