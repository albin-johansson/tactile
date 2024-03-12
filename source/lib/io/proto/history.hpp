// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/file_history.hpp"
#include "tactile/base/container/maybe.hpp"

namespace tactile {

/// Loads the recent file history from the persistent file directory.
[[nodiscard]] auto load_file_history_from_disk() -> Maybe<FileHistory>;

/// Saves the in-memory file history to the persistent file directory.
void save_file_history_to_disk(const FileHistory& history);

}  // namespace tactile
