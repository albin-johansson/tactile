#pragma once

#include <filesystem>  // path

#include "tactile.hpp"

namespace tactile {

/**
 * \brief Utility class for importing and saving files.
 *
 * \details This class is a simple wrapper around the native-file-dialogs library, with
 * some additional utilities for dealing with map and image files, etc.
 */
class file_dialog final
{
 public:
  [[nodiscard]] static auto open_file() -> file_dialog;

  [[nodiscard]] static auto open_map() -> file_dialog;

  [[nodiscard]] static auto open_image() -> file_dialog;

  [[nodiscard]] static auto save_map() -> file_dialog;

  [[nodiscard]] static auto save_image() -> file_dialog;

  [[nodiscard]] auto is_okay() const noexcept -> bool;

  [[nodiscard]] auto path() const -> std::filesystem::path;

 private:
  const char* mPath{};

  file_dialog(const char* path);
};

}  // namespace tactile
