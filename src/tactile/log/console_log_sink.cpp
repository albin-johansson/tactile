// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module tactile.log;

import std;

namespace tactile {
namespace {

constexpr StringView kAnsiColorReset = "\x1B[0m";
constexpr StringView kAnsiColorFgRedBold = "\x1B[31m\x1B[1m";
constexpr StringView kAnsiColorFgYellow = "\x1B[33m";
constexpr StringView kAnsiColorFgMagenta = "\x1B[35m";
constexpr StringView kAnsiColorFgCyan = "\x1B[36m";
constexpr StringView kAnsiColorFgWhite = "\x1B[37m";

[[nodiscard]]
auto _get_ansi_color(const LogLevel level) noexcept -> StringView
{
  switch (level) {
    case LogLevel::kTrace: return kAnsiColorFgMagenta;
    case LogLevel::kDebug: return kAnsiColorFgCyan;
    case LogLevel::kInfo:  return kAnsiColorFgWhite;
    case LogLevel::kWarn:  return kAnsiColorFgYellow;
    case LogLevel::kError: return kAnsiColorFgRedBold;
  }

  return kAnsiColorFgWhite;
}

}  // namespace

void ConsoleLogSink::log(const LogMessage& msg)
{
  if (m_use_colors) {
    std::clog << _get_ansi_color(msg.level);
  }

  std::clog << msg.prefix << ' ' << msg.text;

  if (m_use_colors) {
    std::clog << kAnsiColorReset;
  }

  std::clog << '\n';
}

void ConsoleLogSink::flush()
{
  std::clog.flush();
}

void ConsoleLogSink::set_use_colors(const bool use_colors)
{
  m_use_colors = use_colors;
}

}  // namespace tactile
