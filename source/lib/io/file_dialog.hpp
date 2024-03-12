// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"

namespace tactile {

/// Utility class for importing and saving files.
///
/// This class is a simple wrapper around the native-file-dialogs library, with
/// some additional utilities for dealing with map and image files, etc.
class FileDialog final {
 public:
  [[nodiscard]] static auto open_folder() -> FileDialog;

  [[nodiscard]] static auto open_file() -> FileDialog;

  [[nodiscard]] static auto open_map() -> FileDialog;

  [[nodiscard]] static auto open_image() -> FileDialog;

  [[nodiscard]] static auto save_map() -> FileDialog;

  [[nodiscard]] static auto save_image() -> FileDialog;

  [[nodiscard]] auto is_okay() const noexcept -> bool;

  [[nodiscard]] auto path() const -> Path;

 private:
  const char* mPath {};

  /* implicit */ FileDialog(const char* path);
};

}  // namespace tactile
