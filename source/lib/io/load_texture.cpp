// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "load_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <stb_image.h>

namespace tactile {
namespace {

struct TextureDataDeleter final
{
  void operator()(uchar* data) noexcept { stbi_image_free(data); }
};

using TextureData = Unique<uchar, TextureDataDeleter>;

}  // namespace

auto load_texture(const Path& path) -> Shared<Texture>
{
  Int2 size {};
  const TextureData data {
    stbi_load(path.string().c_str(), &size[0], &size[1], nullptr, STBI_rgb_alpha)};

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
               size.x(),
               size.y(),
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               data.get());

  return std::make_shared<Texture>(id, size, path);
}

}  // namespace tactile
