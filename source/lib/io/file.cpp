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

#include "file.hpp"

#include <ios>  // ios

namespace tactile {
namespace {

[[nodiscard]] auto read_file_helper(const auto& path, const FileType type) -> IfStream
{
  if (type == FileType::Binary) {
    return IfStream {path, std::ios::in | std::ios::binary};
  }
  else {
    return IfStream {path, std::ios::in};
  }
}

[[nodiscard]] auto write_file_helper(const auto& path, const FileType type) -> OfStream
{
  if (type == FileType::Binary) {
    return OfStream {path, std::ios::out | std::ios::trunc | std::ios::binary};
  }
  else {
    return OfStream {path, std::ios::out | std::ios::trunc};
  }
}

}  // namespace

auto read_file(const char* path, const FileType type) -> IfStream
{
  return read_file_helper(path, type);
}

auto read_file(const Path& path, const FileType type) -> IfStream
{
  return read_file_helper(path, type);
}

auto write_file(const char* path, const FileType type) -> OfStream
{
  return write_file_helper(path, type);
}

auto write_file(const Path& path, const FileType type) -> OfStream
{
  return write_file_helper(path, type);
}

}  // namespace tactile
