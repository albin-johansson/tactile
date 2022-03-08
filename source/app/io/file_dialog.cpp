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

namespace tactile {
namespace {

constexpr int _num_map_patterns = 5;
constexpr const char* _map_patterns[] = {"*.yaml", "*.yml", "*.json", "*.tmx", "*.xml"};
constexpr const char* _map_pattern_descriptor = "Map files";

constexpr int _num_image_patterns = 3;
constexpr const char* _image_patterns[] = {"*.png", "*.jpg", "*.jpeg"};
constexpr const char* _image_pattern_descriptor = "Image files";

}  // namespace

file_dialog::file_dialog(const char* path) : mPath{path} {}

auto file_dialog::is_okay() const noexcept -> bool
{
  return mPath != nullptr;
}

auto file_dialog::path() const -> std::filesystem::path
{
  TACTILE_ASSERT(is_okay());
  return {mPath};
}

auto file_dialog::open_file() -> file_dialog
{
  return {tinyfd_openFileDialog("Open File", nullptr, 0, nullptr, "Any file", 0)};
}

auto file_dialog::open_map() -> file_dialog
{
  return {tinyfd_openFileDialog("Open Map",
                                nullptr,
                                _num_map_patterns,
                                _map_patterns,
                                _map_pattern_descriptor,
                                0)};
}

auto file_dialog::open_image() -> file_dialog
{
  return {tinyfd_openFileDialog("Open Image",
                                nullptr,
                                _num_image_patterns,
                                _image_patterns,
                                _image_pattern_descriptor,
                                0)};
}

auto file_dialog::save_map() -> file_dialog
{
  return {tinyfd_saveFileDialog("Save Map",
                                nullptr,
                                _num_map_patterns,
                                _map_patterns,
                                _map_pattern_descriptor)};
}

auto file_dialog::save_image() -> file_dialog
{
  return {tinyfd_saveFileDialog("Save Image",
                                nullptr,
                                _num_image_patterns,
                                _image_patterns,
                                _image_pattern_descriptor)};
}

}  // namespace tactile
