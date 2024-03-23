// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "texture.hpp"

#include <utility>  // move

#include <glad/glad.h>

namespace tactile {

Texture::Texture(const uint id, const Int2 size, Path path)
  : mId {id},
    mSize {size},
    mPath {std::move(path)}
{}

Texture::~Texture() noexcept
{
  destroy();
}

void Texture::destroy() noexcept
{
  if (mId != 0) {
    glDeleteTextures(1, &mId);
  }
}

Texture::Texture(Texture&& other) noexcept
  : mId {other.mId},
    mSize {other.mSize},
    mPath {std::move(other.mPath)}
{
  other.mId = 0;
}

auto Texture::operator=(Texture&& other) noexcept -> Texture&
{
  if (this != &other) {
    destroy();

    mId = other.mId;
    mSize = other.mSize;
    mPath = std::move(other.mPath);

    other.mId = 0;
  }

  return *this;
}

}  // namespace tactile
