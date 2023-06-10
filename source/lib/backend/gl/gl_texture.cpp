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

#include "gl_texture.hpp"

#include <glad/glad.h>

namespace tactile::gl {

auto create_texture(const TextureData& texture_data) -> uint
{
  uint id {};
  glGenTextures(1, &id);

  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               texture_data.size.x,
               texture_data.size.y,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               texture_data.pixels.get());

  return id;
}

void destroy_texture(const uint texture_id)
{
  glDeleteTextures(1, &texture_id);
}

}  // namespace tactile::gl
