#include "icons.hpp"

#include <GL/glew.h>

#include <filesystem>  // path

#include "core/tactile_error.hpp"
#include "utils/load_texture.hpp"

namespace Tactile {
namespace {

inline GLuint icon_tactile;

[[nodiscard]] auto LoadIcon(const std::filesystem::path& path) -> GLuint
{
  if (const auto info = LoadTexture(path))
  {
    return info->texture;
  }
  else
  {
    throw TactileError{"Failed to load icon!"};
  }
}

}  // namespace

void LoadIcons()
{
  icon_tactile = LoadIcon("resources/icon.png");
}

void UnloadIcons()
{
  glDeleteTextures(1, &icon_tactile);
}

auto GetTactileIcon() noexcept -> uint
{
  return icon_tactile;
}

}  // namespace Tactile
