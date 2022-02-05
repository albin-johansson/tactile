#pragma once

#include <imgui.h>

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Converts an OpenGL texture identifier to a Dear ImGui texture identifier.
 *
 * \ingroup gui
 *
 * \param texture the source OpenGL texture identifier.
 *
 * \return a Dear ImGui texture identifier.
 */
[[nodiscard]] auto to_texture_id(uint texture) noexcept -> ImTextureID;

}  // namespace tactile
