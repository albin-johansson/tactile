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

#include "common/type/path.hpp"
#include "model/model.hpp"
#include "model/persistence/file_history_components.hpp"

namespace tactile::sys {

void store_open_documents_in_file_history(Model& model);

/// Adds a file path to the recent file history, as long as it doesn't already exist.
void add_to_file_history(FileHistory& history, const Path& path);

/// Indicates whether there is a valid most recently closed file entry.
[[nodiscard]] auto is_last_closed_file_valid(const FileHistory& history) -> bool;

}  // namespace tactile::sys
