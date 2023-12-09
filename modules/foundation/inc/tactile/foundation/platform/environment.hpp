// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Reads the value of an environment variable.
 *
 * \param var the environment variable name, may be null.
 *
 * \return the variable value; or nothing if variable wasn't found.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto get_env(const char* var) -> Maybe<String>;

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
TACTILE_FOUNDATION_API void win32_enable_virtual_terminal_processing();

}  // namespace tactile
