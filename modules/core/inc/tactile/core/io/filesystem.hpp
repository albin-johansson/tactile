// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path, directory_iterator, recursive_directory_iterator

#include "tactile/core/api.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

namespace fs = std::filesystem;

using Path = fs::path;
using DirectoryIterator = fs::directory_iterator;
using RecursiveDirectoryIterator = fs::recursive_directory_iterator;

/**
 * Converts a path to a string that is guaranteed to use forward slash characters.
 *
 * \details This function is useful when saving paths to files in a portable way. Since
 *          all relevant operating systems understand forward slashes, even if some
 *          operating systems prefer backslashes (most notably Windows).
 *
 * \param path a file path to convert.
 *
 * \return a file path string with forward slashes.
 */
[[nodiscard]] TACTILE_CORE_API auto use_forward_slashes(const Path& path) -> String;

/** Converts file paths to use a '~' prefix (if possible). */
[[nodiscard]] TACTILE_CORE_API auto use_short_home_prefix(const Path& path)
    -> Maybe<String>;

/** Indicates whether a file path starts with the home directory. */
[[nodiscard]] TACTILE_CORE_API auto has_home_prefix(const Path& path) -> bool;

}  // namespace tactile
