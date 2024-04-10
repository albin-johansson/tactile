// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/log/terminal_log_sink.hpp"

#include <iostream>  // cout

namespace tactile {

void TerminalLogSink::log(const LogMessage& msg)
{
  if (mUseAnsiColors) {
    const auto fg_color = get_fg_ansi_color(msg.level);
    std::cout << fg_color;
  }

  std::cout << msg.prefix;

  if (!msg.scope.empty()) {
    std::cout << '[' << msg.scope << ']';
  }

  std::cout << ' ' << msg.text;

  if (mUseAnsiColors) {
    std::cout << kAnsiColorReset;
  }

  std::cout << '\n';
}

void TerminalLogSink::flush()
{
  std::cout.flush();
}

void TerminalLogSink::use_ansi_colors(const bool enabled)
{
  mUseAnsiColors = enabled;
}

auto TerminalLogSink::get_fg_ansi_color(const LogLevel level) -> StringView
{
  switch (level) {
    case LogLevel::kTrace: return kAnsiColorFgMagenta;
    case LogLevel::kDebug: return kAnsiColorFgCyan;
    case LogLevel::kInfo: return kAnsiColorFgWhite;
    case LogLevel::kWarn: return kAnsiColorFgYellow;
    case LogLevel::kError: [[fallthrough]];
    case LogLevel::kFatal: return kAnsiColorFgRedBold;
  }

  return kAnsiColorFgWhite;
}

}  // namespace tactile
