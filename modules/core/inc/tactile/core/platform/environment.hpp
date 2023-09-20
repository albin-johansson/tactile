// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/** Returns the value of an environment variable. */
[[nodiscard]]
TACTILE_CORE_API auto get_env(const char* var) -> Maybe<String>;

/**
 * \brief Enables virtual terminal sequences.
 *
 * \details This function can be used to enable the use of special ANSI color codes in
 *          terminal output on Windows.
 *
 * \note This is a Windows specific API, and has no effect on other platforms.
 *
 * \see https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#example-of-enabling-virtual-terminal-processing
 */
TACTILE_CORE_API
void win32_enable_virtual_terminal_processing();

}  // namespace tactile
