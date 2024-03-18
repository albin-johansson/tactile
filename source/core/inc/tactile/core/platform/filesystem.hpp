// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Attempts to open a directory in the operating system file explorer.
 *
 * \param dir The directory to open.
 *
 * \return
 *    Nothing if the function tried to open the directory; an error code otherwise.
 */
auto open_directory_in_finder(const Path& dir) -> Result<void>;

/**
 * Returns the file path to the persistent storage directory associated with the app.
 *
 * \return
 *    A directory path; or an error code if something went wrong.
 */
[[nodiscard]]
auto get_persistent_storage_directory() -> Result<Path>;

/**
 * Returns the file path to the user home directory.
 *
 * \return
 *    A directory path; or an error code if something went wrong.
 */
[[nodiscard]]
auto get_user_home_directory() -> Result<NativeString>;

/**
 * Returns the path to the associated \c imgui.ini file.
 *
 * \return
 *    A file path.
 */
[[nodiscard]]
auto get_imgui_ini_file_path() -> Path;

/**
 * Converts a path to a string that is guaranteed to use forward slash characters.
 *
 * \details
 *    This function is useful when saving paths to files in a portable way. Since all
 *    relevant operating systems understand forward slashes, even if some operating
 *    systems prefer backslashes (most notably Windows).
 *
 * \param path A file path to convert.
 *
 * \return
 *    A file path string that uses forward slashes.
 */
[[nodiscard]]
auto normalize_path(const Path& path) -> String;

/**
 * Replaces the user home directory prefix in a file path with '~'.
 *
 * \param path     The file path.
 * \param home_dir The user home directory path.
 *
 * \return
 *    A string with a shortened home directory prefix; or an error code if not applicable.
 */
[[nodiscard]]
auto strip_home_directory_prefix(const Path& path, const NativeString& home_dir)
    -> Result<NativeString>;

}  // namespace tactile