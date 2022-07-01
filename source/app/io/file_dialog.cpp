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

#include "file_dialog.hpp"

#include <tinyfiledialogs.h>

#include "misc/assert.hpp"

namespace tactile::io {
namespace {

constexpr int         _num_map_patterns = 6;
constexpr const char* _map_patterns[] = {"*.yaml",  //
                                         "*.yml",   //
                                         "*.json",  //
                                         "*.tmj",   //
                                         "*.tmx",   //
                                         "*.xml"};
constexpr const char* _map_pattern_descriptor = "Map files";

constexpr int         _num_image_patterns = 3;
constexpr const char* _image_patterns[] = {"*.png", "*.jpg", "*.jpeg"};
constexpr const char* _image_pattern_descriptor = "Image files";

}  // namespace

FileDialog::FileDialog(const char* path) : mPath{path} {}

auto FileDialog::is_okay() const noexcept -> bool
{
  return mPath != nullptr;
}

auto FileDialog::path() const -> std::filesystem::path
{
  TACTILE_ASSERT(is_okay());
  return {mPath};
}

auto FileDialog::open_file() -> FileDialog
{
  return {tinyfd_openFileDialog("Open File", nullptr, 0, nullptr, "Any file", 0)};
}

auto FileDialog::open_map() -> FileDialog
{
  return {tinyfd_openFileDialog("Open Map",
                                nullptr,
                                _num_map_patterns,
                                _map_patterns,
                                _map_pattern_descriptor,
                                0)};
}

auto FileDialog::open_image() -> FileDialog
{
  return {tinyfd_openFileDialog("Open Image",
                                nullptr,
                                _num_image_patterns,
                                _image_patterns,
                                _image_pattern_descriptor,
                                0)};
}

auto FileDialog::save_map() -> FileDialog
{
  return {tinyfd_saveFileDialog("Save Map",
                                nullptr,
                                _num_map_patterns,
                                _map_patterns,
                                _map_pattern_descriptor)};
}

auto FileDialog::save_image() -> FileDialog
{
  return {tinyfd_saveFileDialog("Save Image",
                                nullptr,
                                _num_image_patterns,
                                _image_patterns,
                                _image_pattern_descriptor)};
}

}  // namespace tactile::io
