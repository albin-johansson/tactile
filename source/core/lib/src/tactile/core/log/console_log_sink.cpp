// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/log/console_log_sink.hpp"

#include <stdexcept>  // invalid_argument

#include <fmt/color.h>
#include <fmt/core.h>

namespace tactile {
namespace {

[[nodiscard]]
auto _to_fg_color(const LogLevel level) -> fmt::color
{
  switch (level) {
    case LogLevel::kTrace: return fmt::color::magenta;
    case LogLevel::kDebug: return fmt::color::dodger_blue;
    case LogLevel::kInfo:  return fmt::color::medium_sea_green;
    case LogLevel::kWarn:  return fmt::color::golden_rod;
    case LogLevel::kError: return fmt::color::orange_red;
    default:               throw std::invalid_argument {"bad log level"};
  }
}

}  // namespace

void ConsoleLogSink::log(const LogMessage& msg)
{
  const auto style = fmt::fg(_to_fg_color(msg.level));
  fmt::print(style, "{} {}\n", msg.prefix, msg.text);
}

void ConsoleLogSink::flush()
{}

}  // namespace tactile
