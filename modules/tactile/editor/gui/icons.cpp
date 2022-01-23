#include "icons.hpp"

#include <filesystem>  // path

#include <GL/glew.h>
#include <tactile_stdlib.hpp>

#include "throw.hpp"

namespace tactile {

Icons::Icons(TextureManager& textures)
    : mTactileIcon{textures.Load("resources/icon.png").value().id}
{}

auto Icons::GetIcon(const LayerType type) const -> CStr
{
  switch (type) {
    case LayerType::TileLayer:
      return TAC_ICON_TILE_LAYER;

    case LayerType::ObjectLayer:
      return TAC_ICON_OBJECT_LAYER;

    case LayerType::GroupLayer:
      return TAC_ICON_GROUP_LAYER;

    default:
      ThrowTraced(TactileError{"Failed to recognize layer type!"});
  }
}

}  // namespace tactile
