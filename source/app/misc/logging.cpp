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

#include <ctime>    // time
#include <deque>    // deque
#include <utility>  // move

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "core/common/memory.hpp"
#include "io/directories.hpp"
#include "meta/build.hpp"
#include "misc/panic.hpp"

namespace tactile {
namespace {

struct LogEntry final
{
  spdlog::level::level_enum level {};
  std::string               msg;
};

/**
 * Records logged messages, intended to be displayed in the log dock.
 */
class HistorySink final : public spdlog::sinks::base_sink<spdlog::details::null_mutex>
{
 public:
  void sink_it_(const spdlog::details::log_msg& msg) override
  {
    const auto time = fmt::localtime(msg.time);
    auto       processed = fmt::format("{:%H:%M:%S} > {}", time, msg.payload);
    mHistory.push_back({msg.level, std::move(processed)});
  }

  void flush_() override
  {
    // Do nothing
  }

  void clear()
  {
    mHistory.clear();
  }

  [[nodiscard]] auto get_entry(const LogLevel filter, usize index)
      -> std::pair<LogLevel, const std::string&>
  {
    usize i = 0;

    for (const auto& [level, str] : mHistory) {
      if (level >= filter) {
        if (i == index) {
          return {level, str};
        }

        ++i;
      }
    }

    throw TactileError("Invalid index for filtered log entry!");
  }

  [[nodiscard]] auto count(const LogLevel filter) const -> usize
  {
    usize count = 0;

    for (const auto& [level, str] : mHistory) {
      if (level >= filter) {
        ++count;
      }
    }

    return count;
  }

 private:
  std::deque<LogEntry> mHistory;
};

inline Shared<HistorySink> _history_sink;

}  // namespace

void init_logger()
{
  const auto path = io::persistent_file_dir() / "logs" / "tactile_log.txt";

  auto cs = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
  auto fs = std::make_shared<spdlog::sinks::basic_file_sink_st>(path.string(), true);
  _history_sink = std::make_shared<HistorySink>();
  spdlog::sinks_init_list sinks {cs, fs, _history_sink};

  auto logger = std::make_shared<spdlog::logger>("tactile", sinks);
  logger->set_pattern("[%T.%f] %^{%l}%$ > %v");
  logger->flush_on(LogLevel::critical);

  spdlog::set_default_logger(logger);
  spdlog::set_level(is_debug_build ? spdlog::level::trace : spdlog::level::debug);

  spdlog::info("Tactile version " TACTILE_VERSION_STRING);

  const auto time = fmt::localtime(std::time(nullptr));
  spdlog::info("Today is {:%A %Y-%m-%d}", time);
}

void clear_log_history()
{
  _history_sink->clear();
}

auto get_log_entry(const LogLevel filter, const usize index)
    -> std::pair<LogLevel, const std::string&>
{
  return _history_sink->get_entry(filter, index);
}

auto log_size(const LogLevel filter) -> usize
{
  return _history_sink->count(filter);
}

}  // namespace tactile
