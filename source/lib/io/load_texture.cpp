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

#include "load_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <stb_image.h>

namespace tactile::io {
namespace {

struct TextureDataDeleter final {
  void operator()(uchar* data) noexcept { stbi_image_free(data); }
};

using TextureData = Unique<uchar, TextureDataDeleter>;

}  // namespace

auto load_texture(const Path& path) -> Shared<Texture>
{
  Int2 size {};
  TextureData data {stbi_load(path.string().c_str(), &size.x, &size.y, nullptr, 4)};

  if (!data) {
    return nullptr;
  }

  uint id {};
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               size.x,
               size.y,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               data.get());

  return std::make_shared<Texture>(id, size, path);
}

}  // namespace tactile::io
