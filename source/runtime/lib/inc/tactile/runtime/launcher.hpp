// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/runtime/api.hpp"

namespace tactile {

/**
 * Launches the Tactile editor application.
 *
 * \param argc The number of command-line arguments.
 * \param argv The command-line arguments.
 *
 * \return
 * \c EXIT_SUCCESS if successful; \c EXIT_FAILURE otherwise.
 */
TACTILE_RUNTIME_API auto launch(int argc, char* argv[]) -> int;

}  // namespace tactile
