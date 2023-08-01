/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "log_history_sink.hpp"

#include <utility>  // move

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "common/type/chrono.hpp"
#include "core/debug/panic.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto _convert_log_level(const spdlog::level::level_enum level)
    -> LogLevelBits
{
  switch (level) {
    case spdlog::level::trace:
      return LOG_LEVEL_TRACE;

    case spdlog::level::debug:
      return LOG_LEVEL_DEBUG;

    case spdlog::level::info:
      return LOG_LEVEL_INFO;

    case spdlog::level::warn:
      return LOG_LEVEL_WARNING;

    case spdlog::level::err:
      return LOG_LEVEL_ERROR;

    case spdlog::level::critical:
      return LOG_LEVEL_CRITICAL;

    default:
      throw TactileError {"Unsupported log level"};
  }
}

}  // namespace

void LogHistorySink::sink_it_(const spdlog::details::log_msg& msg)
{
  const auto time = fmt::localtime(Clock::to_time_t(msg.time));

  const auto level = _convert_log_level(msg.level);
  auto formatted_msg = fmt::format("[{:%H:%M:%S}] {}", time, msg.payload);

  mMessages.push_back(LogEntry {level, std::move(formatted_msg)});
}

void LogHistorySink::flush_()
{
  // Do nothing
}

void LogHistorySink::clear()
{
  mMessages.clear();
}

}  // namespace tactile
