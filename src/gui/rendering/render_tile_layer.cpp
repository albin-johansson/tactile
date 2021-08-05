#include "render_tile_layer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/map/layers/tile_layer.hpp"
#include "render_info.hpp"
#include "render_tile.hpp"

namespace Tactile {

void RenderTileLayer(const TileLayer& layer,
                     const TilesetManager& tilesets,
                     const RenderInfo& info,
                     const float parentOpacity)
{
  const auto opacity = parentOpacity * layer.GetOpacity();

  const auto endRow = info.bounds.end.GetRow();
  const auto endCol = info.bounds.end.GetColumn();
  for (auto row = info.bounds.begin.GetRow(); row < endRow; ++row)
  {
    for (auto col = info.bounds.begin.GetColumn(); col < endCol; ++col)
    {
      const auto tile = layer.GetTile({row, col});
      if (tile && tile != empty_tile)
      {
        const ImVec2 offset = info.grid_size * ImVec2{static_cast<float>(col),
                                                      static_cast<float>(row)};
        const ImVec2 position = info.map_position + offset;
        RenderTile(*tile, tilesets, position, info.grid_size, opacity);
      }
    }
  }
}

}  // namespace Tactile
