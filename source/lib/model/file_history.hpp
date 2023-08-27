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

#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/type/deque.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

// We store paths as strings because that makes displaying them
// in menus much easier (and faster)
struct FileHistory final {
  Maybe<String> last_closed_file;
  Deque<String> entries;
};

/// Clears the current in-memory recent file history.
void clear_file_history();

/// Adds a file path to the recent file history, if it doesn't already exist.
void add_to_file_history(const Path& path);

/// Sets the most recently closed file, and stores it in the file history.
void set_last_closed_file(const Path& path);

/// Indicates whether there is a valid most recently closed file entry.
[[nodiscard]] auto is_last_closed_file_valid() -> bool;

/// Overwrites the in-memory file history.
void set_file_history(FileHistory history);

/// Returns the current in-memory recent file history.
[[nodiscard]] auto get_file_history() -> const FileHistory&;

}  // namespace tactile
