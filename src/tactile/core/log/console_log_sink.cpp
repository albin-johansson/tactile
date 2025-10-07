// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/log/console_log_sink.hpp"

#include <iostream>
#include <ostream>

#include "tactile/core/container/string.hpp"

namespace tactile {
namespace {

constexpr StringView kAnsiColorReset = "\x1B[0m";
constexpr StringView kAnsiColorFgRedBold = "\x1B[31m\x1B[1m";
constexpr StringView kAnsiColorFgYellow = "\x1B[33m";
constexpr StringView kAnsiColorFgMagenta = "\x1B[35m";
constexpr StringView kAnsiColorFgCyan = "\x1B[36m";
constexpr StringView kAnsiColorFgWhite = "\x1B[37m";

auto to_ansi_color(const LogCategory category) noexcept -> StringView
{
  switch (category) {
    case LogCategory::kVerbose: return kAnsiColorFgMagenta;
    case LogCategory::kDebug:   return kAnsiColorFgCyan;
    case LogCategory::kInfo:    return kAnsiColorFgWhite;
    case LogCategory::kWarn:    return kAnsiColorFgYellow;
    case LogCategory::kError:   return kAnsiColorFgRedBold;
    default:                    return kAnsiColorFgWhite;
  }
}

}  // namespace

void ConsoleLogSink::flush()
{
  std::clog << std::flush;
}

void ConsoleLogSink::log(const LogEntry& entry)
{
  if (m_ansi_colors) {
    std::clog << to_ansi_color(entry.category);
  }

  std::clog << entry.prefix << ' ' << entry.message;

  if (m_ansi_colors) {
    std::clog << kAnsiColorReset;
  }

  std::clog << '\n';
}

void ConsoleLogSink::set_ansi_colors(const bool enabled)
{
  m_ansi_colors = enabled;
}

}  // namespace tactile
