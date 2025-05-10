// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.log:level;

export import tactile.core;

export namespace tactile {

/// The supported log level categories.
///
/// The underlying value increases with severity.
enum class LogLevel : u8
{
  /// For verbose messages only useful for detailed debugging.
  kTrace,

  /// For messages that provide useful developer information.
  kDebug,

  /// For messages that provide general high-level information.
  kInfo,

  /// For messages that warn about potential issues.
  kWarn,

  /// For messages that report errors.
  kError,
};

}  // namespace tactile
