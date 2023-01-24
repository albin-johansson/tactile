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
