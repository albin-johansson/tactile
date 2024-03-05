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

#pragma once

#include "common/type/fstream.hpp"
#include "common/type/path.hpp"
#include "tactile/base/container/maybe.hpp"

namespace tactile {

enum class FileType {
  Text,
  Binary
};

[[nodiscard]] auto open_input_stream(const char* path, FileType type) -> Maybe<IfStream>;
[[nodiscard]] auto open_input_stream(const Path& path, FileType type) -> Maybe<IfStream>;

[[nodiscard]] auto open_output_stream(const char* path, FileType type) -> Maybe<OfStream>;
[[nodiscard]] auto open_output_stream(const Path& path, FileType type) -> Maybe<OfStream>;

}  // namespace tactile
