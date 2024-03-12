// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"

namespace tactile {

/// Opens a directory in the OS file explorer
void open_directory(const Path& dir);

/// Returns the path to a resource.
///
/// This function should be used to locate all resource files, such as images, to ensure
/// that resource paths are resolved in app bundles on macOS.
///
/// \param resource the path to the resource, e.g "assets/foo.png".
///
/// \return the resolved file path.
[[nodiscard]] auto find_resource(const char* resource) -> Path;

/// Returns the file path to the associated `imgui.ini` file.
[[nodiscard]] auto get_widget_ini_path() -> const Path&;

/// Returns the path to the persistent file directory.
[[nodiscard]] auto get_persistent_file_dir() -> const Path&;

}  // namespace tactile
