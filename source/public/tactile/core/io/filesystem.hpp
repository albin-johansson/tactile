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

#include <filesystem>   // path, directory_iterator, recursive_directory_iterator
#include <string>       // basic_string
#include <string_view>  // basic_string_view

#include "tactile/core/api.hpp"
#include "tactile/core/common/prelude.hpp"
#include "tactile/core/containers/string.hpp"
#include "tactile/core/functional/maybe.hpp"

namespace tactile {

namespace fs = std::filesystem;

using Path = fs::path;
using DirectoryIterator = fs::directory_iterator;
using RecursiveDirectoryIterator = fs::recursive_directory_iterator;

using NativeChar = Path::value_type;
using NativeString = std::basic_string<NativeChar>;
using NativeStringView = std::basic_string_view<NativeChar>;

/**
 * Converts a path to a string that is guaranteed to use forward slashes.
 *
 * \details This function is useful when saving paths to files in a portable way. Since
 *          all relevant operating systems understand forward slashes, even if some
 *          operating systems prefer backslashes (e.g., Windows).
 *
 * \param path the file path that will be converted.
 *
 * \return a version of the path using forward slashes instead of backslashes.
 */
[[nodiscard]] TACTILE_CORE_API auto to_forward_slashes_path(const Path& path) -> String;

/** Indicates whether a file path starts with the home directory. */
[[nodiscard]] TACTILE_CORE_API auto has_home_prefix(const Path& path) -> bool;

/** Converts file paths to use a '~' prefix (if possible). */
[[nodiscard]] TACTILE_CORE_API auto use_short_home_prefix(const Path& path)
    -> Maybe<String>;

/** Creates a string using the native filesystem character type. */
[[nodiscard]] TACTILE_CORE_API auto make_native_string(const char* str)
    -> Maybe<NativeString>;

}  // namespace tactile
