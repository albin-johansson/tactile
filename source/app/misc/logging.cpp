/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "logging.hpp"

#include <ctime>  // time
#include <deque>  // deque

#include <fmt/chrono.h>
#include <fmt/core.h>

#include "throw.hpp"

namespace tactile {
namespace {

struct LogEntry final
{
  LogLevel level{};
  std::string str;
};

std::deque<LogEntry> _history;
LogLevel _log_level{LogLevel::info};

void _log(const fmt::color color,
          const LogLevel level,
          const std::string_view priority,
          const std::string_view fmt,
          const fmt::format_args args)
{
  const auto time = fmt::localtime(std::time(nullptr));
  const auto msg = fmt::vformat(fmt, args);
  const auto full = fmt::vformat("{:%H:%M:%S} > {}\n", fmt::make_format_args(time, msg));

  _history.push_back(LogEntry{level, full});

  if constexpr (is_debug_build) {
    print(color, "{:>9} {}", priority, full);
  }
}

}  // namespace

namespace logger {

void log_verbose_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(LogLevel::verbose)) {
    _log(fmt::color::violet, LogLevel::verbose, "[VERBOSE]", fmt, args);
  }
}

void log_debug_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(LogLevel::debug)) {
    _log(fmt::color::aquamarine, LogLevel::debug, "[DEBUG]", fmt, args);
  }
}

void log_info_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(LogLevel::info)) {
    _log(fmt::color::white, LogLevel::info, "[INFO]", fmt, args);
  }
}

void log_warning_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(LogLevel::warning)) {
    _log(fmt::color::yellow, LogLevel::warning, "[WARNING]", fmt, args);
  }
}

void log_error_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(LogLevel::error)) {
    _log(fmt::color::orange_red, LogLevel::error, "[ERROR]", fmt, args);
  }
}

}  // namespace logger

void clear_log_history()
{
  _history.clear();
}

void set_log_level(const LogLevel level)
{
  _log_level = level;
}

auto get_filtered_log_entry(const LogLevel filter, const usize index)
    -> std::pair<LogLevel, const std::string&>
{
  usize i = 0;

  for (const auto& [level, str] : _history) {
    if (is_enabled(filter, level)) {
      if (i == index) {
        return {level, str};
      }
      else {
        ++i;
      }
    }
  }

  panic("Invalid index for filtered log entry!");
}

auto log_size(const LogLevel filter) -> usize
{
  usize count = 0;

  for (const auto& [level, str] : _history) {
    if (is_enabled(filter, level)) {
      ++count;
    }
  }

  return count;
}

auto is_enabled(const LogLevel filter, const LogLevel level) -> bool
{
  switch (level) {
    case LogLevel::verbose:
      return filter == LogLevel::verbose;

    case LogLevel::debug:
      return filter == LogLevel::verbose || filter == LogLevel::debug;

    case LogLevel::info:
      return filter == LogLevel::verbose || filter == LogLevel::debug ||
             filter == LogLevel::info;

    case LogLevel::warning:
      return filter == LogLevel::verbose || filter == LogLevel::debug ||
             filter == LogLevel::info || filter == LogLevel::warning;

    case LogLevel::error:
      return true; /* Errors are always logged */

    default:
      return false;
  }
}

auto is_enabled(const LogLevel level) -> bool
{
  return is_enabled(_log_level, level);
}

}  // namespace tactile
