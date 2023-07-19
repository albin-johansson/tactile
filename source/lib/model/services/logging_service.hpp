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

#pragma once

#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>

#include "common/predef.hpp"
#include "common/primitives.hpp"
#include "common/type/deque.hpp"
#include "common/type/func.hpp"
#include "common/type/ptr.hpp"
#include "common/type/string.hpp"

namespace tactile {

enum LogLevelBits : uint32 {
  LOG_LEVEL_TRACE = 1u << 0u,
  LOG_LEVEL_DEBUG = 1u << 1u,
  LOG_LEVEL_INFO = 1u << 2u,
  LOG_LEVEL_WARNING = 1u << 3u,
  LOG_LEVEL_ERROR = 1u << 4u,
  LOG_LEVEL_CRITICAL = 1u << 5u,
};

using LogLevelFlags = uint32;

inline constexpr LogLevelFlags kAllLogLevels = LOG_LEVEL_TRACE | LOG_LEVEL_DEBUG |
                                               LOG_LEVEL_INFO | LOG_LEVEL_WARNING |
                                               LOG_LEVEL_ERROR | LOG_LEVEL_CRITICAL;

#if TACTILE_DEBUG
inline constexpr LogLevelFlags kDefaultLogLevels = kAllLogLevels;
#else
inline constexpr LogLevelFlags kDefaultLogLevels = LOG_LEVEL_INFO |     //
                                                   LOG_LEVEL_WARNING |  //
                                                   LOG_LEVEL_ERROR |    //
                                                   LOG_LEVEL_CRITICAL;
#endif  // TACTILE_DEBUG

struct LogEntry final {
  LogLevelBits level;
  String msg;
};

class LogHistorySink final :
    public spdlog::sinks::base_sink<spdlog::details::null_mutex> {
 public:
  void sink_it_(const spdlog::details::log_msg& msg) override;

  void flush_() override;

  void clear();

  [[nodiscard]] auto get_messages() const -> const Deque<LogEntry>& { return mMessages; }

 private:
  Deque<LogEntry> mMessages;
};

class LoggingService final {
 public:
  using LogMessageCallback = Func<void(LogLevelBits, StringView)>;

  /// Installs the global logger.
  void install_logger();

  /// Clears the in-memory history of logged messages.
  void clear_history();

  /**
   * Visits each logged message that satisfies the specified log level filter and range.
   *
   * \complexity O(N)
   *
   * \param filtered_begin_index the index of the first passing message to include.
   * \param filtered_end_index   the index of the last passing message to include.
   * \param log_level_flags      a bitmask of log levels used as a message filter.
   * \param callback             a callback invoked for each passing message.
   */
  void each_logged_message(usize filtered_begin_index,
                           usize filtered_end_index,
                           LogLevelFlags log_level_flags,
                           const LogMessageCallback& callback) const;

  /**
   * Returns the number of messages that match the specified log levels.
   *
   * \complexity O(N)
   *
   * \param log_level_flags a bitmask of log levels used as a message filter.
   *
   * \return the number of matching messages.
   */
  [[nodiscard]] auto count_logged_messages(LogLevelFlags log_level_flags) const -> usize;

 private:
  Shared<LogHistorySink> mHistorySink;
};

}  // namespace tactile
