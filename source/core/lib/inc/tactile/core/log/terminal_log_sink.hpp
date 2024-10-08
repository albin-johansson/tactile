// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <string_view>  // string_view

#include "tactile/base/prelude.hpp"
#include "tactile/core/log/log_sink.hpp"

namespace tactile {

/**
 * A log sink that simply forwards messages to the terminal.
 */
class TerminalLogSink final : public ILogSink
{
 public:
  constexpr static std::string_view kAnsiColorReset = "\x1B[0m";
  constexpr static std::string_view kAnsiColorBold = "\x1B[1m";
  constexpr static std::string_view kAnsiColorFgBlack = "\x1B[30m";
  constexpr static std::string_view kAnsiColorFgBlackBold = "\x1B[30m\x1B[1m";
  constexpr static std::string_view kAnsiColorFgRed = "\x1B[31m";
  constexpr static std::string_view kAnsiColorFgRedBold = "\x1B[31m\x1B[1m";
  constexpr static std::string_view kAnsiColorFgGreen = "\x1B[32m";
  constexpr static std::string_view kAnsiColorFgYellow = "\x1B[33m";
  constexpr static std::string_view kAnsiColorFgBlue = "\x1B[34m";
  constexpr static std::string_view kAnsiColorFgMagenta = "\x1B[35m";
  constexpr static std::string_view kAnsiColorFgCyan = "\x1B[36m";
  constexpr static std::string_view kAnsiColorFgWhite = "\x1B[37m";

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
  static auto get_fg_ansi_color(LogLevel level) -> std::string_view;

 private:
  bool mUseAnsiColors {false};
};

}  // namespace tactile
