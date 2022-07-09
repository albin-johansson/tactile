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

#include <vector>  // vector

#include "core/common/filesystem.hpp"
#include "core/common/ints.hpp"
#include "core/common/macros.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"

namespace tactile {

struct TextureInfo final
{
  uint     id {};
  Vector2i size {};
  fs::path path;
};

class TextureManager final
{
 public:
  TACTILE_DEFAULT_COPY(TextureManager);
  TACTILE_DEFAULT_MOVE(TextureManager);

  TextureManager() = default;

  ~TextureManager();

  [[nodiscard]] auto load(const fs::path& path) -> Maybe<TextureInfo>;

 private:
  std::vector<uint> mTextures;
};

}  // namespace tactile
