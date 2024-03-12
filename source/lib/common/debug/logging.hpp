// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <spdlog/spdlog.h>

#include "tactile/base/container/function.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

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
using LoggedMessageVisitorFn = Function<void(LogLevel, const String&)>;

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
