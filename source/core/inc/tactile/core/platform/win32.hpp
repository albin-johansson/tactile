// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

struct SDL_Window;

namespace tactile {

/**
 * Makes the window title bar dark.
 *
 * \note
 * This is a Windows-specific API, it has no effect on other platforms.
 *
 * \param window The target window.
 */
void win32_use_immersive_dark_mode(SDL_Window* window);

/**
 * Enables virtual terminal sequences.
 *
 * \details
 * This function can be used to enable the use of special ANSI color codes in
 * terminal output on Windows.
 *
 * \note
 * This is a Windows-specific API, it has no effect on other platforms.
 *
 * \see
 * https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
 */
void win32_enable_virtual_terminal_processing();

}  // namespace tactile
