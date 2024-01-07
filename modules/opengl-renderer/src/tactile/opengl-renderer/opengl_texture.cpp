// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/opengl-renderer/opengl_texture.hpp"

#include <utility>  // exchange, move

#include <glad/glad.h>

#include "tactile/foundation/io/texture_io.hpp"
#include "tactile/foundation/platform/bits.hpp"
#include "tactile/opengl-renderer/opengl_error.hpp"

namespace tactile::opengl {

auto OpenGLTexture::load(const FilePath& image_path) -> Result<OpenGLTexture>
{
  if (const auto texture_data = load_texture(image_path)) {
    uint id {};
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    if (const auto err = glGetError(); err != GL_NONE) {
      return unexpected(make_opengl_error(to_opengl_error(err)));
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    if (const auto err = glGetError(); err != GL_NONE) {
      return unexpected(make_opengl_error(to_opengl_error(err)));
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (const auto err = glGetError(); err != GL_NONE) {
      return unexpected(make_opengl_error(to_opengl_error(err)));
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    if (const auto err = glGetError(); err != GL_NONE) {
      return unexpected(make_opengl_error(to_opengl_error(err)));
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if (const auto err = glGetError(); err != GL_NONE) {
      return unexpected(make_opengl_error(to_opengl_error(err)));
    }

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 texture_data->size.x,
                 texture_data->size.y,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 texture_data->pixels.get());
    if (const auto err = glGetError(); err != GL_NONE) {
      return unexpected(make_opengl_error(to_opengl_error(err)));
    }

    return OpenGLTexture {id, texture_data->size, image_path};
  }
  else {
    return propagate_unexpected(texture_data);
  }
}

OpenGLTexture::OpenGLTexture(const id_type id, const Int2 size, FilePath path)
  : mID {id},
    mSize {size},
    mPath {std::move(path)}
{}

OpenGLTexture::~OpenGLTexture() noexcept
{
  _dispose();
}

void OpenGLTexture::_dispose() noexcept
{
  if (mID != 0) {
    glDeleteTextures(1, &mID);
    mID = 0;
  }
}

OpenGLTexture::OpenGLTexture(OpenGLTexture&& other) noexcept
  : mID {std::exchange(other.mID, 0)},
    mSize {std::exchange(other.mSize, Int2 {})},
    mPath {std::exchange(other.mPath, FilePath {})}
{}

auto OpenGLTexture::operator=(OpenGLTexture&& other) noexcept -> OpenGLTexture&
{
  if (this != &other) {
    _dispose();

    mID = std::exchange(other.mID, 0);
    mSize = std::exchange(other.mSize, Int2 {});
    mPath = std::exchange(other.mPath, FilePath {});
  }

  return *this;
}

auto OpenGLTexture::get_handle() const -> void*
{
  return interpret_as<void*>(static_cast<uintptr>(mID));
}

auto OpenGLTexture::get_size() const -> Int2
{
  return mSize;
}

auto OpenGLTexture::get_path() const -> const FilePath&
{
  return mPath;
}

}  // namespace tactile::opengl
