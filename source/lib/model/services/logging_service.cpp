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

#include "logging_service.hpp"

#include <ctime>    // time
#include <utility>  // move

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/type/chrono.hpp"
#include "io/directories.hpp"

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

void LoggingService::install_logger()
{
  using ColorSink = spdlog::sinks::stdout_color_sink_st;
  using FileSink = spdlog::sinks::basic_file_sink_st;

  const auto log_file_path = get_persistent_file_dir() / "logs" / "tactile_log.txt";

  auto color_sink = std::make_shared<ColorSink>();
  auto file_sink = std::make_shared<FileSink>(log_file_path.string(), true);
  mHistorySink = std::make_shared<LogHistorySink>();

  const spdlog::sinks_init_list sinks {color_sink, file_sink, mHistorySink};

  auto logger = std::make_shared<spdlog::logger>("tactile", sinks);
  logger->set_pattern("%^[%L][%T.%e]%$ %v");
  logger->flush_on(spdlog::level::critical);

  spdlog::set_default_logger(logger);
  spdlog::set_level(kIsDebugBuild ? spdlog::level::trace : spdlog::level::info);

  spdlog::info("Tactile version {} ({} build)",
               TACTILE_VERSION_STRING,
               TACTILE_DEBUG ? "Unoptimized" : "Optimized");

  const auto time = fmt::localtime(std::time(nullptr));
  spdlog::info("Today is {:%A %Y-%m-%d}", time);
}

void LoggingService::clear_history()
{
  mHistorySink->clear();
}

void LoggingService::each_logged_message(const usize filtered_begin_index,
                                         const usize filtered_end_index,
                                         const LogLevelFlags log_level_flags,
                                         const LogMessageCallback& callback) const
{
  usize filtered_index = 0;
  for (const auto& [level, msg]: mHistorySink->get_messages()) {
    if (filtered_index >= filtered_end_index) {
      break;
    }

    if (level & log_level_flags) {
      if (filtered_index >= filtered_begin_index) {
        callback(level, msg);
      }

      ++filtered_index;
    }
  }
}

auto LoggingService::count_logged_messages(const LogLevelFlags log_level_flags) const
    -> usize
{
  usize count = 0;

  for (const auto& [level, msg]: mHistorySink->get_messages()) {
    if (level & log_level_flags) {
      ++count;
    }
  }

  return count;
}

}  // namespace tactile
