// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/debug/log/logger_sink.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

inline constexpr StringView kAnsiColorReset = "\x1B[0m";
inline constexpr StringView kAnsiColorBold = "\x1B[1m";
inline constexpr StringView kAnsiColorFgBlack = "\x1B[30m";
inline constexpr StringView kAnsiColorFgBlackBold = "\x1B[30m\x1B[1m";
inline constexpr StringView kAnsiColorFgRed = "\x1B[31m";
inline constexpr StringView kAnsiColorFgRedBold = "\x1B[31m\x1B[1m";
inline constexpr StringView kAnsiColorFgGreen = "\x1B[32m";
inline constexpr StringView kAnsiColorFgYellow = "\x1B[33m";
inline constexpr StringView kAnsiColorFgBlue = "\x1B[34m";
inline constexpr StringView kAnsiColorFgMagenta = "\x1B[35m";
inline constexpr StringView kAnsiColorFgCyan = "\x1B[36m";
inline constexpr StringView kAnsiColorFgWhite = "\x1B[37m";
inline constexpr StringView kAnsiColorBgRed = "\x1B[41m";

/**
 * \brief A logger sink that outputs log messages to the terminal.
 */
class TerminalLoggerSink final : public ILoggerSink {
 public:
  TACTILE_CORE_API void log(const LogMessage& msg) override;

  TACTILE_CORE_API void flush() override;

  /**
   * \brief Controls whether messages are color-coded using ANSI colors.
   *
   * \param enabled true if ANSI colors should be used; false otherwise.
   */
  TACTILE_CORE_API void use_ansi_colors(bool enabled);

  /**
   * \brief Returns the foreground ANSI color for a given log level.
   *
   * \param level the log level to query.
   */
  [[nodiscard]]
  TACTILE_CORE_API static auto get_fg_ansi_color(LogLevel level) -> StringView;

  /**
   * \brief Returns the background ANSI color for a given log level.
   *
   * \param level the log level to query.
   */
  [[nodiscard]]
  TACTILE_CORE_API static auto get_bg_ansi_color(LogLevel level) -> StringView;

 private:
  bool mUseAnsiColors {};
};

}  // namespace tactile
