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

#include "texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <memory>  // unique_ptr

#include <GL/glew.h>
#include <stb_image.h>

#include "misc/logging.hpp"

namespace tactile {
namespace {

struct texture_data_deleter final
{
  void operator()(uchar* data) noexcept { stbi_image_free(data); }
};

using texture_data_ptr = std::unique_ptr<uchar, texture_data_deleter>;

}  // namespace

TextureManager::~TextureManager()
{
  for (const auto texture : mTextures) {
    log_debug("Deleting texture {}", texture);
    glDeleteTextures(1, &texture);
  }

  mTextures.clear();
}

auto TextureManager::load(const std::filesystem::path& path)
    -> std::optional<comp::Texture>
{
  comp::Texture texture;
  texture.path = path;

  // Load from file
  texture_data_ptr data{
      stbi_load(path.string().c_str(), &texture.width, &texture.height, nullptr, 4)};
  if (!data) {
    return std::nullopt;
  }

  // Create a OpenGL texture identifier
  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // This is required on WebGL for non power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               texture.width,
               texture.height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               data.get());

  log_debug("Loaded texture with ID {}", texture.id);
  mTextures.push_back(texture.id);

  return texture;
}

}  // namespace tactile
