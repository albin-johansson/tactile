// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class IRenderer;

/**
 * Attempts to reload the font texture atlas.
 *
 * \note
 * This function does nothing if the renderer doesn't support font reloading.
 *
 * \param renderer          The active renderer.
 * \param font_size         The font size to use.
 * \param framebuffer_scale The display framebuffer scale.
 */
void reload_fonts(IRenderer& renderer,
                  float font_size,
                  float framebuffer_scale);

}  // namespace tactile
