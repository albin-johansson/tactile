#include "icons.hpp"

#include <filesystem>  // path

#include <GL/glew.h>

#include "tactile_stdlib.hpp"
#include "throw.hpp"

namespace tactile {

Icons::Icons(texture_manager& textures)
    : mTactileIcon{textures.load("resources/icon.png").value().id}
{}

auto Icons::GetIcon(const layer_type type) const -> c_str
{
  switch (type) {
    case layer_type::tile_layer:
      return TAC_ICON_TILE_LAYER;

    case layer_type::object_layer:
      return TAC_ICON_OBJECT_LAYER;

    case layer_type::group_layer:
      return TAC_ICON_GROUP_LAYER;

    default:
      ThrowTraced(TactileError{"Failed to recognize layer type!"});
  }
}

}  // namespace tactile
