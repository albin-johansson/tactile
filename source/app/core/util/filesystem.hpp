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

#include "core/common/fs.hpp"
#include "core/common/maybe.hpp"
#include "core/type/string.hpp"

namespace tactile {

/// Converts a path to a string that is guaranteed to use forward slashes.
///
/// This function is useful when saving paths to files in a portable way. Since all
/// relevant operating systems understand forward slashes, even if some operating systems
/// prefer backslashes (e.g. Windows).
///
/// \param path the file path that will be converted.
///
/// \return a version of the path with forward slashes instead of backslashes.
[[nodiscard]] auto convert_to_forward_slashes(const fs::path& path) -> String;

/// Indicates whether a file path starts with the home directory.
[[nodiscard]] auto has_home_prefix(const fs::path& path) -> bool;

/// Converts file paths to use a '~' prefix (if possible).
[[nodiscard]] auto to_canonical(const fs::path& path) -> Maybe<String>;

/// Creates a string using the native filesystem character type.
[[nodiscard]] auto to_fs_string(const char* str) -> Maybe<FileString>;

}  // namespace tactile
