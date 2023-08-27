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

#include "stream.hpp"

#include <ios>  // ios

namespace tactile {
namespace {

template <typename Stream>
[[nodiscard]] auto _open_stream(const Path& path, const std::ios::openmode mode)
    -> Maybe<Stream>
{
  Stream stream {path, mode};
  if (stream.is_open() && stream.good()) {
    return stream;
  }
  else {
    return kNone;
  }
}

}  // namespace

auto open_input_stream(const Path& path, FileType type) -> Maybe<std::ifstream>
{
  const auto mode = (type == FileType::Binary) ? std::ios::in | std::ios::binary  //
                                               : std::ios::in;
  return _open_stream<std::ifstream>(path, mode);
}

auto open_output_stream(const Path& path, FileType type) -> Maybe<std::ofstream>
{
  const auto mode = (type == FileType::Binary)
                        ? std::ios::out | std::ios::trunc | std::ios::binary
                        : std::ios::out | std::ios::trunc;
  return _open_stream<std::ofstream>(path, mode);
}

}  // namespace tactile
