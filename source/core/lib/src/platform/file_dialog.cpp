// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/platform/file_dialog.hpp"

#include <array>  // array

#include <tinyfiledialogs.h>

namespace tactile::core {
namespace {

inline constexpr const char* kMapPatternDescriptor = "Map files";
inline constexpr const char* kImagePatternDescriptor = "Image files";

inline constexpr std::array kMapPatterns {
  "*.yaml",
  "*.yml",
  "*.json",
  "*.tmj",
  "*.tmx",
  "*.xml",
};

inline constexpr std::array kImagePatterns {
  "*.png",
  "*.jpg",
  "*.jpeg",
};

}  // namespace

auto FileDialog::open_folder() -> std::optional<std::filesystem::path>
{
  if (const auto* path = tinyfd_selectFolderDialog("Open Folder", nullptr)) {
    return path;
  }

  return std::nullopt;
}

auto FileDialog::open_file() -> std::optional<std::filesystem::path>
{
  if (const auto* path =
          tinyfd_openFileDialog("Open File", nullptr, 0, nullptr, "Any file", 0)) {
    return path;
  }

  return std::nullopt;
}

auto FileDialog::open_map() -> std::optional<std::filesystem::path>
{
  if (const auto* path = tinyfd_openFileDialog("Open Map",
                                               nullptr,
                                               std::ssize(kMapPatterns),
                                               kMapPatterns.data(),
                                               kMapPatternDescriptor,
                                               0)) {
    return path;
  }

  return std::nullopt;
}

auto FileDialog::open_image() -> std::optional<std::filesystem::path>
{
  if (const auto* path = tinyfd_openFileDialog("Open Image",
                                               nullptr,
                                               std::ssize(kImagePatterns),
                                               kImagePatterns.data(),
                                               kImagePatternDescriptor,
                                               0)) {
    return path;
  }

  return std::nullopt;
}

auto FileDialog::save_map() -> std::optional<std::filesystem::path>
{
  if (const auto* path = tinyfd_saveFileDialog("Save Map",
                                               nullptr,
                                               std::ssize(kMapPatterns),
                                               kMapPatterns.data(),
                                               kMapPatternDescriptor)) {
    return path;
  }

  return std::nullopt;
}

auto FileDialog::save_image() -> std::optional<std::filesystem::path>
{
  if (const auto* path = tinyfd_saveFileDialog("Save Image",
                                               nullptr,
                                               std::ssize(kImagePatterns),
                                               kImagePatterns.data(),
                                               kImagePatternDescriptor)) {
    return path;
  }

  return std::nullopt;
}

}  // namespace tactile::core
