// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

struct SDL_Window;

namespace tactile {

/**
 * Makes the window title bar dark.
 *
 * \note
 *    This is a Windows-specific API, and has no effect on other platforms.
 *
 * \param window The target window.
 */
void win32_use_immersive_dark_mode(SDL_Window* window);

}  // namespace tactile
