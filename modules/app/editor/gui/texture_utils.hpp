#pragma once

#include <imgui.h>

#include "tactile_def.hpp"

namespace tactile {

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

}  // namespace tactile
