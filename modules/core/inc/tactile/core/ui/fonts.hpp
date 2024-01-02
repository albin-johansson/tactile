// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class IRenderer;

/**
 * Attempts to reload the Dear ImGui font texture atlas.
 *
 * \note
 *    This function does nothing if the renderer doesn't support font reloading.
 *
 * \param renderer          The active renderer.
 * \param font_size         The current font size.
 * \param framebuffer_scale The current display framebuffer scale.
 */
TACTILE_CORE_API void try_reload_imgui_fonts(IRenderer& renderer,
                                             int font_size,
                                             float framebuffer_scale);

}  // namespace tactile
