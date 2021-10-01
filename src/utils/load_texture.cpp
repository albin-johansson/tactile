#include "load_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <stb_image.h>

#include <centurion.hpp>
#include <memory>  // unique_ptr
#include <vector>  // vector

#include "common/ints.hpp"

namespace Tactile {
namespace {

struct TextureDataDeleter final
{
  void operator()(unsigned char* data) noexcept
  {
    stbi_image_free(data);
  }
};

using TextureDataPtr = std::unique_ptr<unsigned char, TextureDataDeleter>;

inline std::vector<uint> textures;

}  // namespace

void UnloadTextures()
{
  for (const auto texture : textures) {
    CENTURION_LOG_DEBUG("Deleting texture %u...", texture);
    glDeleteTextures(1, &texture);
  }

  textures.clear();
}

/**
 * The LoadTexture function was based on an example from the Dear ImGui wiki,
 * see:
 * https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#Example-for-OpenGL-users
 */
auto LoadTexture(const std::filesystem::path& path) -> Maybe<Texture>
{
  Texture texture;
  texture.path = path;

  // Load from file
  TextureDataPtr data{
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

  CENTURION_LOG_DEBUG("Loaded texture with ID: %u", texture.id);
  textures.push_back(texture.id);

  return texture;
}

}  // namespace Tactile
