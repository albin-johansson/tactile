#pragma once

#include <filesystem>  // path

#include <nfd.h>
#include <tactile_def.hpp>

namespace tactile {

/**
 * \brief Utility class for importing and saving files.
 *
 * \details This class is a simple wrapper around the native-file-dialogs library, with
 * some additional utilities for dealing with map and image files, etc.
 */
class file_dialog final {
 public:
  TACTILE_DELETE_COPY(file_dialog)
  TACTILE_DELETE_MOVE(file_dialog)

  ~file_dialog() noexcept;

  [[nodiscard]] static auto open_file(const nfdchar_t* filter = nullptr,
                                      const nfdchar_t* defaultPath = nullptr)
      -> file_dialog;

  [[nodiscard]] static auto open_map() -> file_dialog;

  [[nodiscard]] static auto open_image() -> file_dialog;

  [[nodiscard]] static auto save_file(const nfdchar_t* filter = nullptr,
                                      const nfdchar_t* defaultPath = nullptr)
      -> file_dialog;

  [[nodiscard]] static auto save_map() -> file_dialog;

  [[nodiscard]] static auto save_image() -> file_dialog;

  [[nodiscard]] auto is_okay() const noexcept -> bool;

  [[nodiscard]] auto was_canceled() const noexcept -> bool;

  [[nodiscard]] auto path() const -> std::filesystem::path;

 private:
  nfdresult_t mResult{};
  nfdchar_t* mPath{};

  file_dialog(nfdresult_t result, nfdchar_t* path);
};

}  // namespace tactile
