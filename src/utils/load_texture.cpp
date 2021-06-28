#include "load_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <stb_image.h>

namespace tactile {

/**
 * The LoadTexture function was based on an example from the Dear ImGui wiki,
 * see:
 * https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#Example-for-OpenGL-users
 */
auto LoadTexture(const std::filesystem::path& path) -> Maybe<TextureInfo>
{
  TextureInfo info;

  // Load from file
  auto* data =
      stbi_load(path.string().c_str(), &info.width, &info.height, nullptr, 4);
  if (!data)
  {
    return nothing;
  }

  // Create a OpenGL texture identifier
  glGenTextures(1, &info.texture);
  glBindTexture(GL_TEXTURE_2D, info.texture);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
               info.width,
               info.height,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               data);

  stbi_image_free(data);

  return info;
}

}  // namespace tactile
