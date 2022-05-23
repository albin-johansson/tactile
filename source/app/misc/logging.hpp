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

#pragma once

#include <string>   // string
#include <utility>  // pair

#include <spdlog/spdlog.h>

#include "core/common/ints.hpp"

namespace tactile {

using LogLevel = spdlog::level::level_enum;

/**
 * \brief Initializes the logger, this must be called before any logging takes place.
 */
void init_logger();

/**
 * \brief Clears the entire log history.
 */
void clear_log_history();

/**
 * \brief Returns the log entry at a specific index amongst entries that satisfy a filter.
 *
 * \param filter the log level filter to apply before querying log entries.
 * \param index the index of the desired log entry among those that satisfy the filter.
 *
 * \return a pair of the found log level and logged message.
 *
 * \throws TactileError if no log entry was found.
 */
[[nodiscard]] auto get_log_entry(const LogLevel filter, const usize index)
    -> std::pair<LogLevel, const std::string&>;

/**
 * \brief Returns the amount of log entries that satisfy a filter.
 *
 * \param filter the filter that will be used.
 *
 * \return the amount of log entries that weren't filtered out.
 */
[[nodiscard]] auto log_size(LogLevel filter) -> usize;

}  // namespace tactile
