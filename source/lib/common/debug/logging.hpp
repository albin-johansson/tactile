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

#include <spdlog/spdlog.h>

#include "common/primitives.hpp"
#include "common/predef.hpp"
#include "common/type/func.hpp"
#include "common/type/string.hpp"

namespace tactile {

struct LogFilter final {
  bool trace    : 1 {TACTILE_DEBUG};
  bool debug    : 1 {TACTILE_DEBUG};
  bool info     : 1 {true};
  bool warn     : 1 {true};
  bool error    : 1 {true};
  bool critical : 1 {true};
};

using LogLevel = spdlog::level::level_enum;
using LoggedMessageVisitorFn = Func<void(LogLevel, const String&)>;

/// Initializes the logger, this must be called before any logging takes place.
void init_logger();

/// Clears the entire log history.
void clear_log_history();

/// Returns the amount of logged messages that satisfy the provided filter.
[[nodiscard]] auto count_matching_log_entries(const LogFilter& filter) -> usize;

/// Invokes a callback for a range of logged messages matching the provided filter.
void visit_logged_message_range(const LogFilter& filter,
                                usize begin_index,
                                usize end_index,
                                const LoggedMessageVisitorFn& fn);

}  // namespace tactile
