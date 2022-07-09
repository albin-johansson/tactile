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

#include "core/common/filesystem.hpp"

namespace tactile::io {

/**
 * Utility class for importing and saving files.
 *
 * This class is a simple wrapper around the native-file-dialogs library, with
 * some additional utilities for dealing with map and image files, etc.
 */
class FileDialog final
{
 public:
  [[nodiscard]] static auto open_file() -> FileDialog;

  [[nodiscard]] static auto open_map() -> FileDialog;

  [[nodiscard]] static auto open_image() -> FileDialog;

  [[nodiscard]] static auto save_map() -> FileDialog;

  [[nodiscard]] static auto save_image() -> FileDialog;

  [[nodiscard]] auto is_okay() const noexcept -> bool;

  [[nodiscard]] auto path() const -> fs::path;

 private:
  const char* mPath {};

  FileDialog(const char* path);
};

}  // namespace tactile::io
