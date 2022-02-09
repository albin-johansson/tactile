#include "icons.hpp"

#include "misc/throw.hpp"

namespace tactile {

icon_manager::icon_manager(texture_manager& textures)
    : mTactileIcon{textures.load("resources/icon.png").value().id}
{}

auto get_icon(const layer_type type) -> const char*
{
  switch (type) {
    case layer_type::tile_layer:
      return TAC_ICON_TILE_LAYER;

    case layer_type::object_layer:
      return TAC_ICON_OBJECT_LAYER;

    case layer_type::group_layer:
      return TAC_ICON_GROUP_LAYER;

    default:
      throw_traced(tactile_error{"Failed to recognize layer type!"});
  }
}

}  // namespace tactile
