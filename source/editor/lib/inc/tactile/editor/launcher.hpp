// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#pragma once

namespace tactile::editor {

/// Launches the Tactile editor with the given command line arguments.
[[nodiscard]]
auto launch(int argc, char* argv[]) -> int;

}  // namespace tactile::editor
