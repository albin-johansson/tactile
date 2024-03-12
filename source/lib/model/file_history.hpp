// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/deque.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"

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
