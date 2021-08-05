#include "icons.hpp"

#include "gui/icons.hpp"

namespace Tactile {

auto GetIcon(const LayerType type) noexcept -> std::string_view
{
  switch (type)
  {
    default:
      [[fallthrough]];

    case LayerType::TileLayer:
      return TAC_ICON_TILE_LAYER;

    case LayerType::ObjectLayer:
      return TAC_ICON_OBJECT_LAYER;

    case LayerType::GroupLayer:
      return TAC_ICON_GROUP_LAYER;
  }
}

}  // namespace Tactile
