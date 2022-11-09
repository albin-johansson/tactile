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

#include "texture.hpp"

#include <utility>   // move

#include <GL/glew.h>

namespace tactile {

Texture::Texture(const uint id, const Int2 size, Path path)
    : mId {id},
      mSize {size},
      mPath {std::move(path)}
{
}

Texture::~Texture() noexcept
{
  glDeleteTextures(1, &mId);
}

auto Texture::id() const -> uint
{
  return mId;
}

auto Texture::size() const -> const Int2&
{
  return mSize;
}

auto Texture::width() const -> int32
{
  return mSize.x;
}

auto Texture::height() const -> int32
{
  return mSize.y;
}

auto Texture::path() const -> const Path&
{
  return mPath;
}

}  // namespace tactile
