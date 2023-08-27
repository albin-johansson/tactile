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

#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/math/vector.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/maybe.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(TextureHandle)

/// Represents an OpenGL texture.
class Texture final {
 public:
  TACTILE_DELETE_COPY(Texture);

  Texture(uint id, Int2 size, Path path);

  ~Texture() noexcept;

  Texture(Texture&& other) noexcept;

  auto operator=(Texture&& other) noexcept -> Texture&;

  [[nodiscard]] auto get_id() const -> uint { return mId; }

  [[nodiscard]] auto get_size() const -> Int2 { return mSize; }

  [[nodiscard]] auto get_path() const -> const Path& { return mPath; }

 private:
  uint mId {};
  Int2 mSize {};
  Path mPath;

  void destroy() noexcept;
};

}  // namespace tactile
