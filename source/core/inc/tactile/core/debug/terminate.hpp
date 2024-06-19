// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Provides a custom terminate handler.
 *
 * \details
 * This function will log an error message including the current stack trace,
 * and subsequently abort the program.
 */
[[noreturn]]
void on_terminate() noexcept;

}  // namespace tactile
