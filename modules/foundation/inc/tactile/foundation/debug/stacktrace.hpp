// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Captures and returns the current call stack.
 *
 * \return
 *    The call stack encoded in a string.
 */
[[nodiscard]]
TACTILE_NOINLINE TACTILE_FOUNDATION_API auto get_stacktrace() -> String;

}  // namespace tactile
