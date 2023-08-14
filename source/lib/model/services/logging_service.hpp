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

#include "tactile/core/containers/deque.hpp"
#include "core/containers/smart_ptr.hpp"
#include "tactile/core/containers/string.hpp"
#include "core/functional/function.hpp"
#include "core/prelude.hpp"
#include "model/services/logging/log_level.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(LogHistorySink)

class LoggingService final {
 public:
  using LogMessageCallback = Function<void(LogLevelBits, StringView)>;

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
