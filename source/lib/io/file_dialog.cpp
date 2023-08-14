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

#include "file_dialog.hpp"

#include <tinyfiledialogs.h>

#include "tactile/core/common/assert.hpp"
#include "tactile/core/containers/array.hpp"

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
                                static_cast<int>(std::ssize(kMapPatterns)),
                                kMapPatterns.data(),
                                kMapPatternDescriptor,
                                0)};
}

auto FileDialog::open_image() -> FileDialog
{
  return {tinyfd_openFileDialog("Open Image",
                                nullptr,
                                static_cast<int>(std::ssize(kImagePatterns)),
                                kImagePatterns.data(),
                                kImagePatternDescriptor,
                                0)};
}

auto FileDialog::save_map() -> FileDialog
{
  return {tinyfd_saveFileDialog("Save Map",
                                nullptr,
                                static_cast<int>(std::ssize(kMapPatterns)),
                                kMapPatterns.data(),
                                kMapPatternDescriptor)};
}

auto FileDialog::save_image() -> FileDialog
{
  return {tinyfd_saveFileDialog("Save Image",
                                nullptr,
                                static_cast<int>(std::ssize(kImagePatterns)),
                                kImagePatterns.data(),
                                kImagePatternDescriptor)};
}

}  // namespace tactile
