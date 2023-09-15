// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/log/terminal_logger_sink.hpp"

#include <iostream>  // clog

namespace tactile {

void TerminalLoggerSink::log(const LogMessage& msg)
{
  if (mUseAnsiColors) {
    const auto bg_color = get_bg_ansi_color(msg.level);
    const auto fg_color = get_fg_ansi_color(msg.level);

    std::clog << bg_color << fg_color;
  }

  std::clog << msg.prefix << ' ' << msg.text;

  if (mUseAnsiColors) {
    std::clog << kAnsiColorReset;
  }

  std::clog << '\n';
}

void TerminalLoggerSink::flush()
{
  std::clog.flush();
}

void TerminalLoggerSink::use_ansi_colors(const bool enabled)
{
  mUseAnsiColors = enabled;
}

auto TerminalLoggerSink::get_fg_ansi_color(const LogLevel level) -> StringView
{
  switch (level) {
    case LogLevel::kTrace:
      return kAnsiColorFgGreen;

    case LogLevel::kDebug:
      return kAnsiColorFgCyan;

    case LogLevel::kInfo:
      return kAnsiColorFgWhite;

    case LogLevel::kWarn:
      return kAnsiColorFgYellow;

    case LogLevel::kError:
      return kAnsiColorFgRedBold;

    case LogLevel::kFatal:
      return kAnsiColorFgBlackBold;
  }

  return kAnsiColorFgWhite;
}

auto TerminalLoggerSink::get_bg_ansi_color(const LogLevel level) -> StringView
{
  switch (level) {
    case LogLevel::kTrace:
    case LogLevel::kDebug:
    case LogLevel::kInfo:
    case LogLevel::kWarn:
    case LogLevel::kError:
      return "";

    case LogLevel::kFatal:
      return kAnsiColorBgRed;
  }

  return "";
}

}  // namespace tactile
