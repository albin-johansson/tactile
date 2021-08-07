#include "icons.hpp"

#include <GL/glew.h>

#include <filesystem>  // path

#include "core/tactile_error.hpp"
#include "utils/load_texture.hpp"

namespace Tactile {
namespace {

constinit uint icon_tactile{};

[[nodiscard]] auto LoadIcon(const std::filesystem::path& path) -> uint
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

auto GetTactileIcon() noexcept -> uint
{
  return icon_tactile;
}

Icons::Icons()
{
  icon_tactile = LoadIcon("resources/icon.png");
}

Icons::~Icons()
{
  glDeleteTextures(1, &icon_tactile);
}

}  // namespace Tactile
