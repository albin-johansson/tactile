// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "file_dialog.hpp"

#include <tinyfiledialogs.h>

#include "tactile/base/container/array.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile {
namespace {

constexpr const char* kMapPatternDescriptor = "Map files";
constexpr const char* kImagePatternDescriptor = "Image files";

constexpr Array<const char*, 6> kMapPatterns {"*.yaml",
                                              "*.yml",
                                              "*.json",
                                              "*.tmj",
                                              "*.tmx",
                                              "*.xml"};
constexpr Array<const char*, 3> kImagePatterns {"*.png", "*.jpg", "*.jpeg"};

}  // namespace

FileDialog::FileDialog(const char* path)
    : mPath {path}
{
}

auto FileDialog::is_okay() const noexcept -> bool
{
  return mPath != nullptr;
}

auto FileDialog::path() const -> Path
{
  TACTILE_ASSERT(is_okay());
  return {mPath};
}

auto FileDialog::open_folder() -> FileDialog
{
  return {tinyfd_selectFolderDialog("Open Folder", nullptr)};
}

auto FileDialog::open_file() -> FileDialog
{
  return {tinyfd_openFileDialog("Open File", nullptr, 0, nullptr, "Any file", 0)};
}

auto FileDialog::open_map() -> FileDialog
{
  return {tinyfd_openFileDialog("Open Map",
                                nullptr,
                                std::ssize(kMapPatterns),
                                kMapPatterns.data(),
                                kMapPatternDescriptor,
                                0)};
}

auto FileDialog::open_image() -> FileDialog
{
  return {tinyfd_openFileDialog("Open Image",
                                nullptr,
                                std::ssize(kImagePatterns),
                                kImagePatterns.data(),
                                kImagePatternDescriptor,
                                0)};
}

auto FileDialog::save_map() -> FileDialog
{
  return {tinyfd_saveFileDialog("Save Map",
                                nullptr,
                                std::ssize(kMapPatterns),
                                kMapPatterns.data(),
                                kMapPatternDescriptor)};
}

auto FileDialog::save_image() -> FileDialog
{
  return {tinyfd_saveFileDialog("Save Image",
                                nullptr,
                                std::ssize(kImagePatterns),
                                kImagePatterns.data(),
                                kImagePatternDescriptor)};
}

}  // namespace tactile
