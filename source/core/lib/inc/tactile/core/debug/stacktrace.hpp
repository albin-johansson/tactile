// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string>  // string

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Captures and returns the current call stack.
 *
 * \return
 * The call stack encoded in a string.
 */
[[nodiscard]]
TACTILE_NOINLINE auto get_stacktrace() -> std::string;

}  // namespace tactile
