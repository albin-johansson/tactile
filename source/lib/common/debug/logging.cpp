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

#include "logging.hpp"

#include <ctime>    // time
#include <utility>  // move

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "common/debug/panic.hpp"
#include "common/predef.hpp"
#include "common/type/chrono.hpp"
#include "common/type/deque.hpp"
#include "common/type/ptr.hpp"
#include "common/type/set.hpp"
#include "io/directories.hpp"

namespace tactile {
namespace {

using LogFilterSet = Set<LogLevel>;

struct LogEntry final {
  spdlog::level::level_enum level {};
  String msg;
};

[[nodiscard]] auto _to_filter_set(const LogFilter& filter) -> LogFilterSet
{
  LogFilterSet filter_set;

  if (filter.trace) {
    filter_set.emplace(LogLevel::trace);
  }

  if (filter.debug) {
    filter_set.emplace(LogLevel::debug);
  }

  if (filter.info) {
    filter_set.emplace(LogLevel::info);
  }

  if (filter.warn) {
    filter_set.emplace(LogLevel::warn);
  }

  if (filter.error) {
    filter_set.emplace(LogLevel::err);
  }

  if (filter.critical) {
    filter_set.emplace(LogLevel::critical);
  }

  return filter_set;
}

/// Records logged messages, intended to be displayed in the log dock.
class HistorySink final : public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
 public:
  void sink_it_(const spdlog::details::log_msg& msg) override
  {
    const auto time = fmt::localtime(Clock::to_time_t(msg.time));
    auto processed = fmt::format("[{:%H:%M:%S}] {}", time, msg.payload);
    mHistory.push_back({msg.level, std::move(processed)});
  }

  void flush_() override
  {
    // Do nothing
  }

  void clear() { mHistory.clear(); }

  [[nodiscard]] auto count(const LogFilterSet& set) const -> usize
  {
    usize count = 0;

    for (const auto& [level, str]: mHistory) {
      if (set.contains(level)) {
        ++count;
      }
    }

    return count;
  }

  /// Complexity: O(NlogN)
  void visit_logged_message_range(const LogFilterSet& set,
                                  const usize filtered_begin_index,
                                  const usize filtered_end_index,
                                  const LoggedMessageVisitorFn& fn) const
  {
    usize filtered_index = 0;
    for (const auto& [level, msg]: mHistory) {
      if (filtered_index >= filtered_end_index) {
        break;
      }

      if (set.contains(level)) {
        if (filtered_index >= filtered_begin_index) {
          fn(level, msg);
        }
        ++filtered_index;
      }
    }
  }

 private:
  Deque<LogEntry> mHistory;
};

inline Shared<HistorySink> gHistorySink;

}  // namespace

void init_logger()
{
  const auto path = get_persistent_file_dir() / "logs" / "tactile_log.txt";

  auto cs = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
  auto fs = std::make_shared<spdlog::sinks::basic_file_sink_st>(path.string(), true);
  gHistorySink = std::make_shared<HistorySink>();

  const spdlog::sinks_init_list sinks {cs, fs, gHistorySink};

  auto logger = std::make_shared<spdlog::logger>("tactile", sinks);
  logger->set_pattern("%^[%L][%T.%e]%$ %v");
  logger->flush_on(LogLevel::critical);

  spdlog::set_default_logger(logger);
  spdlog::set_level(kIsDebugBuild ? spdlog::level::trace : spdlog::level::info);

  spdlog::info("Tactile version {} ({} build)",
               TACTILE_VERSION_STRING,
               TACTILE_DEBUG ? "Unoptimized" : "Optimized");

  const auto time = fmt::localtime(std::time(nullptr));
  spdlog::info("Today is {:%A %Y-%m-%d}", time);
}

void clear_log_history()
{
  gHistorySink->clear();
}

auto count_matching_log_entries(const LogFilter& filter) -> usize
{
  const auto filter_set = _to_filter_set(filter);
  return gHistorySink->count(filter_set);
}

void visit_logged_message_range(const LogFilter& filter,
                                const usize begin_index,
                                const usize end_index,
                                const LoggedMessageVisitorFn& fn)
{
  const auto filter_set = _to_filter_set(filter);
  gHistorySink->visit_logged_message_range(filter_set, begin_index, end_index, fn);
}

}  // namespace tactile
