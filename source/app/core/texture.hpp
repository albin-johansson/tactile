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

#pragma once

#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/path.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(TextureHandle)

/// Represents an OpenGL texture.
class Texture final {
 public:
  TACTILE_DELETE_COPY(Texture);
  TACTILE_DEFAULT_MOVE(Texture);

  Texture(uint id, Int2 size, Path path);

  ~Texture() noexcept;

  [[nodiscard]] auto id() const -> uint;

  [[nodiscard]] auto size() const -> const Int2&;

  [[nodiscard]] auto width() const -> int32;

  [[nodiscard]] auto height() const -> int32;

  [[nodiscard]] auto path() const -> const Path&;

 private:
  uint mId;
  Int2 mSize;
  Path mPath;
};

}  // namespace tactile
