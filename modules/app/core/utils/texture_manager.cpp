#include "texture_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <memory>  // unique_ptr

#include <GL/glew.h>
#include <stb_image.h>

#include "logging.hpp"

namespace tactile {
namespace {

struct texture_data_deleter final
{
  void operator()(uchar* data) noexcept { stbi_image_free(data); }
};

using texture_data_ptr = std::unique_ptr<uchar, texture_data_deleter>;

}  // namespace

texture_manager::~texture_manager()
{
  for (const auto texture : mTextures) {
    log_debug("Deleting texture {}", texture);
    glDeleteTextures(1, &texture);
  }

  mTextures.clear();
}

auto texture_manager::load(const std::filesystem::path& path) -> Maybe<comp::texture>
{
  comp::texture texture;
  texture.path = path;

  // Load from file
  texture_data_ptr data{
      stbi_load(path.string().c_str(), &texture.width, &texture.height, nullptr, 4)};
  if (!data) {
    return nothing;
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
