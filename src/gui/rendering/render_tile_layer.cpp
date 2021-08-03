#include "render_tile_layer.hpp"

#include <imgui.h>

#include "core/map/layers/tile_layer.hpp"
#include "render_info.hpp"
#include "render_tile.hpp"

namespace Tactile {

void RenderTileLayer(const TileLayer& layer,
                     const TilesetManager& tilesets,
                     const RenderInfo& info)
{
  const auto opacity = layer.GetOpacity();

  const auto endRow = info.bounds.end.GetRow();
  const auto endCol = info.bounds.end.GetColumn();
  for (auto row = info.bounds.begin.GetRow(); row < endRow; ++row)
  {
    for (auto col = info.bounds.begin.GetColumn(); col < endCol; ++col)
    {
      const auto tile = layer.GetTile({row, col});
      if (tile && tile != empty_tile)
      {
        const ImVec2 pos = {
            info.map_position.x + (info.grid_size.x * static_cast<float>(col)),
            info.map_position.y + (info.grid_size.y * static_cast<float>(row))};
        RenderTile(*tile, tilesets, pos, info.grid_size, opacity);
      }
    }
  }
}

}  // namespace Tactile
