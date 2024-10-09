// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <filesystem>  // path
#include <optional>    // optional

#include "tactile/base/prelude.hpp"

namespace tactile::core {

/**
 * Utility for importing and saving files.
 *
 * \details
 * This class is a simple wrapper around the native-file-dialogs library, with
 * some additional utilities for dealing with map and image files, etc.
 */
class FileDialog final
{
 public:
  TACTILE_STATIC_CLASS(FileDialog);

  /**
   * Shows a file dialog that can be used for selecting folders.
   *
   * \return
   * A folder path if successful; an empty optional otherwise.
   */
  [[nodiscard]]
  static auto open_folder() -> std::optional<std::filesystem::path>;

  /**
   * Shows a file dialog that can be used for selecting files.
   *
   * \return
   * A file path if successful; an empty optional otherwise.
   */
  [[nodiscard]]
  static auto open_file() -> std::optional<std::filesystem::path>;

  /**
   * Shows a file dialog that can be used for selecting map files.
   *
   * \return
   * A map file path if successful; an empty optional otherwise.
   */
  [[nodiscard]]
  static auto open_map() -> std::optional<std::filesystem::path>;

  /**
   * Shows a file dialog that can be used for selecting image files.
   *
   * \return
   * An image file path if successful; an empty optional otherwise.
   */
  [[nodiscard]]
  static auto open_image() -> std::optional<std::filesystem::path>;

  /**
   * Shows a file dialog that can be used for selecting map file paths.
   *
   * \return
   * A map file path if successful; an empty optional otherwise.
   */
  [[nodiscard]]
  static auto save_map() -> std::optional<std::filesystem::path>;

  /**
   * Shows a file dialog that can be used for selecting image file paths.
   *
   * \return
   * An image file path if successful; an empty optional otherwise.
   */
  [[nodiscard]]
  static auto save_image() -> std::optional<std::filesystem::path>;
};

}  // namespace tactile::core
