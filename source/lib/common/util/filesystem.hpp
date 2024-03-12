// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"

namespace tactile {

/// Converts a path to a string that is guaranteed to use forward slashes.
///
/// This function is useful when saving paths to files in a portable way. Since all
/// relevant operating systems understand forward slashes, even if some operating systems
/// prefer backslashes (e.g. Windows).
///
/// \param path the file path that will be converted.
/// \return a version of the path with forward slashes instead of backslashes.
[[nodiscard]] auto use_forward_slashes(const Path& path) -> String;

/// Indicates whether a file path starts with the home directory.
[[nodiscard]] auto has_home_prefix(const Path& path) -> bool;

/// Converts file paths to use a '~' prefix (if possible).
[[nodiscard]] auto use_short_home_prefix(const Path& path) -> Maybe<String>;

/// Creates a string using the native filesystem character type.
[[nodiscard]] auto make_native_string(const char* str) -> Maybe<OsString>;

}  // namespace tactile
