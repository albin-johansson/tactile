// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/log/log_level.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/foundation/render/renderer_id.hpp"

namespace tactile::core {

/**
 * Represents the command-line options.
 */
struct CommandLineOptions final {
  LogLevel log_level;       ///< The logger verbosity level.
  RendererID renderer_id;   ///< The requested renderer backend.
  bool reset_settings : 1;  ///< True if the persistent settings should be reset.
  bool should_exit : 1;     ///< True if the editor shouldn't launch (e.g., for --help).
};

/**
 * Used to parse command-line options.
 */
class TACTILE_CORE_API CommandLineParser final {
 public:
  TACTILE_STATIC_CLASS(CommandLineParser);

  /**
   * Parses command-line arguments.
   *
   * \param argc The number of arguments.
   * \param argv The command-line arguments from `main`.
   *
   * \return
   *    The parsed command-line options.
   */
  [[nodiscard]]
  static auto parse(int argc, char** argv) -> CommandLineOptions;
};

}  // namespace tactile::core
