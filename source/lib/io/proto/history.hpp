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

#include "common/type/deque.hpp"
#include "common/type/path.hpp"
#include "common/type/string.hpp"

namespace tactile {

/// Loads the recent file history from the persistent file directory.
void load_file_history_from_disk();

/// Saves the in-memory file history to the persistent file directory.
void save_file_history_to_disk();

/// Clears the current in-memory recent file history.
void clear_file_history();

/// Adds a file path to the recent file history, if it doesn't already exist.
void add_to_file_history(const Path& path);

/// Sets the most recently closed file, and stores it in the file history.
void set_last_closed_file(const Path& path);

/// Returns the current in-memory recent file history.
[[nodiscard]] auto get_file_history() -> const Deque<String>&;

/// Returns the file path to the most recently closed file.
[[nodiscard]] auto get_last_closed_file() -> const String&;

/// Indicates whether there is a valid most recently closed file entry.
[[nodiscard]] auto is_last_closed_file_valid() -> bool;

}  // namespace tactile
