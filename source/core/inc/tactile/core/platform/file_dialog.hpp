// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Utility for importing and saving files.
 *
 * \details
 *    This class is a simple wrapper around the native-file-dialogs library,
 *    with some additional utilities for dealing with map and image files, etc.
 */
class FileDialog final {
 public:
  TACTILE_STATIC_CLASS(FileDialog);

  /**
   * Shows a file dialog that can be used for selecting folders.
   *
   * \return
   *    The selected folder; or an error code if none was selected.
   */
  [[nodiscard]]
  static auto open_folder() -> Result<Path>;

  /**
   * Shows a file dialog that can be used for selecting files.
   *
   * \return
   *    The selected file; or an error code if none was selected.
   */
  [[nodiscard]]
  static auto open_file() -> Result<Path>;

  /**
   * Shows a file dialog that can be used for selecting map files.
   *
   * \return
   *    The selected map file; or an error code if none was selected.
   */
  [[nodiscard]]
  static auto open_map() -> Result<Path>;

  /**
   * Shows a file dialog that can be used for selecting image files.
   *
   * \return
   *    The selected image file; or an error code if none was selected.
   */
  [[nodiscard]]
  static auto open_image() -> Result<Path>;

  /**
   * Shows a file dialog that can be used for selecting map file paths.
   *
   * \return
   *    The chosen map file path; or an error code if none was picked.
   */
  [[nodiscard]]
  static auto save_map() -> Result<Path>;

  /**
   * Shows a file dialog that can be used for selecting image file paths.
   *
   * \return
   *    The chosen image file path; or an error code if none was picked.
   */
  [[nodiscard]]
  static auto save_image() -> Result<Path>;
};

}  // namespace tactile