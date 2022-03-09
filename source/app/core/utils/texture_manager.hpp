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

#include <filesystem>  // path
#include <vector>      // vector

#include "core/components/texture.hpp"
#include "tactile.hpp"

namespace tactile {

class texture_manager final
{
 public:
  TACTILE_DEFAULT_COPY(texture_manager)
  TACTILE_DEFAULT_MOVE(texture_manager)

  texture_manager() = default;

  ~texture_manager();

  [[nodiscard]] auto load(const std::filesystem::path& path) -> Maybe<comp::Texture>;

 private:
  std::vector<uint> mTextures;
};

}  // namespace tactile
