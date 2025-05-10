// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <cstdlib>

module tactile_editor.app;

import tactile_editor.cli;

namespace tactile::editor {

auto launch(const int argc, char* argv[]) -> int
{
  const auto cli_options = parse_cli_options(argc, argv);
  if (!cli_options.has_value()) {
    return EXIT_FAILURE;
  }

  if (!cli_options->should_launch) {
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}

}  // namespace tactile::editor
