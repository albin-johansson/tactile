// Copyright (C) 2025 Albin Johansson

export module tactile.editor.cli;

import tactile.core;

export namespace tactile::editor {

/// Represents command line options.
struct CliOptions final
{
  /// The minimum log level to use.
  LogLevel log_level;

  /// Indicates whether the editor should be launched.
  bool should_launch;
};

/// Returns the default CLI options.
auto make_default_cli_options() -> CliOptions;

/// Parses command line options.
auto parse_cli_options(int argc, char* argv[]) -> Result<CliOptions>;

}  // namespace tactile::editor
