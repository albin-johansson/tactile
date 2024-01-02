// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/api.hpp"

namespace tactile {

/**
 * Logs the current call stack and aborts the program.
 *
 * \details
 *    This function is intended to be used an alternative terminate handler, which
 *    can be configured using `std::set_terminate`.
 */
[[noreturn]]
TACTILE_FOUNDATION_API void on_terminate();

}  // namespace tactile
