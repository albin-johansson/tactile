// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class IRenderer;

/**
 * \brief Attempts to reload the Dear ImGui font texture atlas.
 *
 * \note This function does nothing if the renderer doesn't support font reloading.
 *
 * \param renderer          the active renderer.
 * \param font_size         the current font size.
 * \param framebuffer_scale the current display framebuffer scale.
 */
TACTILE_CORE_API void try_reload_imgui_fonts(IRenderer& renderer,
                                             float font_size,
                                             float framebuffer_scale);

}  // namespace tactile
