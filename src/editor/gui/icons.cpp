#include "icons.hpp"

#include <filesystem>  // path

#include <GL/glew.h>

#include <tactile-base/tactile_error.hpp>
#include "core/utils/load_texture.hpp"

namespace Tactile {
namespace {

constinit uint icon_tactile{};

[[nodiscard]] auto LoadIcon(const std::filesystem::path& path) -> uint
{
  if (const auto info = LoadTexture(path)) {
    return info->id;
  }
  else {
    throw TactileError{"Failed to load icon!"};
  }
}

}  // namespace

auto GetTactileIcon() noexcept -> uint
{
  return icon_tactile;
}

auto GetIcon(const LayerType type) -> CStr
{
  switch (type) {
    case LayerType::TileLayer:
      return TAC_ICON_TILE_LAYER;

    case LayerType::ObjectLayer:
      return TAC_ICON_OBJECT_LAYER;

    case LayerType::GroupLayer:
      return TAC_ICON_GROUP_LAYER;

    default:
      throw TactileError{"Failed to recognize layer type!"};
  }
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
