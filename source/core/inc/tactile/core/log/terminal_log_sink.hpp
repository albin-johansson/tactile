// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/log/log_sink.hpp"

namespace tactile {

/**
 * A log sink that simply forwards messages to the terminal.
 */
class TerminalLogSink final : public ILogSink
{
 public:
  inline static constexpr StringView kAnsiColorReset = "\x1B[0m";
  inline static constexpr StringView kAnsiColorBold = "\x1B[1m";
  inline static constexpr StringView kAnsiColorFgBlack = "\x1B[30m";
  inline static constexpr StringView kAnsiColorFgBlackBold = "\x1B[30m\x1B[1m";
  inline static constexpr StringView kAnsiColorFgRed = "\x1B[31m";
  inline static constexpr StringView kAnsiColorFgRedBold = "\x1B[31m\x1B[1m";
  inline static constexpr StringView kAnsiColorFgGreen = "\x1B[32m";
  inline static constexpr StringView kAnsiColorFgYellow = "\x1B[33m";
  inline static constexpr StringView kAnsiColorFgBlue = "\x1B[34m";
  inline static constexpr StringView kAnsiColorFgMagenta = "\x1B[35m";
  inline static constexpr StringView kAnsiColorFgCyan = "\x1B[36m";
  inline static constexpr StringView kAnsiColorFgWhite = "\x1B[37m";

  void log(const LogMessage& msg) override;

  void flush() override;

  /**
   * Controls whether messages are color-coded using ANSI colors.
   *
   * \param enabled True if ANSI colors should be used; false otherwise.
   */
  void use_ansi_colors(bool enabled);

  /**
   * Returns the foreground ANSI color for a given log level.
   *
   * \param level The log level to query.
   */
  [[nodiscard]]
  static auto get_fg_ansi_color(LogLevel level) -> StringView;

 private:
  bool mUseAnsiColors {false};
};

}  // namespace tactile
