// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path, directory_iterator, recursive_directory_iterator

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"

namespace tactile {

using FilePath = std::filesystem::path;
using DirectoryIterator = std::filesystem::directory_iterator;
using RecursiveDirectoryIterator = std::filesystem::recursive_directory_iterator;

namespace fs {

/**
 * \brief Converts a path to a string that is guaranteed to use forward slash characters.
 *
 * \details This function is useful when saving paths to files in a portable way. Since
 *          all relevant operating systems understand forward slashes, even if some
 *          operating systems prefer backslashes (most notably Windows).
 *
 * \param path a file path to convert.
 *
 * \return a file path string with forward slashes.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto use_forward_slashes(const FilePath& path) -> String;

/**
 * \brief Converts file paths to use a '~' prefix (if possible).
 *
 * \param path the file path to convert.
 *
 * \return a string with a shortened user directory prefix; or nothing if not applicable.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto use_short_home_prefix(const FilePath& path) -> Maybe<String>;

/**
 * \brief Indicates whether a file path starts with the home directory.
 *
 * \param path the file path to check.
 *
 * \return true if the path features the home directory prefix; false otherwise.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto has_home_prefix(const FilePath& path) -> bool;

}  // namespace fs

namespace fs_literals {

[[nodiscard]] inline auto operator"" _path(const char* str, const usize /* length */)
    -> FilePath
{
  return FilePath {str};
}

}  // namespace fs_literals
}  // namespace tactile
