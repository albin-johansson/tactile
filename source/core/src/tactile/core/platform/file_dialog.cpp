// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/file_dialog.hpp"

#include <tinyfiledialogs.h>

#include "tactile/base/container/array.hpp"
#include "tactile/core/debug/generic_error.hpp"

namespace tactile {
namespace {

inline constexpr const char* kMapPatternDescriptor = "Map files";
inline constexpr const char* kImagePatternDescriptor = "Image files";

inline constexpr Array<const char*, 6> kMapPatterns {
  "*.yaml",
  "*.yml",
  "*.json",
  "*.tmj",
  "*.tmx",
  "*.xml",
};

inline constexpr Array<const char*, 3> kImagePatterns {
  "*.png",
  "*.jpg",
  "*.jpeg",
};

}  // namespace

auto FileDialog::open_folder() -> Result<Path>
{
  if (const auto* path = tinyfd_selectFolderDialog("Open Folder", nullptr)) {
    return path;
  }

  return unexpected(make_error(GenericError::kInvalidFile));
}

auto FileDialog::open_file() -> Result<Path>
{
  if (const auto* path = tinyfd_openFileDialog("Open File",
                                               nullptr,
                                               0,
                                               nullptr,
                                               "Any file",
                                               0)) {
    return path;
  }

  return unexpected(make_error(GenericError::kInvalidFile));
}

auto FileDialog::open_map() -> Result<Path>
{
  if (const auto* path = tinyfd_openFileDialog("Open Map",
                                               nullptr,
                                               std::ssize(kMapPatterns),
                                               kMapPatterns.data(),
                                               kMapPatternDescriptor,
                                               0)) {
    return path;
  }

  return unexpected(make_error(GenericError::kInvalidFile));
}

auto FileDialog::open_image() -> Result<Path>
{
  if (const auto* path = tinyfd_openFileDialog("Open Image",
                                               nullptr,
                                               std::ssize(kImagePatterns),
                                               kImagePatterns.data(),
                                               kImagePatternDescriptor,
                                               0)) {
    return path;
  }

  return unexpected(make_error(GenericError::kInvalidFile));
}

auto FileDialog::save_map() -> Result<Path>
{
  if (const auto* path = tinyfd_saveFileDialog("Save Map",
                                               nullptr,
                                               std::ssize(kMapPatterns),
                                               kMapPatterns.data(),
                                               kMapPatternDescriptor)) {
    return path;
  }

  return unexpected(make_error(GenericError::kInvalidFile));
}

auto FileDialog::save_image() -> Result<Path>
{
  if (const auto* path = tinyfd_saveFileDialog("Save Image",
                                               nullptr,
                                               std::ssize(kImagePatterns),
                                               kImagePatterns.data(),
                                               kImagePatternDescriptor)) {
    return path;
  }

  return unexpected(make_error(GenericError::kInvalidFile));
}

}  // namespace tactile
