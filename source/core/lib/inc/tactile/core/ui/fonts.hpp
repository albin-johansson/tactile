// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>  // uint8_t

#include "tactile/base/prelude.hpp"
#include "tactile/base/render/renderer.hpp"

namespace tactile::core {

struct Settings;

namespace ui {

/**
 * Provides identifiers for the supported editor fonts.
 */
enum class FontID : std::uint8_t
{
  /** The default embedded Dear ImGui font. */
  kDefault,
  kRoboto
};

/**
 * Attempts to reload the font texture atlas.
 *
 * \note
 * This function does nothing if the renderer doesn't support font reloading.
 *
 * \param renderer          The active renderer.
 * \param settings          The current settings.
 * \param framebuffer_scale The display framebuffer scale.
 */
void reload_fonts(IRenderer& renderer, const Settings& settings, float framebuffer_scale);

}  // namespace ui
}  // namespace tactile::core
